#include <string.h>
#include <stdlib.h>

#include "aux/argvec.h"
#include "tec.h"
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

int tec_cli_cat(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    tec_arg_t args;
    tec_argvec_t vec;
    tec_unit_t *unitpgn;
    tec_unit_t *units;
    int opt_quiet, opt_help;
    int c, i, retcode, status;
    const char *errfmt = "cannot show units '%s': %s";
    const char *unitfmt = "%-" xstr(PADDING_UNIT) "s : %s\n";

    retcode = LIBTEC_OK;
    units = unitpgn = NULL;
    opt_quiet = opt_help = false;
    args.env = args.desk = args.taskid = NULL;

    argvec_init(&vec);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hk:q")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'k':
            argvec_add(&vec, optarg);
            break;
        case 'q':
            opt_quiet = true;
            break;
        case ':':
            return elog(1, "option `-%c' requires an argument", optopt);
        default:
            return elog(1, "invalid option `-%c'", optopt);
        }
    }
    i = optind;

    if (opt_help == true)
        return help_usage("cat");

    if ((status = check_arg_env(&args, errfmt, opt_quiet)))
        return status;
    else if ((status = check_arg_desk(&args, errfmt, opt_quiet)))
        return status;

    do {
        args.taskid = argvec->argv[i];

        if ((status = check_arg_task(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = tec_task_get(teccfg.base.task, &args, ctx))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
        } else if ((status = valid_unitkeys(ctx->units))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, "invalid unit keys");
        } else if ((status = hook_cat(&unitpgn, &args, "cat"))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, "failed to execute hooks");
        }

        if (status == LIBTEC_OK) {
            units = tec_unit_add(units, "id", args.taskid);
            units = tec_unit_join(units, ctx->units);
            units = tec_unit_join(units, unitpgn);

            if (argvec_is_empty(&vec) == false) {
                for (int i = 0; i < vec.used; i++) {
                    int found;
                    for (tec_unit_t * tmp = units; tmp; tmp = tmp->next) {
                        if ((found = strcmp(vec.argv[i], tmp->key)) == 0) {
                            printf("%s\n", tmp->val);
                            break;
                        }
                    }
                    if (found && opt_quiet == false)
                        elog(1, errfmt, vec.argv[i], "no such key");
                    retcode = found == 0 ? retcode : 1;
                }
            } else {
                for (tec_unit_t * tmp = units; tmp; tmp = tmp->next)
                    printf(unitfmt, tmp->key, tmp->val);
            }
        }

        units = ctx->units = unitpgn = tec_unit_free(units);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++i < argvec->used);

    argvec_deinit(&vec);
    return retcode;
}
