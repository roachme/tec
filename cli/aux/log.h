#ifndef TEC_CLI_LOG_H
#define TEC_CLI_LOG_H

#include <stdio.h>
#include "aux.h"

typedef enum tec_cli_log_lvl {
    TEC_LOG_PROMPT,             /* Prompt user for input */
    TEC_LOG_ERROR,              /* Error, no recovery, terminate immediately */
    TEC_LOG_INFO,               /* Info for user */
    TEC_LOG_DEBUG,              /* Debug info */
} tec_cli_log_lvl_t;

#define TEC_LOG_E(fmt, ...) tec_cli_log_error(fmt, ##__VA_ARGS__)
#define TEC_LOG_I(fmt, ...) tec_cli_log_info(fmt, ##__VA_ARGS__)
#define TEC_LOG_D(fmt, ...) tec_cli_log_debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define TEC_LOG_P(fmt, ...) tec_cli_log_prompt(fmt, ##__VA_ARGS__)

int tec_cli_log_error(const char *fmt, ...);
int tec_cli_log_debug(const char *fname, int line, const char *fmt, ...);
int tec_cli_log_info(const char *fmt, ...);
int tec_cli_log_prompt(const char *fmt, ...);

#endif
