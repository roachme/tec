#include <stdio.h>
#include "pwd.h"
#include "log.h"
#include "config.h"

int tec_cli_pwd_set(tec_arg_t *args)
{
    FILE *fp;
    char *taskdir = teccfg.base.task;
    const char *fmt = "%s/%s/%s/%s\n";
    const char *fmtdebug = "pwd: env='%s', desk='%s', taskid='%s'";

    args->env = args->env == NULL ? "" : args->env;
    args->desk = args->desk == NULL ? "" : args->desk;
    args->taskid = args->taskid == NULL ? "" : args->taskid;

    if ((fp = fopen(PWDFILE, "w"))) {
        dlog(1, fmtdebug, args->env, args->desk, args->taskid);
        fprintf(fp, fmt, taskdir, args->env, args->desk, args->taskid);
        return fclose(fp);
    }
    return 1;
}

int tec_cli_pwd_unset()
{
    FILE *fp;

    if ((fp = fopen(PWDFILE, "w")) == NULL)
        return 1;
    return fclose(fp);
}
