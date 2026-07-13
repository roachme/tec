/*
 * TODO:
 * 1. Add support to line-up key names in output. Make sure builtin and plugin
 *    keys line-up well.
 */

#include <stdlib.h>
#include <string.h>
#include "tec.h"
#include "aux/opts.h"
#include "aux/argvec.h"
#include "aux/config.h"

static int valid_unitkeys(tec_unit_t *units)
{
    for (size_t i = 0; units && i < nunitkey; units = units->next, ++i)
        if (strcmp(units->key, unitkeys[i]) != 0)
            return 1;
    return 0;
}

int tec_cli_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    tec_argvec_t keys;
    int retcode = TEC_OK;
    tec_unit_t *units = NULL;
    tec_ctx_t ctx = CTX_INIT();
    tec_unit_t *unitpgn = NULL;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cat_options opts;
    const char *errfmt = "cannot show units '%s': %s";
    const char *unitfmt = "%-" xstr(PADDING_UNIT) "s : %s\n";
    const char *keyfmt = "cannot show units '%s': '%s' no such key";

    argvec_init(&keys);
    tec_cli_cat_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hk:q")) != -1) {
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
        case 'k':
            argvec_add(&keys, optarg);
            break;
        case 'q':
            opts.quiet = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("cat");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("cat");
        }
    }
    argvec->i = optind;

    if (opts.help == true) {
        retcode = TEC_OK;
        tec_cli_help_usage("cat");
        goto err;
    } else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet))) {
        retcode = EXIT_FAILURE;
        goto err;
    } else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet))) {
        retcode = EXIT_FAILURE;
        goto err;
    }

    do {
        args.task = argvec->argv[argvec->i];

        // TODO: check hook unit keys validity
        if (tec_cli_check_task(&args, errfmt, opts.quiet)) {
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_task_get(cfg->base.task, &args, &ctx))) {
            retcode = EXIT_FAILURE;
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(status));
            continue;
        } else if ((status = valid_unitkeys(ctx.units))) {
            retcode = EXIT_FAILURE;
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "invalid unit keys");
            continue;
        } else if ((status = hook_cat(&unitpgn, &args, "cat"))) {
            retcode = EXIT_FAILURE;
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
            continue;
        }

        units = tec_unit_add(units, "id", args.task);
        units = tec_unit_join(units, ctx.units);
        units = tec_unit_join(units, unitpgn);

        /* Show all keys.  */
        if (argvec_is_empty(&keys) == true) {
            for (tec_unit_t * tmp = units; tmp; tmp = tmp->next)
                printf(unitfmt, tmp->key, tmp->val);
        } else {                /* Show specific keys only.  */
            for (int i = 0; i < keys.used; i++) {
                status = aux_show_key(keys.argv[i], units);
                if (status && opts.quiet == false)
                    TEC_LOG_E(keyfmt, args.env, keys.argv[i]);
                retcode = status == EXIT_SUCCESS ? retcode : EXIT_FAILURE;
            }
        }

        units = ctx.units = unitpgn = tec_unit_free(units);
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

 err:
    argvec_deinit(&keys);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
