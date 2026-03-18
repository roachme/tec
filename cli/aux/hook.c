#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "hook.h"
#include "config.h"
#include "../tec.h"
#include "../../lib/libtec.h"

static char pathname[PATH_MAX + 1];

static char *_hook_cmd(tec_arg_t *args, char *name, char *cmd)
{
    const char *fmt = "%s/%s/%s -T %s %s -e %s -d %s %s";
    sprintf(pathname, fmt, teccfg.base.pgn, name, name, teccfg.base.task,
            cmd, args->env, args->desk, args->taskid);
    return pathname;
}

int hook_action(tec_arg_t *args, char *cmd)
{
    int retcode, status;
    struct tec_hook *hooks = teccfg.hooks;

    retcode = status = LIBTEC_OK;

    /* Execute hooks only if they are enabled.  */
    if (teccfg.opts.hook == false)
        return 0;

    for (; hooks; hooks = hooks->next) {
        if (strcmp(cmd, hooks->cmd) == 0) {
            char *cmd = _hook_cmd(args, hooks->pgname, hooks->pgncmd);
            dlog(1, cmd);
            status = system(cmd) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
            retcode = status == LIBTEC_OK ? retcode : status;
        }
    }

    return retcode;
}

int hook_show(tec_unit_t **units, tec_arg_t *args, char *cmd)
{
    FILE *pipe;
    int retcode, status;
    char line[BUFSIZ + 1] = { 0 };
    struct tec_hook *hooks = teccfg.hooks;

    retcode = status = LIBTEC_OK;

    /* Execute hooks only if they are enabled.  */
    if (teccfg.opts.hook == false)
        return 0;

    for (; hooks; hooks = hooks->next) {
        if (strcmp(hooks->cmd, cmd) != 0)
            continue;

        char *hook_cmd = _hook_cmd(args, hooks->pgname, hooks->pgncmd);
        //printf("## hook_cmd: '%s'\n", hook_cmd);
        if (!(pipe = popen(hook_cmd, "r"))) {
            // TODO: add quiet option and show error message
            continue;
        }
        while (fgets(line, BUFSIZ, pipe))
            *units = tec_unit_parse(*units, line);
        pclose(pipe);
    }
    return retcode;
}

/*
char *hook_list(struct tec_hook *hooks, char *pgnout, char *env, char *task)
{
    FILE *pipe;
    char *prefix = "  ";
    char line[BUFSIZ + 1] = { 0 };

    // Execute hooks only if they are enabled.
    if (teccfg.opts.hook == true)
        return 0;

    for (; hooks; hooks = hooks->next) {
        if (strcmp(hooks->cmd, "list") != 0)
            continue;

        if ((pipe =
             popen(genpath_pgn(env, task, hooks->pgname, hooks->pgncmd),
                   "r")) == NULL) {
            return NULL;
        }
        // NOTE: gotta get a single word
        if (fgets(line, BUFSIZ, pipe)) {
            line[strcspn(line, "\n")] = 0;
            strcat(pgnout, prefix);
            strcat(pgnout, line);
        }

        pclose(pipe);
    }

    if (pgnout[1] == ' ') {
        pgnout[1] = '[';
        strcat(pgnout, "]");
    }
    //return LIBTEC_OK;
    return pgnout;
}
*/
