#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../tec.h"

static char env_curr[ENVSIZ + 1];
static char env_prev[ENVSIZ + 1];

static char desk_curr[DESKSIZ + 1];
static char desk_prev[DESKSIZ + 1];

static char task_curr[IDSIZ + 1];
static char task_prev[IDSIZ + 1];

static char *path_env_toggle(char *base, const tec_arg_t *args)
{
    const char *fmt = "%s/.tec/toggles";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base);
    TEC_LOG_D("%s:pathname: '%s'", __func__, pathname);
    return pathname;
}

static char *path_desk_toggle(char *base, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/.tec/toggles";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->env);
    TEC_LOG_D("%s:pathname: '%s'", __func__, pathname);
    return pathname;
}

static char *path_task_toggle(char *base, const tec_arg_t *args)
{
    const char *fmt = "%s/%s/%s/.tec/toggles";
    static char pathname[PATH_MAX + 1];
    sprintf(pathname, fmt, base, args->env, args->desk);
    TEC_LOG_D("%s:pathname: '%s'", __func__, pathname);
    return pathname;
}

static char *_get_toggle(const char *fname, char *buf, int bufsiz,
                         char *togname)
{
    FILE *fp;
    char *toggle;
    tec_unit_t *units;
    char buffer[BUFSIZ + 1] = { 0 };

    toggle = NULL;
    units = NULL;
    if ((fp = fopen(fname, "r")) == NULL)
        return NULL;

    while (fgets(buffer, BUFSIZ, fp) != NULL)
        units = tec_unit_parse(units, buffer);

    if ((toggle = tec_unit_get(units, togname))) {
        strncpy(buf, toggle, bufsiz);
        buf[bufsiz] = '\0';
    } else {
        buf[0] = '\0';          /* toggle current or previous not found. */
    }

    tec_unit_free(units);
    fclose(fp);
    return buf[0] == '\0' ? NULL : buf;
}

static char *env_get_curr(char *base, tec_arg_t *args)
{
    char *path = path_env_toggle(base, args);
    return _get_toggle(path, env_curr, ENVSIZ, "curr");
}

static char *env_get_prev(char *base, tec_arg_t *args)
{
    char *path = path_env_toggle(base, args);
    return _get_toggle(path, env_prev, ENVSIZ, "prev");
}

static char *desk_get_curr(char *base, tec_arg_t *args)
{
    char *path = path_desk_toggle(base, args);
    return _get_toggle(path, desk_curr, DESKSIZ, "curr");
}

static char *desk_get_prev(char *base, tec_arg_t *args)
{
    char *path = path_desk_toggle(base, args);
    return _get_toggle(path, desk_prev, DESKSIZ, "prev");
}

static char *task_get_curr(char *base, tec_arg_t *args)
{
    char *path = path_task_toggle(base, args);
    return _get_toggle(path, task_curr, IDSIZ, "curr");
}

static char *task_get_prev(char *base, tec_arg_t *args)
{
    char *path = path_task_toggle(base, args);
    return _get_toggle(path, task_prev, IDSIZ, "prev");
}

int toggle_env_get_curr(char *base, tec_arg_t *args)
{
    if (!args->env && !(args->env = env_get_curr(base, args)))
        return 1;
    return 0;
}

int toggle_env_get_prev(char *base, tec_arg_t *args)
{
    if (!args->env && !(args->env = env_get_prev(base, args)))
        return 1;
    return 0;
}

int toggle_desk_get_curr(char *base, tec_arg_t *args)
{
    if (!args->desk && !(args->desk = desk_get_curr(base, args)))
        return 1;
    return 0;
}

int toggle_desk_get_prev(char *base, tec_arg_t *args)
{
    if (!args->desk && !(args->desk = desk_get_prev(base, args)))
        return 1;
    return 0;
}

int toggle_task_get_curr(char *base, tec_arg_t *args)
{
    if (!args->task && !(args->task = task_get_curr(base, args)))
        return 1;
    return 0;
}

int toggle_task_get_prev(char *base, tec_arg_t *args)
{
    if (!args->task && !(args->task = task_get_prev(base, args)))
        return 1;
    return 0;
}

bool toggle_task_is_curr(char *base, tec_arg_t *args)
{
    char *task;

    /* There is no current task ID - immediately return false.  */
    if ((task = task_get_curr(base, args)) == NULL)
        return false;
    return !strcmp(task, args->task);
}

bool toggle_task_is_prev(char *base, tec_arg_t *args)
{
    char *task;

    /* There is no previous task ID - immediately return false.  */
    if ((task = task_get_prev(base, args)) == NULL)
        return false;
    return !strcmp(task, args->task);
}

int toggle_env_set_curr(char *base, tec_arg_t *args)
{
    char *curr, *prev;
    tec_unit_t *toggles;

    toggles = NULL;
    curr = args->env;
    prev = env_get_curr(base, args);

    toggles = tec_unit_add(toggles, "curr", curr);
    if (prev)
        toggles = tec_unit_add(toggles, "prev", prev);

    /* Prevent duplicates in toggles.  */
    if (prev && !strcmp(curr, prev)) {
        // do nothing
    } else {
        tec_unit_save(path_env_toggle(base, args), toggles);
    }
    tec_unit_free(toggles);
    return 0;
}

