#ifndef LIBTEC_TEC_H
#define LIBTEC_TEC_H

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>

#include "aux/aux.h"
#include "aux/log.h"
#include "aux/hook.h"
#include "aux/color.h"
#include "aux/argvec.h"
#include "../lib/libtec.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define xstr(s) str(s)
#define str(s) #s

#define IDSIZ               8
#define DESKSIZ             10
#define ENVSIZ              10
#define CMDSIZ              10

#define xstr(s)             str(s)
#define str(s)              #s

#define IDLIMIT             9999999
#define IDFMT               "%0" xstr(IDSIZ) "u"
#define PADDING_UNIT        6

#define LIST_OBJ_UNITS(_obj, _pgnout, _desc) do {\
    color_print_str("%-" xstr(IDSIZ) "s ", (_obj), BBLU); \
    color_print_str("%s ", (_pgnout), WHT); \
    color_print_str("%s\n", (_desc), WHT); \
} while (0)

#define CTX_INIT { .units = NULL, .list = NULL }

enum tec_setup_level {
    TEC_PAGER,
    TEC_SETUP_SOFT,
    TEC_SETUP_HARD,
};

typedef struct builtin {
    const char *name;
    int (*func)(tec_argvec_t * argvec, tec_ctx_t * ctx);
    unsigned int option;
} tec_builtin_t;

extern char *unitkeys[];
extern unsigned int nunitkey;

int is_valid_length(const char *obj, int len);
int check_arg_env(tec_arg_t * args, const char *errfmt, int quiet);
int check_arg_desk(tec_arg_t * args, const char *errfmt, int quiet);
int check_arg_task(tec_arg_t * args, const char *errfmt, int quiet);

int help_list_pretty_commands(void);
int help_usage(const char *cmd);
int help_lookup(const char *cmd);

bool tec_cli_get_user_choice(void);

int tec_pwd_unset(void);
int tec_pwd_task(tec_arg_t * args);
int tec_pwd_desk(tec_arg_t * args);
int tec_pwd_env(tec_arg_t * args);

// NOTE: maybe use 'prefix' like in git?
// int cmd_add(int argc, const char **argv, const char *prefix, struct repository *repo);
int tec_cli_add(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_cat(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_cd(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_cfg(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_env(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_desk(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_help(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_init(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_ls(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_mv(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_pgn(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_rm(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_set(tec_argvec_t * argvec, tec_ctx_t * ctx);
int tec_cli_version(tec_argvec_t * argvec, tec_ctx_t * ctx);

#endif
