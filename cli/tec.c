#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>

#include "tec.h"
#include "aux/config.h"
#include "aux/toggle.h"

#define tec_getopt_unset()  \
    do {                    \
        optind = 0;         \
    } while (0)             \

static char alias_name[20];
static char alias_cmd[20];

/*
typedef struct tec_cli_status {
    const char *fmt;
    char *obj;
    char *msg;
    int status;
} tec_cli_status_t;
*/

struct config teccfg;
char *unitkeys[] = { "prio", "type", "date", "desc", };

unsigned int nunitkey = sizeof(unitkeys) / sizeof(unitkeys[0]);

static builtin_t builtins[] = {
    {.name = "add",.func = &tec_cli_add,.option = TEC_SETUP_HARD},
    {.name = "cat",.func = &tec_cli_cat,.option = TEC_SETUP_HARD},
    {.name = "cd",.func = &tec_cli_cd,.option = TEC_SETUP_HARD},
    {.name = "cfg",.func = &tec_cli_cfg,.option = TEC_SETUP_HARD},
    {.name = "env",.func = &tec_cli_env,.option = TEC_SETUP_HARD},
    {.name = "desk",.func = &tec_cli_desk,.option = TEC_SETUP_HARD},
    {.name = "help",.func = &tec_cli_help,.option = TEC_SETUP_SOFT},
    {.name = "init",.func = &tec_cli_init,.option = TEC_SETUP_SOFT},
    {.name = "ls",.func = &tec_cli_ls,.option = TEC_SETUP_HARD},
    {.name = "mv",.func = &tec_cli_mv,.option = TEC_SETUP_HARD},
    {.name = "rm",.func = &tec_cli_rm,.option = TEC_SETUP_HARD},
    {.name = "set",.func = &tec_cli_set,.option = TEC_SETUP_HARD},
    {.name = "version",.func = &tec_cli_version,.option = TEC_SETUP_HARD},
};

static int tec_setup(int setuplvl)
{
    int status = LIBTEC_OK;

    if (setuplvl == TEC_SETUP_SOFT)     /* no filesystem check.  */
        ;
    else if (setuplvl == TEC_SETUP_HARD) {      /* check filesystem.  */
        status = tec_check_db(teccfg.base.task);
    }
    return status;
}

static int is_alias(char *pgndir, const char *cmd)
{
    struct tec_alias *head;

    for (head = teccfg.alias; head != NULL;) {
        if (strcmp(cmd, head->name) == 0) {
            //printf("-> alias found\n");
            strcpy(alias_name, head->name);
            strcpy(alias_cmd, head->cmd);
            return true;
        }
        //printf("--- '%s' : '%s' - '%s'\n", cmd, head->name, head->cmd);
        head = head->next;
    }
    return false;
}

static builtin_t *is_builtin(const char *cmd)
{
    for (int idx = 0; idx < ARRAY_SIZE(builtins); ++idx)
        if (strcmp(cmd, builtins[idx].name) == 0)
            return &builtins[idx];
    return NULL;
}

static int is_plugin(char *pgndir, const char *pgname)
{
    FILE *fp;
    char path[PATH_MAX + 1];

    sprintf(path, "%s/%s/%s", pgndir, pgname, pgname);

    if ((fp = fopen(path, "r")) == NULL)
        return false;
    fclose(fp);
    return true;
}

static int run_builtin(tec_argvec_t *argvec, builtin_t *cmd)
{
    int status;
    tec_ctx_t ctx = CTX_INIT;

    if ((status = tec_setup(cmd->option)) != LIBTEC_OK)
        return elog(status, "setup failed: %s", tec_strerror(status));

    return cmd->func(argvec, &ctx);
}

static int run_plugin(tec_argvec_t *argvec)
{
    int status;
    tec_ctx_t ctx = CTX_INIT;

    if ((status = tec_setup(TEC_SETUP_HARD)) != LIBTEC_OK)
        return elog(status, "setup failed: %s", tec_strerror(status));

    return tec_cli_pgn(argvec, &ctx);
}

/* TODO: add support to include alias into alias (i.e. recursive aliases).  */
static int run_alias(tec_argvec_t *argvec)
{
    char *tok;
    int status = LIBTEC_OK;

    tok = strtok(alias_cmd, " ");

    if (is_plugin(teccfg.base.pgn, tok) == true) {
        dlog(1, "alias execute as plugin: '%s'", alias_name);
        argvec_replace(argvec, 0, tok, strlen(tok));

        while ((tok = strtok(NULL, " ")) != NULL) {
            argvec_add(argvec, tok);
        }
        status = run_plugin(argvec);
    } else {
        dlog(1, "alias execute as builtin: '%s'", alias_name);
        argvec_replace(argvec, 0, tok, strlen(tok));

        while ((tok = strtok(NULL, " ")) != NULL) {
            argvec_add(argvec, tok);
        }
        status = run_builtin(argvec, is_builtin(alias_cmd));
    }
    return status;
}

