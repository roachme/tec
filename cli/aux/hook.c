#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "log.h"
#include "hook.h"
#include "config.h"
#include "errno.h"
#include "../../lib/libtec.h"

static char pathname[PATH_MAX + 1];

static char *_hook_cmd(tec_arg_t *args, char *name, char *cmd)
{
    const char *fmt = "%s/%s/%s -T %s %s -e %s -d %s %s";
    sprintf(pathname, fmt, teccfg.base.pgn, name, name, teccfg.base.task,
            cmd, args->env, args->desk, args->task);
    return pathname;
}

int hook_action(tec_arg_t *args, char *cmd)
{
    int retcode, status;
    struct tec_hook *hooks = teccfg.hooks;

    retcode = status = TEC_OK;

    /* Execute hooks only if they are enabled.  */
    if (teccfg.opts.hook == false)
        return 0;

    for (; hooks; hooks = hooks->next) {
        if (strcmp(cmd, hooks->cmd) == 0) {
            char *_cmd = _hook_cmd(args, hooks->pgname, hooks->pgncmd);
            TEC_LOG_D(_cmd);
            status = system(_cmd) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
            retcode = status == TEC_OK ? retcode : status;
        }
    }

    return retcode == TEC_OK ? TEC_OK : TEC_HOOK;
}

int hook_cat(tec_unit_t **units, tec_arg_t *args, char *cmd)
{
    FILE *pipe;
    int retcode, status;
    char line[BUFSIZ + 1] = { 0 };
    struct tec_hook *hooks = teccfg.hooks;

    retcode = status = TEC_OK;

    /* Execute hooks only if they are enabled.  */
    if (teccfg.opts.hook == false)
        return EXIT_SUCCESS;

    for (; hooks; hooks = hooks->next) {
        if (strcmp(hooks->cmd, cmd) != 0)
            continue;

        char *hook_cmd = _hook_cmd(args, hooks->pgname, hooks->pgncmd);
        TEC_LOG_D(hook_cmd);
        if (!(pipe = popen(hook_cmd, "r"))) {
            // TODO: add quiet option and show error message of plugin
            retcode = EXIT_FAILURE;
            continue;
        }
        while (fgets(line, BUFSIZ, pipe))
            *units = tec_unit_parse(*units, line);
        retcode = pclose(pipe) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
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
    //return TEC_OK;
    return pgnout;
}
*/
