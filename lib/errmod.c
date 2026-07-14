#include <string.h>

#include "libtec.h"

static int errcode = TEC_OK;

const char *errcodes[__TEC_STATUS_LAST] = {
    [TEC_OK] = "OK",

    [TEC_SYS_DB] = "database directory not found",
    [TEC_SYS_MALLOC] = "cannot malloc memory",

    [TEC_ARG_ILLEG] = "illegal object name",
    [TEC_ARG_NOSUCH] = "no such object",
    [TEC_ARG_EXISTS] = "object already exists",

    [TEC_DIR_RM] = "cannot remove directory",
    [TEC_DIR_MAKE] = "cannot create directory",
    [TEC_DIR_MOVE] = "cannot rename directory",
    [TEC_DIR_OPEN] = "cannot open directory",

    [TEC_UNIT_ADD] = "cannot add unit node",
    [TEC_UNIT_RM] = "cannot remove unit node",
    [TEC_UNIT_GET] = "cannot get unit values",
    [TEC_UNIT_ILLEG] = "illegal unit value",
    [TEC_UNIT_KEY] = "unit key does not exist",
    [TEC_UNIT_LOAD] = "cannot load units",
    [TEC_UNIT_SAVE] = "cannot save unit values",
    [TEC_UNIT_SET] = "cannot set unit values",

    /* HOTFIX: remove it once error code table is filled.  */
    /* Not yet defined error messages.  */
    [TEC_NODEF_ERR] = "not yet defined error messages",
};

int emod_set(int err)
{
    errcode = err;
    if (errcode < 0 || errcode > __TEC_STATUS_LAST)
        errcode = -1;
    return errcode;
}

char *emod_strerror(int errnum)
{
    static char errmsg[TEC_ERRMSGSIZ + 1];

    if (errnum < 0 || errnum >= __TEC_STATUS_LAST)
        return strncpy(errmsg, "internal unknown error", TEC_ERRMSGSIZ);
    return strncpy(errmsg, errcodes[errnum], TEC_ERRMSGSIZ);
}
