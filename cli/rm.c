#include <stdlib.h>
#include "rm.h"
#include "tec.h"
#include "aux/aux.h"
#include "aux/osdep.h"
#include "aux/toggle.h"
#include "aux/config.h"

static int update_toggles_and_cwd(tec_arg_t *args,
                                  struct tec_cli_rm_options *opts)
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

void tec_cli_rm_option_init(struct tec_cli_rm_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->verbose = false;
    opts->change_dir = false;
    opts->interactive = RMI_ALWAYS;
}

int tec_cli_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int ntasks;
    tec_ctx_t ctx = CTX_INIT;
    int retcode = LIBTEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_rm_options opts;
    const char *errfmt = "cannot remove task '%s': %s";

    tec_cli_rm_option_init(&opts);
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
            elog(EXIT_FAILURE, FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("rm");
        default:
            elog(EXIT_FAILURE, FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("rm");
        }
    }
    argvec->i = optind;
    ntasks = argvec->used - argvec->i;

    if (opts.help == true)
        return tec_cli_help_usage("rm");
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
        args.task = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args, errfmt, opts.quiet))) {
            retcode = status == LIBTEC_OK ? retcode : status;
            continue;
        } else if (opts.interactive == RMI_ALWAYS) {
            tec_cli_log_prompt(0, "remove task '%s'? [y/N] ", args.task);
            if (!yesno())
                continue;
        }

        if ((status = hook_action(&args, "rm"))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.task, "failed to execute hooks");
        } else if ((status = update_toggles_and_cwd(&args, &opts))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.task, "could not update toggles");
        } else if ((status = tec_task_del(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.task, tec_strerror(status));
        } else if (opts.verbose == true)
            llog(0, "removed task '%s'", args.task);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == LIBTEC_OK && opts.change_dir) {
        args.task = NULL;       // FIXME: ducking hotfix to get current task ID from file
        if (toggle_task_get_curr(cfg->base.task, &args))
            args.task = "";
        retcode = tec_cli_pwd_set(&args) == LIBTEC_OK ? retcode : status;
    }

    return retcode == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
