#ifndef TEC_CLI_TOGGLE_H
#define TEC_CLI_TOGGLE_H

#include "../tec.h"

int toggle_env_get_curr(char *base, tec_arg_t * args);
int toggle_env_get_prev(char *base, tec_arg_t * args);
bool toggle_env_is_curr(char *base, tec_arg_t * args);
bool toggle_env_is_prev(char *base, tec_arg_t * args);
int toggle_env_set_curr(char *base, tec_arg_t * args);

int toggle_desk_get_curr(char *base, tec_arg_t * args);
int toggle_desk_get_prev(char *base, tec_arg_t * args);
int toggle_desk_set_curr(char *base, tec_arg_t * args);

int toggle_task_get_curr(char *base, tec_arg_t * args);
int toggle_task_get_prev(char *base, tec_arg_t * args);
int toggle_task_set_curr(char *base, tec_arg_t * args);
bool toggle_task_is_curr(char *base, tec_arg_t * args);
bool toggle_task_is_prev(char *base, tec_arg_t * args);
int toggle_task_unset_curr(char *base, tec_arg_t * args);
int toggle_task_unset_prev(char *base, tec_arg_t * args);

/* Update toggles after rename/move operations */
int toggle_task_update(char *base, tec_arg_t * args,
                       const char *old_id, const char *new_id);
int toggle_task_clear(char *base, tec_arg_t * args, const char *task);

int toggle_env_update(char *base, tec_arg_t * args, const char *src,
                      const char *dst);

#endif
