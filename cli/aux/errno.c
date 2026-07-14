#include <string.h>
#include "errno.h"

static const char *errcodes[__TEC_CLI_STATUS_LAST] = {
    [TEC_ALIAS] = "alias '-' is used alone",
    [TEC_HOOK] = "failed to execute hooks",

    [TEC_TOGG_UPDATE] = "cannot update toggles",
    [TEC_TOGG_GET_CURR] = "no current task ID",
    [TEC_TOGG_GET_PREV] = "no previous task ID",

    [TEC_ENV_NOCURR] = "no current environment",
    [TEC_ENV_NOPREV] = "no previous environment",
};

char *tec_strerror(int errnum)
{
    static char errmsg[TEC_ERRMSGSIZ + 1];

    if (errnum >= 0 && errnum < __TEC_STATUS_LAST)
        return tec_geterr(errnum);
    else if (errnum < __TEC_CLI_STATUS_LAST)
        return strncpy(errmsg, errcodes[errnum], TEC_ERRMSGSIZ);
    return strncpy(errmsg, "cli internal unknown error", TEC_CLI_ERRMSGSIZ);
}