static int valid_toggle(char *tog)
{
    if (strcmp(tog, "on") == 0)
        return true;
    else if (strcmp(tog, "off") == 0)
        return false;
    return -1;
}

void argvec_init(tec_argvec_t *vec)
{
    int size = 2;

    if ((vec->argv = malloc(size * sizeof(vec->argv))) == NULL) {
        elog(1, "'%s': memory allocation failed", __FUNCTION__);
        exit(1);
    }

    /* Prevent invalid pointer dereference. Used in case when default
     * env name, desk name, task ID used.  */
    for (int i = 0; i < size; ++i)
        vec->argv[i] = NULL;

    vec->used = 0;
    vec->offset = 0;
    vec->size = size;
}

void argvec_show(tec_argvec_t *vec)
{
    for (int i = 0; i < vec->used; ++i) {
        printf("argvec_show: %s[%d] - used: %d\n", vec->argv[i], i, vec->used);
    }
}

void argvec_add(tec_argvec_t *vec, const char *arg)
{
    /* The last element is never used that's why minus one.
     * It is a NULL terminator to mimic C argv.  */
    if (vec->used >= vec->size - 1) {
        vec->size *= 2;
        if ((vec->argv =
             realloc(vec->argv, vec->size * sizeof(char *))) == NULL) {
            elog(1, "'%s': memory allocation failed", __FUNCTION__);
            exit(1);
        }

        /* Prevent invalid pointer dereference. Used in case when default
         * env name, desk name, task ID used.  */
        for (int i = vec->used; i < vec->size; ++i)
            vec->argv[i] = NULL;
    }
    vec->argv[vec->used++] = strdup(arg);
}

void argvec_parse(tec_argvec_t *vec, int argc, const char **argv)
{
    for (int i = 0; i < argc; i++)
        argvec_add(vec, argv[i]);
}

void argvec_replace(tec_argvec_t *vec, int vec_idx, char *arg, int argsiz)
{
    free(vec->argv[vec_idx]);   /* free previous key value.  */
    if ((vec->argv[vec_idx] = strndup(arg, argsiz)) == NULL) {
        elog(1, "'%s': memory allocation failed", __FUNCTION__);
        exit(1);
    }
}

void argvec_offset(tec_argvec_t *vec, int offset)
{
    assert((vec->used - offset) >= 0);
    vec->offset += offset;
    vec->used -= offset;
    vec->argv += offset;
}

void argvec_free(tec_argvec_t *vec)
{
    vec->argv = vec->argv - vec->offset;        /* Restore pointer to first element.  */
    for (int i = 0; i < vec->size; ++i)
        free(vec->argv[i]);
    free(vec->argv);
}

void argvec_remove(tec_argvec_t *vec, int index)
{
    /* Validate index */
    if (index < 0 || index >= vec->used) {
        elog(1, "argvec_remove: index out of range");
        return;
    }

    /* Free the string at the specified index */
    free(vec->argv[index]);

    /* Shift all remaining elements left by one */
    for (int i = index; i < vec->used - 1; i++) {
        vec->argv[i] = vec->argv[i + 1];
    }

    /* Decrement used count */
    vec->used--;

    /* Set the now-unused last element to NULL */
    vec->argv[vec->used] = NULL;
}

bool tec_cli_get_user_choice(void)
{
    char choice[10] = { 0 };

    fgets(choice, sizeof(choice), stdin);
    if (choice[0] == 'y' || choice[0] == 'Y')
        return true;
    return false;
}

int check_arg_env(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_env_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current env");
        return status;
    } else if ((status = tec_env_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->env, tec_strerror(status));
        return status;
    } else if (is_valid_length(args->env, ENVSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->env, "env name is too long");
        return status;
    } else if ((status = tec_env_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->env, tec_strerror(status));
        return status;
    }
    return 0;
}

int check_arg_desk(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_desk_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current desk");
        return status;
    } else if ((status = tec_desk_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->desk, tec_strerror(status));
        return status;
    } else if (is_valid_length(args->desk, DESKSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->desk, "desk name is too long");
        return status;
    } else if ((status = tec_desk_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->desk, tec_strerror(status));
        return status;
    }
    return 0;
}

int check_arg_task(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_task_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current task");
        return status;
    } else if ((status = tec_task_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->taskid, tec_strerror(status));
        return status;
    } else if (is_valid_length(args->taskid, IDSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->taskid, "task ID is too long");
        return status;
    } else if ((status = tec_task_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->taskid, tec_strerror(status));
        return status;
    }
    return 0;
}

