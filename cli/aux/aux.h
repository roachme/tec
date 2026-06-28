#ifndef TEC_CLI_AUX_H
#define TEC_CLI_AUX_H

#include <stdbool.h>
#include "../../lib/libtec.h"

#define PROGRAM     "tec"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define xstr(s) str(s)
#define str(s) #s

// TODO: Get rid of it (set options with default boolean values)
#define NONEBOOL        -1      /* Not yet set boolean value */

extern char *unitkeys[];
extern unsigned int nunitkey;

bool yesno(void);
bool do_change_user_cwd(tec_arg_t * args);

int tec_cli_help_list(void);
int tec_cli_help_usage(const char *cmd);

#endif
