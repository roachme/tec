#include <stdlib.h>
#include <string.h>

#include "tec.h"
#include "aux/opts.h"
#include "aux/errno.h"
#include "aux/toggle.h"
#include "aux/config.h"

int tec_cli_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = ETEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hnpqN")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'h':
            opts.help = true;
            break;
        case 'n':
            opts.change_tog = false;
            break;
        case 'p':
            return TEC_LOG_E(EFMT_DEV, c);
        case 'q':
            opts.quiet = true;
            break;
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("cd");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("cd");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("cd");
    else if ((status = tec_cli_check_env(&args))) {
        args.env = args.env ? args.env : ETEC_NOCURR;
        if (opts.quiet == false)
            TEC_LOG_E(EFMT_TASK_CD, args.env, tec_strerror(status));
        return EXIT_FAILURE;
    } else if ((status = tec_cli_check_desk(&args))) {
        args.desk = args.desk ? args.desk : ETEC_NOCURR;
        if (opts.quiet == false)
            TEC_LOG_E(EFMT_TASK_CD, args.desk, tec_strerror(status));
        return EXIT_FAILURE;
    } else if ((status = tec_aux_check_cd_alias(argvec)))
        return TEC_LOG_E(tec_strerror(status));

    /* Resolve alias '-' to switch to previous task ID.  */
    if (argvec->argv[argvec->i] && strcmp("-", argvec->argv[argvec->i]) == 0) {
        if ((status = toggle_task_get_prev(cfg->base.task, &args)))
            return TEC_LOG_E(EFMT_TASK_CD, ETEC_NOPREV, tec_strerror(status));
        argvec_replace(argvec, argvec->i, args.task);
    }

    do {
        args.task = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args))) {
            args.task = args.task ? args.task : ETEC_NOCURR;
            if (opts.quiet == false)
                TEC_LOG_E(EFMT_TASK_CD, args.task, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = hook_action(&args, "cd"))) {
            if (opts.quiet == false)
                TEC_LOG_E(EFMT_TASK_CD, args.task, tec_strerror(status));
        } else if (opts.change_tog == true) {
            if ((status = toggle_task_set_curr(cfg->base.task, &args))) {
                if (opts.quiet == false)
                    TEC_LOG_E(tec_strerror(status));
            }
        }
        retcode = status == ETEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == ETEC_OK && opts.change_dir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == ETEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