int tec_pwd_env(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s\n";
        const char *debug_fmt = "PWD env: '%s'";

        dlog(1, debug_fmt, args->env);
        fprintf(fp, fmt, taskdir, args->env);
        return fclose(fp);
    }
    return 1;

}

int tec_pwd_desk(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s/%s\n";
        const char *debug_fmt = "PWD env: '%s', desk: '%s'";

        dlog(1, debug_fmt, args->env, args->desk);
        fprintf(fp, fmt, taskdir, args->env, args->desk);
        return fclose(fp);
    }
    return 1;
}

int tec_pwd_task(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s/%s/%s\n";
        const char *debug_fmt = "PWD env: '%s', desk: '%s', task ID: '%s'";

        dlog(1, debug_fmt, args->env, args->desk, args->taskid);
        fprintf(fp, fmt, taskdir, args->env, args->desk, args->taskid);
        return fclose(fp);
    }
    return 1;
}

int tec_pwd_unset(void)
{
    FILE *fp;

    if ((fp = fopen(PWDFILE, "w")) == NULL)
        return 1;
    return fclose(fp);
}

int elog(int status, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, PROGRAM ": ");
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");
    va_end(arg);
    return status;
}

int dlog(int level, const char *fmt, ...)
{
    if (teccfg.opts.debug == false)
        return 0;

    va_list arg;
    va_start(arg, fmt);
    printf(PROGRAM ": ");
    vprintf(fmt, arg);
    printf("\n");
    va_end(arg);
    return 0;
}

int llog(int status, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    printf(PROGRAM ": ");
    vprintf(fmt, arg);
    printf("\n");
    va_end(arg);
    return 0;
}

int main(int argc, const char **argv)
{
    tec_opt_t opts;
    const char *cmd;
    tec_base_t base;
    builtin_t *builtin;
    tec_argvec_t argvec;
    char *option, *togfmt;
    int c, i, status, showhelp, showversion;

    cmd = NULL;
    status = LIBTEC_OK;
    showhelp = showversion = false;
    base.pgn = base.task = option = NULL;
    opts.color = opts.debug = opts.hook = NONEBOOL;
    togfmt = "option `-%c' accepts either 'on' or 'off'";

    argvec_init(&argvec);
    argvec_parse(&argvec, argc, argv);

    /* Parse util itself options.  */
    while ((c = getopt(argvec.used, argvec.argv, "+:hC:D:F:H:P:T:V")) != -1) {
        switch (c) {
        case 'h':
            showhelp = true;
            break;
        case 'C':
            if ((opts.color = valid_toggle(optarg)) == -1)
                return elog(1, togfmt, c);
            break;
        case 'D':
            if ((opts.debug = valid_toggle(optarg)) == -1)
                return elog(1, togfmt, c);
            break;
        case 'F':
            return elog(1, "option `-%c' under development", c);
        case 'H':
            if ((opts.hook = valid_toggle(optarg)) == -1)
                return elog(1, togfmt, c);
            break;
        case 'P':
            base.pgn = optarg;
            break;
        case 'T':
            base.task = optarg;
            break;
        case 'V':
            showversion = true;
            break;
        case ':':
            return elog(1, "option `-%c' requires an argument", optopt);
        default:
            return elog(1, "invalid option `-%c'", optopt);
        }
    }

    i = optind;
    tec_pwd_unset();
    tec_getopt_unset();         /* Unset option index cuz subcommands use getopt too.  */

    if (showhelp == true) {
        argvec_replace(&argvec, argvec.used - 1, "help", strlen("help"));
    } else if (showversion == true) {
        argvec_replace(&argvec, argvec.used - 1, "version", strlen("version"));
    }
    if ((cmd = argvec.argv[i]) == NULL) {
        status = 1;
        help_list_pretty_commands();
        goto err;
    }

    if (tec_config_init(&teccfg))
        return elog(1, "could init config file");
    else if (tec_config_parse(&teccfg))
        return elog(1, "could parse config file");
    else if (tec_config_set_base(&base))
        return elog(1, "could set config base directories");
    else if (tec_config_set_options(&opts))
        return elog(1, "could set config options");

    argvec_offset(&argvec, i + 1);
    if (is_alias(teccfg.base.pgn, cmd) == true) {
        status = run_alias(&argvec);
    } else if (is_plugin(teccfg.base.pgn, cmd) == true) {
        status = run_plugin(&argvec);
    } else if ((builtin = is_builtin(cmd)) != NULL) {
        status = run_builtin(&argvec, builtin);
    } else {
        status = elog(1, "'%s': no such command, alias or plugin", cmd);
    }

 err:
    tec_config_destroy(&teccfg);
    argvec_free(&argvec);
    return status;
}
