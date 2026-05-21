#include <stdlib.h>
#include <string.h>

#include "tec.h"
#include "cd.h"
#include "aux/toggle.h"
#include "aux/config.h"

void tec_cli_cd_option_init(struct tec_cli_cd_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->chdir = true;
    opts->chtog = true;
}

int tec_cli_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = LIBTEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot switch to '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hnqN")) != -1) {
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
            opts.chtog = false;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'N':
            opts.chdir = false;
            opts.chtog = false;
            break;
        case ':':
            elog(EXIT_FAILURE, FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("cd");
        default:
            elog(EXIT_FAILURE, FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("cd");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("cd");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;

    /* Check that alias '-' is not passed with other task IDs nor duplicated.  */
    for (int idx = argvec->i; idx < argvec->used; ++idx) {
        if (strcmp(argvec->argv[idx], "-") == 0 && argvec->used - argvec->i > 1)
            return elog(1, "alias '-' is used alone");
    }

    /* Resolve alias '-' to switch to previous task ID.  */
    if (argvec->argv[argvec->i] && strcmp("-", argvec->argv[argvec->i]) == 0) {
        if ((status = toggle_task_get_prev(cfg->base.task, &args)))
            return elog(1, errfmt, "PREV", "no previous task ID");
        argvec_replace(argvec, argvec->i, args.task, IDSIZ);
    }

    do {
        args.task = argvec->argv[argvec->i];

        if ((status = tec_cli_check_task(&args, errfmt, opts.quiet))) {
            retcode = status == LIBTEC_OK ? retcode : status;
            continue;
        }

        if ((status = hook_action(&args, "cd"))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.task, "failed to execute hooks");
        } else if (opts.chtog == true) {
            if ((status = toggle_task_set_curr(cfg->base.task, &args))) {
                if (opts.quiet == false)
                    elog(1, "could not update toggles");
            }
        }
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == LIBTEC_OK && opts.chdir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
