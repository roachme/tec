#include <stdlib.h>
#include <string.h>
#include "cd.h"
#include "tec.h"
#include "aux/argvec.h"
#include "aux/config.h"

static int valid_unitkeys(tec_unit_t *units)
{
    for (int i = 0; units && i < nunitkey; units = units->next) {
        if (strcmp(units->key, unitkeys[i]) != 0)
            return 1;
        ++i;
    }
    return 0;
}

int tec_cli_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    tec_ctx_t ctx = CTX_INIT;
    tec_argvec_t keyvec;
    int retcode = LIBTEC_OK;
    tec_unit_t *units = NULL;
    tec_unit_t *unitpgn = NULL;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot show units '%s': %s";
    const char *unitfmt = "%-" xstr(PADDING_UNIT) "s : %s\n";
    const char *keyfmt = "cannot show units '%s': '%s' no such key";

    argvec_init(&keyvec);
    tec_cli_cd_option_init(&opts);
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
            argvec_add(&keyvec, optarg);
            break;
        case 'q':
            opts.quiet = true;
            break;
        case ':':
            elog(EXIT_FAILURE, FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("cat");
        default:
            elog(EXIT_FAILURE, FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("cat");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("cat");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;

    do {
        args.taskid = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args, errfmt, opts.quiet))) {
            retcode = status == LIBTEC_OK ? retcode : status;
            continue;
        }

        if ((status = tec_task_get(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
        } else if ((status = valid_unitkeys(ctx.units))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, "invalid unit keys");
        } else if ((status = hook_cat(&unitpgn, &args, "cat"))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, "failed to execute hooks");
        }

        if (status == LIBTEC_OK) {
            units = tec_unit_add(units, "id", args.taskid);
            units = tec_unit_join(units, ctx.units);
            units = tec_unit_join(units, unitpgn);

            /* Show specific keys only.  */
            if (argvec_is_empty(&keyvec) == false) {
                for (int i = 0; i < keyvec.used; i++) {
                    int notfound;
                    for (tec_unit_t * tmp = units; tmp; tmp = tmp->next) {
                        if ((notfound = strcmp(keyvec.argv[i], tmp->key)) == 0) {
                            printf("%s\n", tmp->val);
                            break;
                        }
                    }
                    if (notfound && opts.quiet == false)
                        elog(1, keyfmt, args.taskid, keyvec.argv[i]);
                    retcode = notfound == 0 ? retcode : 1;
                }
            } else {            /* Show all keys.  */
                for (tec_unit_t * tmp = units; tmp; tmp = tmp->next)
                    printf(unitfmt, tmp->key, tmp->val);
            }
        }

        units = ctx.units = unitpgn = tec_unit_free(units);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    argvec_deinit(&keyvec);
    return retcode == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
