#ifndef TEC_CLI_VALID_H
#define TEC_CLI_VALID_H

#include <stdio.h>
#include "../../lib/libtec.h"

int tec_cli_len_valid(const char *obj, size_t len);
int tec_cli_check_env(tec_arg_t * args);
int tec_cli_check_desk(tec_arg_t * args);
int tec_cli_check_task(tec_arg_t * args);

#endif
