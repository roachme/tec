#include <stdio.h>
#include <limits.h>

#include "dir.h"
#include "osdep.h"

static int _task_dbmkdir(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    const char *fmt = "%s/%s/%s/%s/.tec";
    sprintf(path, fmt, taskdir, args->env, args->desk, args->task);
    return MKDIR(path);
}

static int _desk_dbmkdir(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/%s/.tec", taskdir, args->env, args->desk);
    return MKDIR(path);
}

static int _env_dbmkdir(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/.tec", taskdir, args->env);
    return MKDIR(path);
}

int dir_task_add(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/%s/%s", taskdir, args->env, args->desk, args->task);
    return !(MKDIR(path) == 0 && _task_dbmkdir(taskdir, args) == 0);
}

int dir_task_del(const char *taskdir, tec_arg_t *args)
{
    char path[BUFSIZ + 1];
    sprintf(path, "%s/%s/%s/%s", taskdir, args->env, args->desk, args->task);
    return RMDIR(path);
}

int dir_task_move(const char *taskdir, tec_arg_t *src, tec_arg_t *dst)
{
    char new_path[PATH_MAX + 1];
    char old_path[PATH_MAX + 1];
    sprintf(old_path, "%s/%s/%s/%s", taskdir, src->env, src->desk, src->task);
    sprintf(new_path, "%s/%s/%s/%s", taskdir, dst->env, dst->desk, dst->task);
    return rename(old_path, new_path);
}

int dir_desk_add(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/%s", taskdir, args->env, args->desk);
    return !(MKDIR(path) == 0 && _desk_dbmkdir(taskdir, args) == 0);
}

int dir_desk_del(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/%s", taskdir, args->env, args->desk);
    return RMDIR(path);
}

int dir_env_add(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s/", taskdir, args->env);
    return !(MKDIR(path) == 0 && _env_dbmkdir(taskdir, args) == 0);
}

int dir_env_del(const char *taskdir, tec_arg_t *args)
{
    char path[PATH_MAX + 1];
    sprintf(path, "%s/%s", taskdir, args->env);
    return RMDIR(path);
}

int dir_env_rename(const char *taskdir, tec_arg_t *src, tec_arg_t *dst)
{
    char srcpath[PATH_MAX + 1];
    char dstpath[PATH_MAX + 1];

    sprintf(srcpath, "%s/%s", taskdir, src->env);
    sprintf(dstpath, "%s/%s", taskdir, dst->env);
    return MOVE(srcpath, dstpath);
}
