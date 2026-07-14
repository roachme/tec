#include <string.h>
#include <stdbool.h>
#include "valid.h"
#include "errno.h"
#include "toggle.h"
#include "config.h"

int tec_cli_len_valid(const char *obj, size_t len)
{
    if (strlen(obj) <= len)
        return true;
    return false;
}

int tec_cli_check_env(tec_arg_t *args)
{
    int status = ETEC_OK;

    if ((status = toggle_env_get_curr(teccfg.base.task, args)))
        return status;
    else if ((status = tec_env_valid(teccfg.base.task, args)))
        return status;
    else if (tec_cli_len_valid(args->env, ENVSIZ) == false)
        return ETEC_ARG_ENV_TOO_LONG;
    else if ((status = tec_env_exist(teccfg.base.task, args)))
        return status;
    return status;
}

int tec_cli_check_desk(tec_arg_t *args)
{
    int status = ETEC_OK;

    if ((status = toggle_desk_get_curr(teccfg.base.task, args)))
        return status;
    else if ((status = tec_desk_valid(teccfg.base.task, args)))
        return status;
    else if (tec_cli_len_valid(args->desk, DESKSIZ) == false)
        return ETEC_ARG_DESK_TOO_LONG;
    else if ((status = tec_desk_exist(teccfg.base.task, args)))
        return status;
    return status;
}

int tec_cli_check_task(tec_arg_t *args)
{
    int status = ETEC_OK;

    if ((status = toggle_task_get_curr(teccfg.base.task, args)))
        return status;
    else if ((status = tec_task_valid(teccfg.base.task, args)))
        return status;
    else if (tec_cli_len_valid(args->task, IDSIZ) == false)
        return ETEC_ARG_TASK_TOO_LONG;
    else if ((status = tec_task_exist(teccfg.base.task, args)))
        return status;
    return status;
}
