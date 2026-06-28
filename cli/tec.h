#ifndef TEC_CLI_TEC_H
#define TEC_CLI_TEC_H

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>

#include "aux/pwd.h"
#include "aux/aux.h"
#include "aux/log.h"
#include "aux/hook.h"
#include "aux/valid.h"
#include "aux/color.h"
#include "aux/argvec.h"
#include "aux/config.h"
#include "../lib/libtec.h"

#define IDSIZ               8
#define DESKSIZ             10
#define ENVSIZ              10
#define CMDSIZ              10

#define xstr(s)             str(s)
#define str(s)              #s

#define IDLIMIT             9999999
#define IDFMT               "%0" xstr(IDSIZ) "u"
#define PADDING_UNIT        6

#define FMT_OPT_ARG_INV "invalid option '-%c'"
#define FMT_OPT_ARG_REQ "option '-%c' requires an argument"

#define LIST_OBJ_UNITS(_obj, _pgnout, _desc, padsiz, enabled) do {\
    color_print_str("%-" xstr(padsiz) "s ", (_obj), BBLU, enabled); \
    color_print_str("%s ", (_pgnout), WHT, enabled); \
    color_print_str("%s\n", (_desc), WHT, enabled); \
} while (0)

#define CTX_INIT { .units = NULL, .list = NULL }
#define ARGS_INIT(args) { .env = NULL, .desk = NULL, .task = NULL }

enum tec_setup_level {
    TEC_PAGER,
    TEC_SETUP_SOFT,
    TEC_SETUP_HARD,
};

typedef struct tec_cmd {
    const char *name;
    union {
        struct tec_cmd *(*type) (tec_argvec_t * argvec, tec_cfg_t * cfg);
        int (*func)(tec_argvec_t * argvec, tec_cfg_t * cfg);
    };
    unsigned int option;
} tec_cmd_t;

tec_cmd_t *tec_cli_is_alias(tec_argvec_t * argvec, tec_cfg_t * cfg);
tec_cmd_t *tec_cli_is_plugin(tec_argvec_t * argvec, tec_cfg_t * cfg);
tec_cmd_t *tec_cli_is_builtin(tec_argvec_t * argvec, tec_cfg_t * cfg);

int tec_cli_add(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_alias(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_cat(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_cd(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_cfg(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_env(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_desk(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_help(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_init(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_ls(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_mv(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_pgn(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_rm(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_set(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_version(tec_argvec_t * argvec, tec_cfg_t * cfg);
int tec_cli_cmd_run(tec_cmd_t * cmd, tec_argvec_t * argvec, tec_cfg_t * cfg);

#endif
