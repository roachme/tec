#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>

#include "tec.h"
#include "aux/config.h"
#include "aux/toggle.h"

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
};

static int show_version(void)
{
    printf("%s version %s\n", PROGRAM, VERSION);
    return 0;
}

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

static int run_builtin(int argc, const char **argv, builtin_t *cmd)
{
    int status;
    tec_ctx_t ctx = CTX_INIT;

    if ((status = tec_setup(cmd->option)) != LIBTEC_OK)
        return elog(status, "setup failed: %s", tec_strerror(status));
    return cmd->func(argc, argv, &ctx);
}

static int run_plugin(int argc, const char **argv)
{
    int status;
    tec_ctx_t ctx = CTX_INIT;

    if ((status = tec_setup(TEC_SETUP_HARD)) != LIBTEC_OK)
        return elog(status, "setup failed: %s", tec_strerror(status));
    return tec_cli_pgn(argc, argv, &ctx);
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
        elog(1, "malloc failed");
        exit(1);
    }

    /* Prevent invalid pointer dereference. Used in case when default
     * env name, desk name, task ID used.  */
    for (int i = 0; i < size; ++i)
        vec->argv[i] = NULL;

    vec->used = 0;
    vec->size = size;
}

void argvec_add(tec_argvec_t *vec, const char *arg)
{
    /* The last element is never used that's why minus one.
     * It is a NULL terminator to mimic C argv.  */
    if (vec->used >= vec->size - 1) {
        vec->size *= 2;
        if ((vec->argv =
             realloc(vec->argv, vec->size * sizeof(char *))) == NULL) {
            elog(1, "realloc failed");
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
        elog(1, "strndup failed");
        exit(1);
    }
}

void argvec_free(tec_argvec_t *vec)
{
    for (int i = 0; i < vec->used; ++i)
        free(vec->argv[i]);
    free(vec->argv);
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
    tec_base_t base;
    builtin_t *builtin;
    int c, i, status, showhelp, showversion;
    const char *cmd;
    char *option, *togfmt;

    cmd = NULL;
    showhelp = showversion = false;
    opts.color = opts.debug = opts.hook = NONEBOOL;
    base.pgn = base.task = option = NULL;
    togfmt = "option `-%c' accepts either 'on' or 'off'";

    /* Parse util itself options.  */
    while ((c = getopt(argc, (char **)argv, "+:hC:D:F:H:P:T:V")) != -1) {
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
    optind = 0;                 /* Unset option index cuz subcommands use getopt too.  */
    tec_pwd_unset();

    cmd = argv[i];

    if (showhelp == true)
        cmd = "help";
    else if (showversion == true)
        return show_version();
    else if (cmd == NULL) {
        /* The user didn't specify a command; give them help */
        help_list_pretty_commands();
        exit(1);
    }

    if (tec_config_init(&teccfg))
        return elog(1, "could init config file");
    else if (tec_config_parse(&teccfg))
        return elog(1, "could parse config file");
    else if (tec_config_set_base(&base))
        return elog(1, "could set config base directories");
    else if (tec_config_set_options(&opts))
        return elog(1, "could set config options");

    if (is_plugin(teccfg.base.pgn, cmd) == true)
        status = run_plugin(argc - i, argv + i);
    else if ((builtin = is_builtin(cmd)) != NULL)
        status = run_builtin(argc - i, argv + i, builtin);
    else
        status = elog(1, "'%s': no such command or plugin", cmd);

    tec_config_destroy(&teccfg);
    return status;
}
