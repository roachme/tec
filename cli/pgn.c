#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tec.h"
#include "aux/config.h"

typedef struct tec_pgn_cmd {
    size_t size;
    size_t offset;
    char name[CMDSIZ + 1];
    char cmd[BUFSIZ + 1];
} tec_pgn_cmd_t;

static void pgn_cmd_init(tec_pgn_cmd_t *cmd, char *name)
{
    strcpy(cmd->name, name);
    cmd->cmd[0] = '\0';
    cmd->offset = 0;
    cmd->size = sizeof(cmd->cmd);
}

static int pgn_cmd_add_path(tec_pgn_cmd_t *cmd, tec_cfg_t *cfg)
{
    size_t len;
    char *name = cmd->name;
    const char *fmt = "%s/%s/%s";
    char *cmdptr = cmd->cmd + cmd->offset;

    len = snprintf(cmdptr, cmd->size, fmt, cfg->base.pgn, name, name);
    if (len >= cmd->size)
        return EXIT_FAILURE;

    cmd->offset = strlen(cmd->cmd);
    return EXIT_SUCCESS;
}

static int pgn_cmd_add_opts(tec_pgn_cmd_t *cmd, tec_cfg_t *cfg)
{
    size_t len;
    const char *fmt = " -T %s -P %s";
    char *cmdptr = cmd->cmd + cmd->offset;

    len = snprintf(cmdptr, cmd->size, fmt, cfg->base.task, cfg->base.pgn);
    if (len >= cmd->size)
        return EXIT_FAILURE;

    cmd->offset = strlen(cmd->cmd);
    return EXIT_SUCCESS;
}

static int pgn_cmd_add_args(tec_pgn_cmd_t *cmd, tec_argvec_t *argvec)
{
    size_t len;

    for (int i = 0; i < argvec->used; ++i) {
        char *cmdptr = cmd->cmd + cmd->offset;
        len = snprintf(cmdptr, cmd->size, " %s", argvec->argv[i]);
        if (len >= cmd->size) {
            return EXIT_FAILURE;
        }
        cmd->offset = strlen(cmd->cmd);
    }

    return EXIT_SUCCESS;
}

static int pgn_cmd_genpath(tec_pgn_cmd_t *cmd, tec_argvec_t *argvec,
                           tec_cfg_t *cfg)
{
    if (pgn_cmd_add_path(cmd, cfg))
        return TEC_LOG_E("pgn: base generation buffer overflow");
    else if (pgn_cmd_add_opts(cmd, cfg))
        return TEC_LOG_E("pgn: option generation buffer overflow");
    else if (pgn_cmd_add_args(cmd, argvec))
        return TEC_LOG_E("pgn: argument generation buffer overflow");
    return 0;
}

int tec_cli_pgn(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_pgn_cmd_t cmd;
    char *name = argvec->argv[0];

    pgn_cmd_init(&cmd, name);
    argvec_offset(argvec, 1);

    if (pgn_cmd_genpath(&cmd, argvec, cfg))
        return TEC_LOG_E("pgn: command generation failed '%s'", cmd.cmd);

    TEC_LOG_D("pgn: %s", cmd.cmd);
    return system(cmd.cmd) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}
