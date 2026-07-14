#include <string.h>
#include "errno.h"

#define ESHIFT  E__TEC_STATUS_LAST

static const char *errcodes[__TEC_CLI_STATUS_LAST] = {
    [ETEC_ALIAS - ESHIFT] = "alias '-' is used alone",
    [ETEC_HOOK_EXEC - ESHIFT] = "hook execution failed",

    [ETEC_TOGG_UPDATE - ESHIFT] = "cannot update toggles",

    [ETEC_ARG_ENV_REQ - ESHIFT] = "environment name is required",
    [ETEC_ARG_DESK_REQ - ESHIFT] = "desk name is required",
    [ETEC_ARG_ENV_TOO_LONG - ESHIFT] = "environment name is too long",
    [ETEC_ARG_DESK_TOO_LONG - ESHIFT] = "desk name is too long",
    [ETEC_ARG_TASK_TOO_LONG - ESHIFT] = "task ID is too long",

    [ETEC_UNIT_INV_KEY - ESHIFT] = "invalid unit key",
    [ETEC_UNIT_GEN_FAIL - ESHIFT] = "unit generation failed",

    [ETEC_TOGG_ENV_GET_CURR - ESHIFT] = "no current environment",
    [ETEC_TOGG_ENV_GET_PREV - ESHIFT] = "no previous environment",
    [ETEC_TOGG_ENV_SET_CURR - ESHIFT] = "cannot set current environment",

    [ETEC_TOGG_DESK_GET_CURR - ESHIFT] = "no current desk",
    [ETEC_TOGG_DESK_GET_PREV - ESHIFT] = "no previous desk",
    [ETEC_TOGG_DESK_SET_CURR - ESHIFT] = "cannot set current desk",

    [ETEC_TOGG_TASK_GET_CURR - ESHIFT] = "no current task ID",
    [ETEC_TOGG_TASK_GET_PREV - ESHIFT] = "no previous task ID",
    [ETEC_TOGG_TASK_SET_CURR - ESHIFT] = "cannot set current task ID",
    [ETEC_TOGG_TASK_UNSET - ESHIFT] = "cannot unset task toggle",
    [ETEC_TOGG_TASK_UNSET_PREV - ESHIFT] = "cannot unset previous task",

    //* argvec
    //* aux
    //* color
    //* config
    //* hook
    //* log
    //* opts
    //* osdep
    //* pwd
    //* togg
};

char *tec_strerror(int errnum)
{
    static char errmsg[TEC_CLI_EBUF + 1];
    int cli_idx = errnum - E__TEC_STATUS_LAST;

    /* Get LIB tec error code message */
    if (errnum >= 0 && errnum < E__TEC_STATUS_LAST)
        return tec_geterr(errnum);

    /* Get CLI tec error code message */
    if (cli_idx >= 0 && cli_idx < __TEC_CLI_STATUS_LAST)
        return strncpy(errmsg, errcodes[cli_idx], TEC_CLI_EBUF);

    return strncpy(errmsg, "cli internal unknown error", TEC_CLI_EBUF);
}