int toggle_desk_set_curr(char *base, tec_arg_t *args)
{
    char *curr, *prev;
    tec_unit_t *toggles;

    toggles = NULL;
    curr = args->desk;
    prev = desk_get_curr(base, args);

    toggles = tec_unit_add(toggles, "curr", curr);
    if (prev)
        toggles = tec_unit_add(toggles, "prev", prev);

    /* Prevent duplicates in toggles.  */
    if (prev && strcmp(curr, prev) == 0) {
        // do nothing
    } else {
        tec_unit_save(path_desk_toggle(base, args), toggles);
    }
    tec_unit_free(toggles);
    return 0;
}

int toggle_task_set_curr(char *base, tec_arg_t *args)
{
    char *curr, *prev;
    tec_unit_t *toggles;

    toggles = NULL;
    curr = args->task;
    prev = task_get_curr(base, args);

    toggles = tec_unit_add(toggles, "curr", curr);
    if (prev)
        toggles = tec_unit_add(toggles, "prev", prev);

    /* Prevent duplicates in toggles.  */
    if (prev && strcmp(curr, prev) == 0) {
        // do nothing
    } else {
        tec_unit_save(path_task_toggle(base, args), toggles);
    }
    tec_unit_free(toggles);
    return 0;
}

int toggle_task_unset_curr(char *base, tec_arg_t *args)
{
    int status;
    char *curr, *prev;
    tec_unit_t *toggles;

    toggles = NULL;
    curr = task_get_curr(base, args);
    prev = task_get_prev(base, args);

    if (curr == NULL)
        return 1;
    else if (prev != NULL)
        toggles = tec_unit_add(toggles, "curr", prev);

    status = tec_unit_save(path_task_toggle(base, args), toggles);
    tec_unit_free(toggles);
    return status;
}

int toggle_task_unset_prev(char *base, tec_arg_t *args)
{
    int status;
    char *curr, *prev;
    tec_unit_t *toggles;

    toggles = NULL;
    curr = task_get_curr(base, args);
    prev = task_get_prev(base, args);

    if (prev == NULL)
        return 1;               // do nothing
    if (curr != NULL) {
        // rewrite curr with the same value
        toggles = tec_unit_add(toggles, "curr", curr);
    }
    status = tec_unit_save(path_task_toggle(base, args), toggles);
    tec_unit_free(toggles);
    return status;
}

/*
 * Update task toggles after a task is renamed.
 * If old_id matches curr or prev, update it to new_id.
 * args must have env and desk set for the location.
 */
int toggle_task_update(char *base, tec_arg_t *args,
                       const char *old_id, const char *new_id)
{
    char *curr, *prev;
    tec_unit_t *toggles;
    int changed = 0;

    toggles = NULL;
    curr = task_get_curr(base, args);
    prev = task_get_prev(base, args);

    /* Check if old_id matches curr or prev and update */
    if (curr && strcmp(curr, old_id) == 0) {
        curr = (char *)new_id;
        changed = 1;
    }
    if (prev && strcmp(prev, old_id) == 0) {
        prev = (char *)new_id;
        changed = 1;
    }

    if (!changed)
        return 0;               /* Nothing to update */

    if (curr)
        toggles = tec_unit_add(toggles, "curr", curr);
    if (prev)
        toggles = tec_unit_add(toggles, "prev", prev);

    if (toggles) {
        tec_unit_save(path_task_toggle(base, args), toggles);
        tec_unit_free(toggles);
    }
    return 0;
}

/*
 * Clear a task from toggles when it's moved to a different desk/env.
 * If task matches curr, promote prev to curr and clear prev.
 * If task matches prev, just clear prev.
 * args must have env and desk set for the source location.
 */
int toggle_task_clear(char *base, tec_arg_t *args, const char *task)
{
    char *curr, *prev;
    tec_unit_t *toggles;
    int changed = 0;

    toggles = NULL;
    curr = task_get_curr(base, args);
    prev = task_get_prev(base, args);

    if (curr && strcmp(curr, task) == 0) {
        /* Current task is being moved, promote prev to curr */
        curr = prev;
        prev = NULL;
        changed = 1;
    } else if (prev && strcmp(prev, task) == 0) {
        /* Previous task is being moved, just clear it */
        prev = NULL;
        changed = 1;
    }

    if (!changed) {
        TEC_LOG_D("%s: NOT HAPPENED\n", __func__);
        return 0;
    }

    if (curr)
        toggles = tec_unit_add(toggles, "curr", curr);
    if (prev)
        toggles = tec_unit_add(toggles, "prev", prev);

    tec_unit_save(path_task_toggle(base, args), toggles);
    tec_unit_free(toggles);
    return 0;
}
