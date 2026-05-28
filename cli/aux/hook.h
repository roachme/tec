#ifndef TEC_CLI_HOOK_H
#define TEC_CLI_HOOK_H

#include "../../lib/libtec.h"

// FIXME: get rid of magic numbers. Might cause buffer overflow
struct tec_hook {
    char cmd[10];
    char cmdopt[10];
    char pgntag[10];
    char pgname[10];
    char pgncmd[10];
    char pgnopt[10];
    struct tec_hook *next;
};

int hook_action(tec_arg_t * args, char *cmd);
int hook_cat(tec_unit_t ** units, tec_arg_t * args, char *cmd);
/* TODO: under development.  */
char *hook_list(struct tec_hook *hooks, char *pgnout, char *env, char *id);

#endif
