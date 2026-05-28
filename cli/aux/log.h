#ifndef TEC_CLI_LOG_H
#define TEC_CLI_LOG_H

#include <stdio.h>
#include "aux.h"

typedef enum tec_cli_log_lvl {
    TEC_LOG_ERROR,              /* Error, no recovery, terminate immediately */
    TEC_LOG_DEBUG,              /* Debug info */
    TEC_LOG_VERBOSE,            /* Verbose info for user */
    TEC_LOG_PROMPT,             /* Prompt user for input */
} tec_cli_log_lvl_t;

#define TEC_LOG_E(fmt, ...) tec_cli_log_error(TEC_LOG_ERROR, fmt, ##__VA_ARGS__)
#define TEC_LOG_V(fmt, ...) tec_cli_log_verbose(TEC_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#define TEC_LOG_D(fmt, ...) tec_cli_log_debug(TEC_LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define TEC_LOG_P(fmt, ...) tec_cli_log_prompt(TEC_LOG_PROMPT, fmt, ##__VA_ARGS__)

int tec_cli_log_error(int level, const char *fmt, ...);
int tec_cli_log_debug(int level, const char *fname, int line, const char *fmt,
                      ...);
int tec_cli_log_verbose(int level, const char *fmt, ...);
int tec_cli_log_prompt(int level, const char *fmt, ...);

#endif
