/* TODO: Find a good error message in case option fails.
 *
 * 1. Add option `-d' - remove empty/done tasks (maybe???)...
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tec.h"
#include "aux/toggle.h"
#include "aux/config.h"

#ifdef __linux__
#include <unistd.h>

#define CWD_DELIM "/"
#endif

static char *user_cwd_ptr;
static int change_dir = false;

#ifdef __linux__
static char *OS_GETCWD(void)
{
    /* Get logical current working directory via shell variable, because
     * user current working directory might a symlink.  */
    return getenv("PWD");
}
#endif

static char *get_user_cwd(void)
{
    return OS_GETCWD();
}

static bool do_change_user_cwd(tec_arg_t *args)
{
    char *base = teccfg.base.task;
    char buf[FILENAME_MAX + 1] = { 0 };

    sprintf(buf, "%s/%s/%s/%s", base, args->env, args->desk, args->taskid);
    return strcmp(buf, get_user_cwd()) == 0;
}

static int update_toggles_and_cwd(tec_arg_t *args)
{
    int status;

    status = LIBTEC_OK;
    if (do_change_user_cwd(args) == true)
        change_dir = true;

    /* Update current and previos toggles.  */
    if (toggle_task_is_curr(teccfg.base.task, args)) {
        toggle_task_unset_curr(teccfg.base.task, args);
    } else if (toggle_task_is_prev(teccfg.base.task, args)) {
        toggle_task_unset_curr(teccfg.base.task, args);
    }
    return status;
}

int tec_cli_rm(int argc, const char **argv, tec_ctx_t *ctx)
{
    tec_arg_t args;
    const char *errfmt;
    tec_argvec_t argvec;
    int c, i, retcode, status;
    int opt_quiet, opt_help, opt_verbose;
    int opt_ask_once, opt_ask_every;

    opt_ask_every = true;       /* prompt before every removal.  */
    opt_ask_once = false;       /* prompt before once for all task IDs.  */
    retcode = LIBTEC_OK;
    user_cwd_ptr = get_user_cwd();
    errfmt = "cannot remove task '%s': %s";
    args.env = args.desk = args.taskid = NULL;
    opt_quiet = opt_help = opt_verbose = false;

    argvec_init(&argvec);
    argvec_parse(&argvec, argc, argv);
    while ((c = getopt(argvec.used, argvec.argv, ":d:e:fihqvI")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'f':
            opt_ask_every = false;
            opt_ask_once = false;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'i':
            opt_ask_every = true;
            opt_ask_once = false;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'v':
            opt_verbose = true;
            break;
        case 'I':
            opt_ask_every = false;
            opt_ask_once = true;
            break;
        case ':':
            return elog(1, "option `-%c' requires an argument", optopt);
        default:
            return elog(1, "invalid option `-%c'", optopt);
        }
    }
    i = optind;

    if (opt_help == true)
        return help_usage("rm");
    else if (user_cwd_ptr == NULL)
        return elog(1, errfmt, "TASK", "could not get CWD");

    if ((status = check_arg_env(&args, errfmt, opt_quiet)))
        return status;
    else if ((status = check_arg_desk(&args, errfmt, opt_quiet)))
        return status;

    if (opt_ask_once == true) {
        printf("Are you sure to remove task(s)? [y/N] ");
        if (tec_cli_get_user_choice() == false) {
            return LIBTEC_OK;
        }
    }

    do {
        args.taskid = argvec.argv[i];

        if ((status = check_arg_task(&args, errfmt, opt_quiet))) {
            retcode = status == LIBTEC_OK ? retcode : status;
            continue;
        } else if (opt_ask_every == true) {
            printf("Are you sure to remove task '%s'? [y/N] ", args.taskid);
            if (tec_cli_get_user_choice() == false)
                continue;
        }

        if ((status = hook_action(&args, "rm"))) {
            if (opt_quiet == false)
                elog(1, errfmt, args.taskid, "failed to execute hooks");
        } else if ((status = update_toggles_and_cwd(&args))) {
            if (opt_quiet == false)
                elog(1, errfmt, args.taskid, "could not update toggles");
        } else if ((status = tec_task_del(teccfg.base.task, &args, ctx))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
        } else if (opt_verbose == true)
            llog(0, "removed task '%s'", args.taskid);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++i < argvec.used);

    if (retcode == LIBTEC_OK && change_dir) {
        args.taskid = NULL;     // FIXME: ducking hotfix to get current task ID from file
        if (toggle_task_get_curr(teccfg.base.task, &args))
            args.taskid = "";
        retcode = tec_pwd_task(&args) == LIBTEC_OK ? retcode : status;
    }

    argvec_free(&argvec);
    return retcode;
}
