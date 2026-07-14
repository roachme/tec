#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "tec.h"
#include "aux/opts.h"
#include "aux/argvec.h"
#include "aux/config.h"
#include "aux/toggle.h"

static int generate_task(tec_arg_t *args, tec_argvec_t *argvec)
{
    static char gentask[IDSIZ + 1] = { 0 };

    args->task = gentask;
    for (register unsigned int i = 1; i < IDLIMIT; ++i) {
        sprintf(gentask, IDFMT, i);
        if (tec_task_exist(teccfg.base.task, args) != TEC_OK) {
            argvec_add(argvec, gentask);
            args->task = argvec->argv[argvec->used];
            return 0;
        }
    }
    return 1;
}

static int generate_units(tec_ctx_t *ctx, tec_arg_t *args, char *desc)
{
    char date[BUFSIZ + 1];
    struct tec_unit *units = NULL;
    time_t rawtime = time(NULL);
    const char timefmt[] = "%Y%m%d";
    const struct tm *timeinfo = localtime(&rawtime);
    char _desc[100] = "Generated desciption for ";
    char *unitvals[] = { "mid", "task", date, _desc, };

    /* Set custom description if provided.  */
    if (desc) {
        unitvals[ARRAY_SIZE(unitvals) - 1] = desc;
    } else {                    /* Generate description.  */
        strcat(_desc, args->task);
    }

    strftime(date, BUFSIZ, timefmt, timeinfo);

    for (size_t i = 0; i < nunitkey; ++i)
        units = tec_unit_add(units, unitkeys[i], unitvals[i]);

    if ((ctx->units = units) == NULL)
        return 1;
    return 0;
}

int tec_cli_add(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    char *desc = NULL;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot add task '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hqnvD:N")) != -1) {
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
        case 'q':
            opts.quiet = true;
            break;
        case 'v':
            opts.verbose = true;
            break;
        case 'D':
            desc = optarg;
            break;
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("add");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("add");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("add");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if (optind == argvec->used && generate_task(&args, argvec)) {
        if (opts.quiet == false)
            TEC_LOG_E("cannot generate task ID: limit is %d", IDLIMIT);
        return EXIT_FAILURE;
    }

    do {
        args.task = argvec->argv[argvec->i];

        if (tec_cli_len_valid(args.task, IDSIZ) == false) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "task ID is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_task_valid(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_task_exist(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = generate_units(&ctx, &args, desc))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "unit generation failed");
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_task_add(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, tec_strerror(status));
        } else if ((status = hook_action(&args, "add"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opts.change_tog == true) {
            if ((status = toggle_task_set_curr(cfg->base.task, &args))) {
                if (opts.quiet == false)
                    TEC_LOG_E("cannot update toggles");
            }
        }

        if (opts.verbose == true)
            TEC_LOG_I("added task '%s'", args.task);

        ctx.units = tec_unit_free(ctx.units);
        retcode = status == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
