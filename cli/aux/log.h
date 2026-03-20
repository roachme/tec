#ifndef TEC_LOG_H
#define TEC_LOG_H

#include "aux.h"

int elog(int status, const char *fmt, ...);
int dlog(int level, const char *fmt, ...);
int llog(int status, const char *fmt, ...);

#endif
