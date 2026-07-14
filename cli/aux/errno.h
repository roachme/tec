#ifndef TEC_CLI_ERRNO_H
#define TEC_CLI_ERRNO_H

#include "../../lib/libtec.h"

#define TEC_CLI_ERRMSGSIZ   100

enum tec_cli_errno {
    TEC_ALIAS = __TEC_STATUS_LAST,
    TEC_HOOK,

    TEC_TOGG_UPDATE,
    TEC_TOGG_GET_CURR,
    TEC_TOGG_GET_PREV,

    TEC_ENV_NOCURR,
    TEC_ENV_NOPREV,

    TEC_DESK_NOCURR,
    TEC_DESK_NOPREV,

    TEC_TASK_NOCURR,
    TEC_TASK_NOPREV,

    __TEC_CLI_STATUS_LAST
};

char *tec_strerror(int errnum);

#endif
