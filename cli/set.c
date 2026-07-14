#include <stdlib.h>

#include "tec.h"
#include "aux/aux.h"
#include "aux/opts.h"
#include "aux/errno.h"
#include "aux/config.h"

// "prio",  /* task priority */
// "type",  /* task type: bugfix, hotfix, feature */
// "date",  /* task date of creation */
// "desc",  /* task description */
// "user"   /* who created, who's woring on it */
// "users"  /* list of users */
// "teams"  /* list of teams */
// "label"  /* list of labels */
// "time"   /* time tracker */

int tec_cli_set(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_set_options opts;
    const char *errfmt = "cannot set task units '%s': %s";

    tec_cli_set_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hiqD:T:P:")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'h':
            opts.help = true;
            break;
        case 'i':
            return TEC_LOG_E("this option is under development");
        case 'T':
            if (tec_aux_is_valid_type(optarg) == false) {
                TEC_LOG_E("invalid type '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "type") == NULL)
                ctx.units = tec_unit_add(ctx.units, "type", optarg);
            break;
        case 'D':
            if (tec_aux_is_valid_desc(optarg) == false) {
                TEC_LOG_E("invalid description '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "desc") == NULL)
                ctx.units = tec_unit_add(ctx.units, "desc", optarg);
            break;
        case 'P':
            if (tec_aux_is_valid_prio(optarg) == false) {
                TEC_LOG_E("invalid priority '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "prio") == NULL)
                ctx.units = tec_unit_add(ctx.units, "prio", optarg);
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("set");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("set");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("set");
    else if (tec_cli_check_env(&args, errfmt, opts.quiet))
        return EXIT_FAILURE;
    else if (tec_cli_check_desk(&args, errfmt, opts.quiet))
        return EXIT_FAILURE;

    do {
        args.task = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_task_set(cfg->base.task, &args, &ctx))) {
            args.task = args.task ? args.task : "NOCURR";
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(status));
        } else if ((status = hook_action(&args, "set"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }

        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    ctx.units = tec_unit_free(ctx.units);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
