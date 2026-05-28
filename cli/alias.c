#include <string.h>

#include "tec.h"
#include "aux/config.h"

// TODO: Resolve nested alias, i.e. alias that includes alias
/*
static int alias_resolve()
{
    return 0;
}
*/

static tec_alias_t *get_alias(const char *cmdname, tec_cfg_t *cfg)
{
    tec_alias_t *head;

    for (head = cfg->alias; head != NULL; head = head->next)
        if (strcmp(cmdname, head->name) == 0)
            return head;
    return NULL;
}

static char *resolve_alias(tec_argvec_t *argvec, tec_alias_t *alias)
{
    char *tok, *cmdname;

    tok = cmdname = strtok(alias->cmd, " ");
    argvec_replace(argvec, 0, tok, strlen(tok));

    while ((tok = strtok(NULL, " ")) != NULL)
        argvec_add(argvec, tok);
    return cmdname;
}

int tec_cli_alias(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_cmd_t *cmd;
    int status = TEC_OK;
    tec_alias_t *alias = NULL;
    char *cmdname = argvec->argv[0];

    if ((alias = get_alias(cmdname, cfg)) == NULL) {
        return TEC_LOG_E("'%s': could not get alias", cmdname);
    } else if ((cmdname = resolve_alias(argvec, alias)) == NULL) {
        return TEC_LOG_E("'%s': could not resolve alias", cmdname);
    }

    if ((cmd = tec_cli_is_plugin(argvec, cfg))) {
        TEC_LOG_D("alias execute as plugin: '%s'", alias->name);
        status = tec_cli_cmd_run(cmd, argvec, cfg);
    } else if ((cmd = tec_cli_is_builtin(argvec, NULL))) {
        TEC_LOG_D("alias execute as builtin: '%s'", alias->name);
        status = tec_cli_cmd_run(cmd, argvec, cfg);
    }
    return status;
}
