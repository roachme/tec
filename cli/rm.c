#include <stdlib.h>
#include "rm.h"
#include "tec.h"
#include "aux/aux.h"
#include "aux/osdep.h"
#include "aux/toggle.h"
#include "aux/config.h"

static int update_toggles_and_cwd(tec_arg_t *args, struct rm_options *opts)
{
    char *home;
    int status = LIBTEC_OK;

    /* Not to break shell session in case user CWD gets deleted.  */
    if (do_change_user_cwd(args) == true) {
        if ((home = tec_cli_osdep_getenv_home()) == NULL) {
            elog(EXIT_FAILURE, "could not get env 'HOME' variable");
            exit(EXIT_FAILURE);
        } else if (tec_cli_osdep_chdir(home)) {
            elog(EXIT_FAILURE, "could not change user CWD");
            exit(EXIT_FAILURE);
        }
        opts->change_dir = true;
    }

    /* Update current and previos toggles.  */
    if (toggle_task_is_curr(teccfg.base.task, args)) {
        toggle_task_unset_curr(teccfg.base.task, args);
    } else if (toggle_task_is_prev(teccfg.base.task, args)) {
        toggle_task_unset_prev(teccfg.base.task, args);
    }
    return status;
}

void rm_option_init(struct rm_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->verbose = false;
    opts->change_dir = false;
    opts->interactive = RMI_ALWAYS;
}

int tec_cli_rm(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    int c;
    int status;
    int ntasks;
    struct rm_options opts;
    int retcode = LIBTEC_OK;
    tec_arg_t args = ARGS_INIT();
    const char *errfmt = "cannot remove task '%s': %s";

    rm_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:fihqvI")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'f':
            opts.interactive = RMI_NEVER;
            break;
        case 'h':
            opts.help = true;
            break;
        case 'i':
            opts.interactive = RMI_ALWAYS;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'v':
            opts.verbose = true;
            break;
        case 'I':
            opts.interactive = RMI_SOMETIMES;
            break;
        case ':':
            return elog(1, "option `-%c' requires an argument", optopt);
        default:
            return elog(1, "invalid option `-%c'", optopt);
        }
    }
    argvec->i = optind;
    ntasks = argvec->used - argvec->i;

    if (opts.help == true)
        return help_usage("rm");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;

    if (ntasks >= 3 && opts.interactive == RMI_SOMETIMES) {
        tec_cli_log_prompt(0, "remove %d tasks? [y/N] ", ntasks);
        if (yesno() == false) {
            return EXIT_SUCCESS;
        }
    }

    do {
        args.taskid = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args, errfmt, opts.quiet))) {
            retcode = status == LIBTEC_OK ? retcode : status;
            continue;
        } else if (opts.interactive == RMI_ALWAYS) {
            tec_cli_log_prompt(0, "remove task '%s'? [y/N] ", args.taskid);
            if (!yesno())
                continue;
        }

        if ((status = hook_action(&args, "rm"))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.taskid, "failed to execute hooks");
        } else if ((status = update_toggles_and_cwd(&args, &opts))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.taskid, "could not update toggles");
        } else if ((status = tec_task_del(teccfg.base.task, &args, ctx))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
        } else if (opts.verbose == true)
            llog(0, "removed task '%s'", args.taskid);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == LIBTEC_OK && opts.change_dir) {
        args.taskid = NULL;     // FIXME: ducking hotfix to get current task ID from file
        if (toggle_task_get_curr(teccfg.base.task, &args))
            args.taskid = "";
        retcode = tec_pwd_task(&args) == LIBTEC_OK ? retcode : status;
    }

    return retcode == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
