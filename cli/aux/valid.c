#include <string.h>
#include <stdbool.h>
#include "valid.h"
#include "toggle.h"
#include "config.h"

int tec_cli_len_valid(char *obj, int len)
{
    if (strlen(obj) <= len)
        return true;
    return false;
}

int tec_cli_check_env(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_env_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current env");
        return status;
    } else if ((status = tec_env_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->env, tec_strerror(status));
        return status;
    } else if (tec_cli_len_valid(args->env, ENVSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->env, "env name is too long");
        return status;
    } else if ((status = tec_env_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->env, tec_strerror(status));
        return status;
    }
    return 0;
}

int tec_cli_check_desk(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_desk_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current desk");
        return status;
    } else if ((status = tec_desk_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->desk, tec_strerror(status));
        return status;
    } else if (tec_cli_len_valid(args->desk, DESKSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->desk, "desk name is too long");
        return status;
    } else if ((status = tec_desk_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->desk, tec_strerror(status));
        return status;
    }
    return 0;
}

int tec_cli_check_task(tec_arg_t *args, const char *errfmt, int quiet)
{
    int status;

    if ((status = toggle_task_get_curr(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, "NOCURR", "no current task");
        return status;
    } else if ((status = tec_task_valid(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->task, tec_strerror(status));
        return status;
    } else if (tec_cli_len_valid(args->task, IDSIZ) == false) {
        status = 1;
        if (quiet == false)
            elog(status, errfmt, args->task, "task ID is too long");
        return status;
    } else if ((status = tec_task_exist(teccfg.base.task, args))) {
        if (quiet == false)
            elog(status, errfmt, args->task, tec_strerror(status));
        return status;
    }
    return 0;
}
