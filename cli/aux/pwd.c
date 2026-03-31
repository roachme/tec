#include <stdio.h>
#include "pwd.h"
#include "log.h"
#include "config.h"

int tec_pwd_env(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s\n";
        const char *debug_fmt = "PWD env: '%s'";

        dlog(1, debug_fmt, args->env);
        fprintf(fp, fmt, taskdir, args->env);
        return fclose(fp);
    }
    return 1;
}

int tec_pwd_desk(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s/%s\n";
        const char *debug_fmt = "PWD env: '%s', desk: '%s'";

        dlog(1, debug_fmt, args->env, args->desk);
        fprintf(fp, fmt, taskdir, args->env, args->desk);
        return fclose(fp);
    }
    return 1;
}

int tec_pwd_task(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;

    if ((fp = fopen(PWDFILE, "w"))) {
        const char *fmt = "%s/%s/%s/%s\n";
        const char *debug_fmt = "PWD env: '%s', desk: '%s', task ID: '%s'";

        dlog(1, debug_fmt, args->env, args->desk, args->taskid);
        fprintf(fp, fmt, taskdir, args->env, args->desk, args->taskid);
        return fclose(fp);
    }
    return 1;
}

int tec_pwd_unset(void)
{
    FILE *fp;

    if ((fp = fopen(PWDFILE, "w")) == NULL)
        return 1;
    return fclose(fp);
}
