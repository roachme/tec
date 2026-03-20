#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "log.h"
#include "config.h"

int elog(int status, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    fprintf(stderr, PROGRAM ": ");
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");
    va_end(arg);
    return status;
}

int dlog(int level, const char *fmt, ...)
{
    if (teccfg.opts.debug == false)
        return 0;

    va_list arg;
    va_start(arg, fmt);
    printf(PROGRAM ": ");
    vprintf(fmt, arg);
    printf("\n");
    va_end(arg);
    return 0;
}

int llog(int status, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    printf(PROGRAM ": ");
    vprintf(fmt, arg);
    printf("\n");
    va_end(arg);
    return 0;
}
