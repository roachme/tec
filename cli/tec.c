#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#include "tec.h"
#include "aux/log.h"
#include "aux/pwd.h"
#include "aux/argvec.h"
#include "aux/config.h"
#include "aux/toggle.h"

#define tec_getopt_unset()  \
    do {                    \
        optind = 0;         \
    } while (0)             \

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

static tec_builtin_t builtins[] = {
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
    {.name = "version",.func = &tec_cli_version,.option = TEC_SETUP_SOFT},
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

// TODO: return tec_alias_t *. This way no need for static variables in the file
static tec_alias_t *is_alias(char *pgndir, const char *cmd)
{
    struct tec_alias *head;

    for (head = teccfg.alias; head != NULL; head = head->next) {
        if (strcmp(cmd, head->name) == 0) {
            return head;
        }
    }
    return NULL;
}

// Resolve nested alias
/*
static int alias_resolve()
{
    return 0;
}
*/

static tec_builtin_t *is_builtin(const char *cmd)
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

static int run_builtin(tec_argvec_t *argvec, tec_builtin_t *cmd)
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
static int run_alias(tec_argvec_t *argvec, tec_alias_t *alias)
{
    char *tok, *cmd;
    int status = LIBTEC_OK;

    tok = cmd = strtok(alias->cmd, " ");
    argvec_replace(argvec, 0, tok, strlen(tok));

    while ((tok = strtok(NULL, " ")) != NULL) {
        argvec_add(argvec, tok);
    }

    if (is_plugin(teccfg.base.pgn, cmd) == true) {
        dlog(1, "alias execute as plugin: '%s'", alias->name);
        status = run_plugin(argvec);
    } else {
        dlog(1, "alias execute as builtin: '%s'", alias->name);
        status = run_builtin(argvec, is_builtin(alias->cmd));
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

bool tec_cli_get_user_choice(void)
{
    char choice[10] = { 0 };

    fgets(choice, sizeof(choice), stdin);
    if (choice[0] == 'y' || choice[0] == 'Y')
        return true;
    return false;
}

int main(int argc, const char **argv)
{
    tec_opt_t opts;
    tec_base_t base;
    int c, i, status;
    tec_alias_t *alias;
    tec_argvec_t argvec;
    tec_builtin_t *builtin;
    const char *cmd, *togfmt;
    int opt_help, opt_version;

    cmd = NULL;
    status = LIBTEC_OK;
    base.pgn = base.task = NULL;
    opt_help = opt_version = false;
    opts.color = opts.debug = opts.hook = NONEBOOL;
    togfmt = "option `-%c' accepts either 'on' or 'off'";

    argvec_init(&argvec);
    argvec_parse(&argvec, argc, argv);

    /* Parse util itself options.  */
    while ((c = getopt(argvec.used, argvec.argv, "+:hC:D:F:H:P:T:V")) != -1) {
        switch (c) {
        case 'h':
            opt_help = true;
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
            opt_version = true;
            break;
        case ':':
            return elog(1, "option `-%c' requires an argument", optopt);
        default:
            return elog(1, "invalid option `-%c'", optopt);
        }
    }

    i = optind;
    tec_pwd_unset();
    tec_getopt_unset();
    argvec_offset(&argvec, i);  /* Skip program name and options if any.  */

    if (opt_help == true) {
        argvec_add(&argvec, "help");
    } else if (opt_version == true) {
        argvec_add(&argvec, "version");
    }

    if ((cmd = argvec.argv[0]) == NULL) {
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

    if ((alias = is_alias(teccfg.base.pgn, cmd)) != NULL) {
        status = run_alias(&argvec, alias);
    } else if (is_plugin(teccfg.base.pgn, cmd) == true) {
        status = run_plugin(&argvec);
    } else if ((builtin = is_builtin(cmd)) != NULL) {
        status = run_builtin(&argvec, builtin);
    } else {
        status = elog(1, "'%s': no such command, alias or plugin", cmd);
    }

 err:
    tec_config_destroy(&teccfg);
    argvec_deinit(&argvec);
    return status;
}
