#include <limits.h>
#include <string.h>

#include "dir.h"
#include "osdep.h"
#include "path.h"

static int _task_mkdb(const char *taskdir, tec_arg_t *args)
{
    char *path = path_task_db(taskdir, args);
    return MKDIR(path);
}

static int _desk_mkdb(const char *taskdir, tec_arg_t *args)
{
    char *path = path_desk_db(taskdir, args);
    return MKDIR(path);
}

static int _env_mkdb(const char *taskdir, tec_arg_t *args)
{
    char *path = path_env_db(taskdir, args);
    return MKDIR(path);
}

int dir_task_add(const char *taskdir, tec_arg_t *args)
{
    char *path = path_task_dir(taskdir, args);
    return !(MKDIR(path) == 0 && _task_mkdb(taskdir, args) == 0);
}

int dir_task_rm(const char *taskdir, tec_arg_t *args)
{
    char *path = path_task_dir(taskdir, args);
    return RMDIR(path);
}

int dir_task_move(const char *taskdir, tec_arg_t *src, tec_arg_t *dst)
{
    char srcpath[PATH_MAX + 1];
    char dstpath[PATH_MAX + 1];

    strcpy(srcpath, path_task_dir(taskdir, src));
    strcpy(dstpath, path_task_dir(taskdir, dst));
    return MOVE(srcpath, dstpath);
}

int dir_desk_add(const char *taskdir, tec_arg_t *args)
{
    char *path = path_desk_dir(taskdir, args);
    return !(MKDIR(path) == 0 && _desk_mkdb(taskdir, args) == 0);
}

int dir_desk_rm(const char *taskdir, tec_arg_t *args)
{
    char *path = path_desk_dir(taskdir, args);
    return RMDIR(path);
}

int dir_desk_move(const char *taskdir, tec_arg_t *src, tec_arg_t *dst)
{
    char srcpath[PATH_MAX + 1];
    char dstpath[PATH_MAX + 1];

    strcpy(srcpath, path_desk_dir(taskdir, src));
    strcpy(dstpath, path_desk_dir(taskdir, dst));
    return MOVE(srcpath, dstpath);
}

int dir_env_add(const char *taskdir, tec_arg_t *args)
{
    char *path = path_env_dir(taskdir, args);
    return !(MKDIR(path) == 0 && _env_mkdb(taskdir, args) == 0);
}

int dir_env_rm(const char *taskdir, tec_arg_t *args)
{
    char *path = path_env_dir(taskdir, args);
    return RMDIR(path);
}

int dir_env_rename(const char *taskdir, tec_arg_t *src, tec_arg_t *dst)
{
    char srcpath[PATH_MAX + 1];
    char dstpath[PATH_MAX + 1];

    strcpy(srcpath, path_env_dir(taskdir, src));
    strcpy(dstpath, path_env_dir(taskdir, dst));
    return MOVE(srcpath, dstpath);
}
