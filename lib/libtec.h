#ifndef TEC_H
#define TEC_H

typedef struct tec_arg {
    char *task;
    char *desk;
    char *env;
} tec_arg_t;

typedef struct tec_unit {
    char *key;
    char *val;
    struct tec_unit *next;
} tec_unit_t;

typedef struct tec_list {
    char *name;
    int status;                 /* Status code of the object */
    struct tec_list *next;
} tec_list_t;

typedef struct tec_context {
    struct tec_unit *units;
    struct tec_list *list;
} tec_ctx_t;

enum tec_errno {
    ETEC_OK,

    ETEC_SYS_DB,
    ETEC_SYS_MALLOC,

    ETEC_ARG_TASK_ILLEG,
    ETEC_ARG_DESK_ILLEG,
    ETEC_ARG_ENV_ILLEG,

    ETEC_ARG_TASK_NOSUCH,
    ETEC_ARG_DESK_NOSUCH,
    ETEC_ARG_ENV_NOSUCH,

    ETEC_ARG_TASK_EXIST,
    ETEC_ARG_DESK_EXIST,
    ETEC_ARG_ENV_EXIST,

    ETEC_DIR_RM,
    ETEC_DIR_MAKE,
    ETEC_DIR_MOVE,
    ETEC_DIR_OPEN,
    ETEC_DIR_RENAME,

    ETEC_UNIT_ADD,
    ETEC_UNIT_RM,
    ETEC_UNIT_GET,
    ETEC_UNIT_ILLEG,
    ETEC_UNIT_KEY,
    ETEC_UNIT_LOAD,
    ETEC_UNIT_SAVE,
    ETEC_UNIT_SET,

    E__TEC_STATUS_LAST
};

/* Core functions.  */
int tec_make_db(const char *taskdir);
int tec_check_db(const char *taskdir);
char *tec_geterr(int errnum);

/* Data structure for unit values.  */
tec_unit_t *tec_unit_add(tec_unit_t * head, char *key, char *val);
tec_unit_t *tec_unit_join(tec_unit_t * head, tec_unit_t * body);
tec_unit_t *tec_unit_parse(struct tec_unit *head, const char *str);
char *tec_unit_get(tec_unit_t * head, char *key);
void *tec_unit_free(tec_unit_t * units);
int tec_unit_set(struct tec_unit *head, char *key, char *val);
int tec_unit_save(const char *filename, tec_unit_t * units);

/* Data structure for list of objects.  */
void *tec_list_free(tec_list_t * list);

/* Task functions.  */
int tec_task_add(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_task_exist(const char *taskdir, tec_arg_t * args);
int tec_task_get(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_task_valid(const char *taskdir, tec_arg_t * args);
int tec_task_list(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_task_move(const char *taskdir, tec_arg_t * src, tec_arg_t * dst,
                  tec_ctx_t * ctx);
int tec_task_rm(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_task_set(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);

/* Desk functions.  */
int tec_desk_add(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_desk_exist(const char *taskdir, tec_arg_t * args);
int tec_desk_get(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_desk_valid(const char *taskdir, tec_arg_t * args);
int tec_desk_list(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_desk_move(const char *taskdir, tec_arg_t * src, tec_arg_t * dst,
                  tec_ctx_t * ctx);
int tec_desk_rm(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_desk_set(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);

/* Environment functions.  */
int tec_env_add(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_env_exist(const char *taskdir, tec_arg_t * args);
int tec_env_valid(const char *taskdir, tec_arg_t * args);
int tec_env_get(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_env_list(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_env_rename(const char *taskdir, tec_arg_t * src, tec_arg_t * dst,
                   tec_ctx_t * ctx);
int tec_env_rm(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);
int tec_env_set(const char *taskdir, tec_arg_t * args, tec_ctx_t * ctx);

#endif
