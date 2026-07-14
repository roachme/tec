#include <string.h>

#include "libtec.h"
#include "errmod.h"

static int errcode = ETEC_OK;

const char *errcodes[E__TEC_STATUS_LAST] = {
    [ETEC_OK] = "OK",

    [ETEC_SYS_DB] = "database directory not found",
    [ETEC_SYS_MALLOC] = "cannot malloc memory",

    [ETEC_ARG_TASK_EXIST] = "task ID already exists",
    [ETEC_ARG_TASK_ILLEG] = "illegal task ID",
    [ETEC_ARG_TASK_NOSUCH] = "no such task ID",

    [ETEC_ARG_DESK_EXIST] = "desk already exists",
    [ETEC_ARG_DESK_ILLEG] = "illegal desk name",
    [ETEC_ARG_DESK_NOSUCH] = "no such desk",

    [ETEC_ARG_ENV_EXIST] = "environment already exists",
    [ETEC_ARG_ENV_ILLEG] = "illegal environment name",
    [ETEC_ARG_ENV_NOSUCH] = "no such environment",

    [ETEC_DIR_RM] = "cannot remove directory",
    [ETEC_DIR_MAKE] = "cannot create directory",
    [ETEC_DIR_MOVE] = "cannot rename directory",
    [ETEC_DIR_OPEN] = "cannot open directory",

    [ETEC_UNIT_ADD] = "cannot add unit node",
    [ETEC_UNIT_RM] = "cannot remove unit node",
    [ETEC_UNIT_GET] = "cannot get unit values",
    [ETEC_UNIT_ILLEG] = "illegal unit value",
    [ETEC_UNIT_KEY] = "unit key does not exist",
    [ETEC_UNIT_LOAD] = "cannot load units",
    [ETEC_UNIT_SAVE] = "cannot save unit values",
    [ETEC_UNIT_SET] = "cannot set unit values",
};

int emod_set(int errnum)
{
    errcode = errnum;
    if (errcode < 0 || errcode > E__TEC_STATUS_LAST)
        errcode = -1;
    return errcode;
}

char *emod_geterr(int errnum)
{
    static char errmsg[ERRMOD_MSGSIZ + 1];

    if (errnum < 0 || errnum >= E__TEC_STATUS_LAST)
        return strncpy(errmsg, "internal unknown error", ERRMOD_MSGSIZ);
    return strncpy(errmsg, errcodes[errnum], ERRMOD_MSGSIZ);
}
