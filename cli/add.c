#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "tec.h"
#include "cd.h"
#include "aux/argvec.h"
#include "aux/config.h"
#include "aux/toggle.h"

static int generate_task(tec_arg_t *args, tec_argvec_t *argvec)
{
    char gentask[IDSIZ + 1] = { 0 };

    args->taskid = gentask;
    for (register unsigned int i = 1; i < IDLIMIT; ++i) {
        sprintf(gentask, IDFMT, i);
        if (tec_task_exist(teccfg.base.task, args) != LIBTEC_OK) {
            argvec_add(argvec, gentask);
            args->taskid = argvec->argv[argvec->used];
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
    struct tm *timeinfo = localtime(&rawtime);
    char _desc[100] = "Generated desciption for ";
    char *unitvals[] = { "mid", "task", date, _desc, };

    /* Set custom description if provided.  */
    if (desc) {
        unitvals[ARRAY_SIZE(unitvals) - 1] = desc;
    } else {                    /* Generate description.  */
        strcat(_desc, args->taskid);
    }

    strftime(date, BUFSIZ, timefmt, timeinfo);

    for (int i = 0; i < nunitkey; ++i)
        units = tec_unit_add(units, unitkeys[i], unitvals[i]);
    ctx->units = units;
    return 0;
}

int tec_cli_add(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    int c;
    int status;
    char *desc = NULL;
    int retcode = LIBTEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot create task '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hqnD:N")) != -1) {
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
        case 'D':
            desc = optarg;
            break;
        case 'N':
            opts.chdir = false;
            opts.chtog = false;
            break;
        case ':':
            elog(EXIT_FAILURE, FMT_OPT_ARG_REQ, optopt);
            return help_usage("add");
        default:
            elog(EXIT_FAILURE, FMT_OPT_ARG_INV, optopt);
            return help_usage("add");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return help_usage("add");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet)))
        return EXIT_FAILURE;
    else if (optind == argvec->used && generate_task(&args, argvec)) {
        if (opts.quiet == false)
            elog(1, "could not generate task ID: limit is %d", IDLIMIT);
        return EXIT_FAILURE;
    }

    do {
        args.taskid = argvec->argv[argvec->i];

        if (tec_cli_len_valid(args.taskid, IDSIZ) == false) {
            status = 1;
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, "task ID is too long");
            retcode = status == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
            continue;
        } else if ((status = tec_task_valid(teccfg.base.task, &args))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
            retcode = status == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
            continue;
        } else if (!(status = tec_task_exist(teccfg.base.task, &args))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.taskid, tec_strerror(LIBTEC_ARG_EXISTS));
            retcode = !(status == LIBTEC_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
            continue;
        } else if ((status = generate_units(ctx, &args, desc))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.taskid, "unit generation failed");
            retcode = status == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
            continue;
        }

        if ((status = tec_task_add(teccfg.base.task, &args, ctx))) {
            if (opts.quiet == false)
                elog(status, errfmt, args.taskid, tec_strerror(status));
        } else if ((status = hook_action(&args, "add"))) {
            if (opts.quiet == false)
                elog(1, errfmt, args.taskid, "failed to execute hooks");
        } else if (opts.chtog == true) {
            if ((status = toggle_task_set_curr(teccfg.base.task, &args))) {
                if (opts.quiet == false)
                    elog(status, "could not update toggles");
            }
        }
        ctx->units = tec_unit_free(ctx->units);
        retcode = status == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
    } while (++argvec->i < argvec->used);

    if (retcode == LIBTEC_OK && opts.chdir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == LIBTEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
