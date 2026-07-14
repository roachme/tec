#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#include "tec.h"
#include "aux/aux.h"
#include "aux/log.h"
#include "aux/pwd.h"
#include "aux/errno.h"
#include "aux/argvec.h"

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

static tec_cmd_t tec_cmd_types[] = {
    {.name = "alias",.type = tec_cli_is_alias},
    {.name = "plugin",.type = tec_cli_is_plugin},
    {.name = "builtin",.type = tec_cli_is_builtin},
};

static tec_cmd_t tec_cmds[] = {
    {.name = "add",.func = &tec_cli_add,.option = TEC_SETUP_HARD},
    {.name = "cat",.func = &tec_cli_cat,.option = TEC_SETUP_HARD},
    {.name = "cd",.func = &tec_cli_cd,.option = TEC_SETUP_HARD},
    {.name = "cfg",.func = &tec_cli_cfg,.option = TEC_SETUP_SOFT},
    {.name = "env",.func = &tec_cli_env,.option = TEC_SETUP_HARD},
    {.name = "desk",.func = &tec_cli_desk,.option = TEC_SETUP_HARD},
    {.name = "help",.func = &tec_cli_help,.option = TEC_SETUP_SOFT},
    {.name = "init",.func = &tec_cli_init,.option = TEC_SETUP_SOFT},
    {.name = "ls",.func = &tec_cli_ls,.option = TEC_SETUP_HARD},
    {.name = "mv",.func = &tec_cli_mv,.option = TEC_SETUP_HARD},
    {.name = "rm",.func = &tec_cli_rm,.option = TEC_SETUP_HARD},
    {.name = "set",.func = &tec_cli_set,.option = TEC_SETUP_HARD},
    {.name = "version",.func = &tec_cli_version,.option = TEC_SETUP_SOFT},
    {.name = "_alias_",.func = &tec_cli_alias,.option = TEC_SETUP_SOFT},
    {.name = "_pgn_",.func = &tec_cli_pgn,.option = TEC_SETUP_SOFT},
};

static int toggle2bool(const char *tog)
{
    size_t len = strlen(tog);

    if (len <= 2 && strcmp(tog, "on") == 0)
        return true;
    else if (len <= 3 && strcmp(tog, "off") == 0)
        return false;
    return NONEBOOL;
}

static int cmd_setup(int setuplvl, const tec_cfg_t *cfg)
{
    int status = TEC_OK;

    if (setuplvl == TEC_SETUP_SOFT)     /* no filesystem check.  */
        ;
    else if (setuplvl == TEC_SETUP_HARD) {      /* check filesystem.  */
        status = tec_check_db(cfg->base.task);
    }
    return status;
}

static bool cmd_is_naughty(const char *cmdname)
{
    if (!isalpha(cmdname[0]))
        return true;
    return false;
}

static tec_cmd_t *cmd_get(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_cmd_t *cmd;

    for (size_t i = 0; i < ARRAY_SIZE(tec_cmd_types); ++i)
        if ((cmd = tec_cmd_types[i].type(argvec, cfg)))
            return cmd;
    return NULL;
}

static char *path_generic(char *buf, int bufsiz, const char *fmt, ...)
{
    int len;
    va_list arg;

    va_start(arg, fmt);
    len = vsnprintf(buf, bufsiz, fmt, arg);
    va_end(arg);
    return len >= bufsiz ? NULL : buf;
}

static char *path_pgn(const char *taskdir, const char *pgn)
{
    const char *fmt = "%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, pgn, pgn);
}

tec_cmd_t *tec_cli_is_alias(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_alias_t *head;
    const char *cmdname = argvec->argv[0];

    for (head = cfg->alias; head != NULL; head = head->next)
        if (strcmp(cmdname, head->name) == 0)
            return &tec_cmds[ARRAY_SIZE(tec_cmds) - 2];
    return NULL;
}

tec_cmd_t *tec_cli_is_plugin(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    FILE *fp;
    const char *cmdname = argvec->argv[0];
    char *path = path_pgn(cfg->base.pgn, cmdname);

    if ((fp = fopen(path, "r"))) {
        fclose(fp);
        return &tec_cmds[ARRAY_SIZE(tec_cmds) - 1];
    }
    return NULL;
}

tec_cmd_t *tec_cli_is_builtin(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    (void)cfg;
    const char *cmdname = argvec->argv[0];

    for (size_t idx = 0; idx < ARRAY_SIZE(tec_cmds); ++idx)
        if (strcmp(cmdname, tec_cmds[idx].name) == 0)
            return &tec_cmds[idx];
    return NULL;
}

int tec_cli_cmd_run(tec_cmd_t *cmd, tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int status = TEC_OK;

    if ((status = cmd_setup(cmd->option, cfg)) != TEC_OK)
        return TEC_LOG_E("setup failed: %s", tec_strerror(status));
    return cmd->func(argvec, cfg);
}

int main(int argc, const char **argv)
{
    int c;
    tec_cmd_t *cmd;
    int status = TEC_OK;
    tec_argvec_t argvec;
    tec_cfg_t *cfg = &teccfg;
    const char *cmdname = NULL;
    const char *togfmt = "option `-%c' accepts either 'on' or 'off'";

    tec_config_init(cfg);
    tec_cli_pwd_unset();
    argvec_init(&argvec);
    argvec_parse(&argvec, argc, argv);

    /* Parse util itself options.  */
    while ((c = getopt(argvec.used, argvec.argv, "+:f:hvC:D:H:P:T:")) != -1) {
        switch (c) {
        case 'f':
            cfg->base.cfg = strdup(optarg);
            break;
        case 'h':
            argvec_add(&argvec, "help");
            break;
        case 'v':
            argvec_add(&argvec, "version");
            break;
        case 'C':
            if ((cfg->opts.color = toggle2bool(optarg)) == NONEBOOL)
                return TEC_LOG_E(togfmt, c);
            break;
        case 'D':
            if ((cfg->opts.debug = toggle2bool(optarg)) == NONEBOOL)
                return TEC_LOG_E(togfmt, c);
            break;
        case 'H':
            if ((cfg->opts.hook = toggle2bool(optarg)) == NONEBOOL)
                return TEC_LOG_E(togfmt, c);
            break;
        case 'P':
            if (cfg->base.pgn)
                free(cfg->base.pgn);
            cfg->base.pgn = strdup(optarg);
            break;
        case 'T':
            if (cfg->base.task)
                free(cfg->base.task);
            cfg->base.task = strdup(optarg);
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("tec");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("tec");
        }
    }

    argvec.i = optind;
    tec_getopt_unset();
    argvec_offset(&argvec, argvec.i);   /* Skip program name and options if any.  */

    if (tec_config_parse(cfg)) {
        status = TEC_LOG_E("cannot parse config file");
        goto err;
    } else if ((cmdname = argvec.argv[0]) == NULL) {
        status = EXIT_FAILURE;
        tec_cli_help_list();
        goto err;
    } else if (cmd_is_naughty(cmdname) == true) {
        status = TEC_LOG_E("'%s': naughty command", cmdname);
        goto err;
    } else if ((cmd = cmd_get(&argvec, cfg)) == NULL) {
        status = TEC_LOG_E("'%s': no such command, alias or plugin", cmdname);
        goto err;
    } else if ((status = tec_cli_cmd_run(cmd, &argvec, cfg))) {
        goto err;
    }

 err:
    tec_config_destroy(cfg);
    argvec_deinit(&argvec);
    return status == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}
