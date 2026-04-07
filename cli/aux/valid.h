#ifndef TEC_CLI_VALID_H
#define TEC_CLI_VALID_H

#include "../../lib/libtec.h"

int tec_cli_len_valid(char *obj, int len);
int tec_cli_check_env(tec_arg_t * args, const char *errfmt, int quiet);
int tec_cli_check_desk(tec_arg_t * args, const char *errfmt, int quiet);
int tec_cli_check_task(tec_arg_t * args, const char *errfmt, int quiet);

#endif
