#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

#include "path.h"

char *path_generic(char *buf, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);
    return buf;
}

char *path_env_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env);
}

char *path_env_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/.tec";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env);
}

char *path_desk_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk);
}

char *path_desk_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/.tec";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk);
}

char *path_task_dir(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/%s";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk,
                        args->task);
}

char *path_task_db(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/%s/.tec";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk,
                        args->task);
}

/* Generate path for unit files.  */
char *path_env_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/.tec/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env);
}

char *path_desk_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/.tec/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk);
}

char *path_task_unit(const char *taskdir, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/%s/.tec/units";
    static char pathname[PATH_MAX + 1];
    return path_generic(pathname, fmt, taskdir, args->env, args->desk,
                        args->task);
}
