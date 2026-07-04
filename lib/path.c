#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

#include "path.h"

#define ENVFMT_DIR          "%s/%s"
#define DESKFMT_DIR         ENVFMT_DIR "/" "%s"
#define TASKFMT_DIR         DESKFMT_DIR "/" "%s"

#define ENVFMT_DB           ENVFMT_DIR "/" ".tec"
#define DESKFMT_DB          DESKFMT_DIR "/" ".tec"
#define TASKFMT_DB          TASKFMT_DIR "/" ".tec"

#define ENVFMT_UNIT         ENVFMT_DB "/" "units"
#define DESKFMT_UNIT        DESKFMT_DB "/" "units"
#define TASKFMT_UNIT        TASKFMT_DB "/" "units"

static char *path_generic(char *buf, int bufsiz, const char *fmt, ...)
{
    int len;
    va_list arg;

    va_start(arg, fmt);
    len = vsnprintf(buf, bufsiz, fmt, arg);
    va_end(arg);
    return len >= bufsiz ? NULL : buf;
}

char *path_env_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = ENVFMT_DIR;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env);
}

char *path_env_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = ENVFMT_DB;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env);
}

char *path_env_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = ENVFMT_UNIT;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env);
}

char *path_desk_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = DESKFMT_DIR;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env,
                        args->desk);
}

char *path_desk_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = DESKFMT_DB;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env,
                        args->desk);
}

char *path_desk_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = DESKFMT_UNIT;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env,
                        args->desk);
}

char *path_task_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = TASKFMT_DIR;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env, args->desk,
                        args->task);
}

char *path_task_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = TASKFMT_DB;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env, args->desk,
                        args->task);
}

char *path_task_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = TASKFMT_UNIT;
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, PATH_MAX, fmt, taskdir, args->env, args->desk,
                        args->task);
}
