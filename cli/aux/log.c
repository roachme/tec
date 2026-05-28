#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "log.h"
#include "config.h"

int tec_cli_log_error(int level, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, "%s: ", PROGRAM);
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");
    va_end(arg);
    return EXIT_FAILURE;
}

int tec_cli_log_verbose(int status, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stdout, PROGRAM ": ");
    vfprintf(stdout, fmt, arg);
    fprintf(stdout, "\n");
    va_end(arg);
    return EXIT_SUCCESS;
}

int tec_cli_log_debug(int level, const char *fname, int line, const char *fmt,
                      ...)
{
    va_list arg;

    if (teccfg.opts.debug == false)
        return EXIT_SUCCESS;

    va_start(arg, fmt);
    fprintf(stderr, "%s[%s:%d]: ", PROGRAM, fname, line);
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");
    va_end(arg);
    return EXIT_SUCCESS;
}

int tec_cli_log_prompt(int level, const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);
    fprintf(stderr, PROGRAM ": ");
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    return EXIT_SUCCESS;
}
