#include <string.h>

#include "libtec.h"

static int errcode = TEC_OK;

const char *errcodes[__TEC_STATUS_LAST] = {
    [TEC_OK] = "OK",

    [TEC_SYS_DB] = "database directory not found",
    [TEC_SYS_MALLOC] = "could not malloc memory",

    [TEC_ARG_ILLEG] = "illegal object name",
    [TEC_ARG_NOSUCH] = "no such object",
    [TEC_ARG_EXISTS] = "object already exists",

    [TEC_DIR_RM] = "could not remove directory",
    [TEC_DIR_MAKE] = "could not create directory",
    [TEC_DIR_MOVE] = "could not rename directory",
    [TEC_DIR_OPEN] = "could not open directory",

    [TEC_UNIT_ADD] = "could not add unit node",
    [TEC_UNIT_RM] = "could not remove unit node",
    [TEC_UNIT_GET] = "could not get unit values",
    [TEC_UNIT_ILLEG] = "illegal unit value",
    [TEC_UNIT_KEY] = "unit key does not exist",
    [TEC_UNIT_LOAD] = "could not load units",
    [TEC_UNIT_SAVE] = "could not save unit values",
    [TEC_UNIT_SET] = "could not set unit values",

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
