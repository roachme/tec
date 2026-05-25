#ifndef TEC_PATH_H
#define TEC_PATH_H

#include "libtec.h"

char *path_env_dir(const char *taskdir, const tec_arg_t * args);
char *path_desk_dir(const char *taskdir, const tec_arg_t * args);
char *path_task_dir(const char *taskdir, const tec_arg_t * args);

char *path_env_unit(const char *taskdir, const tec_arg_t * args);
char *path_desk_unit(const char *taskdir, const tec_arg_t * args);
char *path_task_unit(const char *taskdir, const tec_arg_t * args);

char *path_task_db(const char *taskdir, const tec_arg_t * args);
char *path_desk_db(const char *taskdir, const tec_arg_t * args);
char *path_env_db(const char *taskdir, const tec_arg_t * args);

#endif
