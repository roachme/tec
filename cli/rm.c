#include "tec.h"
#include "aux/log.h"
#include "aux/aux.h"
#include "aux/opts.h"
#include "aux/osdep.h"
#include "aux/toggle.h"
#include "aux/config.h"

static int update_cwd(tec_arg_t *args, struct tec_cli_rm_options *opts)
{
    char *home;
    int status = TEC_OK;

    /* Not to break shell session in case user CWD gets deleted.  */
    if (tec_aux_do_change_user_cwd(args) == true) {
        if ((home = tec_cli_osdep_getenv_home()) == NULL) {
            TEC_LOG_D("could not get env 'HOME' variable");
            status = EXIT_FAILURE;
        } else if (tec_cli_osdep_chdir(home)) {
            TEC_LOG_D("could not change user CWD");
            status = EXIT_FAILURE;
        }
        opts->change_dir = true;
    }
    return status;
}

static int update_toggles(tec_arg_t *args)
{
    int status = TEC_OK;

    if (toggle_task_is_curr(teccfg.base.task, args))
        status = toggle_task_unset_curr(teccfg.base.task, args);
    else if (toggle_task_is_prev(teccfg.base.task, args))
        status = toggle_task_unset_prev(teccfg.base.task, args);
    return status;
}

int tec_cli_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int ntasks;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
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
            opts.mode = RMI_NEVER;
            break;
        case 'h':
            opts.help = true;
            break;
        case 'i':
            opts.mode = RMI_ALWAYS;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'v':
            opts.verbose = true;
            break;
        case 'I':
            opts.mode = RMI_SOMETIMES;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("rm");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
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

    if (ntasks > 3 && opts.mode == RMI_SOMETIMES) {
        TEC_LOG_P("remove %d tasks? [y/N] ", ntasks);
        if (!tec_aux_yesno()) {
            return EXIT_SUCCESS;
        }
    }

    do {
        args.task = argvec->argv[argvec->i];

        if (tec_cli_check_task(&args, errfmt, opts.quiet)) {
            retcode = EXIT_FAILURE;
            continue;
        } else if (opts.mode == RMI_ALWAYS) {
            TEC_LOG_P("remove task '%s'? [y/N] ", args.task);
            if (!tec_aux_yesno())
                continue;
        }

        if ((status = hook_action(&args, "rm"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if ((status = update_cwd(&args, &opts))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "could not update CWD");
        } else if ((status = update_toggles(&args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "could not update toggles");
        } else if ((status = tec_task_rm(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(status));
        }

        if (opts.verbose == true)
            TEC_LOG_I("removed task '%s'", args.task);
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir) {
        args.task = NULL;       /* Force to get current task ID.  */
        if (toggle_task_get_curr(cfg->base.task, &args))
            args.task = "";
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;
    }

    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
