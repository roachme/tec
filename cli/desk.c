#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "tec.h"
#include "aux/opts.h"
#include "aux/toggle.h"
#include "aux/config.h"

// TODO: remove parameter 'quiet', return status, and let the caller to
static char *get_unit_desc(tec_ctx_t *ctx, tec_arg_t *args, int quiet,
                           tec_cfg_t *cfg)
{
    int status;
    char *desc = NULL;

    if ((status = tec_desk_get(cfg->base.task, args, ctx))) {
        if (quiet == false)
            TEC_LOG_E("'%s': %s one", args->desk, tec_strerror(status));
    } else if ((desc = tec_unit_get(ctx->units, "desc")) == NULL) {
        if (quiet == false)
            TEC_LOG_E("'%s': %s", args->desk, "description not found");
    }
    return desc;
}

static int generate_units(tec_ctx_t *ctx, char *desk, char *desc)
{
    char buf[BUFSIZ + 1] = { 0 };
    struct tec_unit *units = NULL;
    char _desc[100] = "Generated desciption for ";
    char *unitvals[] = { _desc, };

    /* Set custom description if provided.  */
    if (desc) {
        unitvals[ARRAY_SIZE(unitvals) - 1] = desc;
    } else {                    /* Generate description.  */
        strcat(_desc, desk);
    }

    units = tec_unit_add(units, "desc", buf);
    if ((ctx->units = units) == NULL)
        return 1;
    return 0;
}

// TODO: add support to generate desk name
static int _desk_add(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char c;
    int status;
    char *desc = NULL;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot add desk '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":e:hnqD:N")) != -1) {
        switch (c) {
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
        case 'D':
            desc = optarg;
            break;
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-add");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-add");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("desk-add");
    else if (optind == argvec->used)
        return TEC_LOG_E("desk name required");
    else if (tec_cli_check_env(&args, errfmt, opts.quiet))
        return EXIT_FAILURE;

    do {
        args.desk = argvec->argv[argvec->i];

        if (tec_cli_len_valid(args.desk, DESKSIZ) == false) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, "desk name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_desk_valid(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_desk_exist(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = generate_units(&ctx, args.desk, desc))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, "unit generation failed");
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_desk_add(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
            ctx.units = tec_unit_free(ctx.units);
        } else if ((status = hook_action(&args, "desk-add"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opts.change_tog == true) {
            if ((status = toggle_desk_set_curr(cfg->base.task, &args))) {
                if (opts.quiet == false)
                    TEC_LOG_E("could not update toggles");
            }
        }
        ctx.units = tec_unit_free(ctx.units);
        retcode = status == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir)
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;
    return retcode;
}

static int _desk_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int ntasks;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_rm_options opts;
    const char *errfmt = "cannot remove desk '%s': %s";

    tec_cli_rm_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":e:fhiqvI")) != -1) {
        switch (c) {
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
            return tec_cli_help_usage("desk-rm");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-rm");
        }
    }
    argvec->i = optind;
    ntasks = argvec->used - argvec->i;

    if (opts.help)
        return tec_cli_help_usage("desk-rm");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return status;

    if (ntasks > 3 && opts.mode == RMI_SOMETIMES) {
        TEC_LOG_P("remove %d desks? [y/N] ", ntasks);
        if (!tec_aux_yesno()) {
            return EXIT_SUCCESS;
        }
    }

    do {
        args.desk = argvec->argv[argvec->i];

        if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        } else if (opts.mode == RMI_ALWAYS) {
            TEC_LOG_P("remove desk '%s'? [y/N] ", args.task);
            if (!tec_aux_yesno())
                continue;
        }
        // TODO: update current directory if current env got deleted.
        if ((status = hook_action(&args, "desk-rm"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, "failed to execute hooks");
        } else if ((status = tec_desk_rm(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
        } else if (opts.verbose == true)
            TEC_LOG_I("removed desk '%s'", args.task);
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir) {
        args.desk = NULL;       /* Force to get current task ID.  */
        if (toggle_desk_get_curr(cfg->base.task, &args))
            args.desk = "";
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;
    }
    return retcode == TEC_OK ? tec_cli_pwd_set(&args) : retcode;
}

// TODO: show tasks in desk
static int _desk_ls(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char *desc = NULL;
    int c, i, status;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    int opt_help, opt_quiet;
    const char *errfmt = "cannot list desk(s) '%s': %s";

    opt_help = opt_quiet = false;

    while ((c = getopt(argvec->used, argvec->argv, ":e:hq")) != -1) {
        switch (c) {
        case 'q':
            opt_quiet = true;
            break;
        case 'h':
            opt_help = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-ls");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-ls");
        }
    }

    if (opt_help == true)
        return tec_cli_help_usage("desk-ls");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    i = optind;
    do {
        args.env = argvec->argv[i];

        if (tec_cli_check_env(&args, errfmt, opt_quiet))
            continue;
        if ((status = tec_desk_list(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
            continue;
        }

        for (tec_list_t * obj = ctx.list; obj != NULL; obj = obj->next) {
            args.desk = obj->name;
            if ((desc = get_unit_desc(&ctx, &args, opt_quiet, cfg)) == NULL) {
                continue;
            }
            LIST_OBJ_UNITS(obj->name, "", desc, DESKSIZ, teccfg.opts.color);
            ctx.units = tec_unit_free(ctx.units);
        }
        ctx.list = tec_list_free(ctx.list);
    } while (++i < argvec->used);

    return status;
}

static int _desk_mv(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    (void)argvec;
    (void)cfg;
    return TEC_LOG_E("%s: under development", __func__);
}

static int _desk_set(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_set_options opts;
    const char *errfmt = "cannot set desk unit value '%s': %s";

    tec_cli_set_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":e:hqD:")) != -1) {
        switch (c) {
        case 'h':
            opts.help = true;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'D':
            if (tec_aux_is_valid_desc(optarg) == false) {
                TEC_LOG_E("invalid description '%s'", optarg);
                return tec_cli_help_usage("desk-set");
            }
            ctx.units = tec_unit_add(ctx.units, "desc", optarg);
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-set");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-set");
        }
    }
    argvec->i = optind;

    if (opts.help)
        return tec_cli_help_usage("desk-set");

    if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return status;

    do {
        args.desk = argvec->argv[argvec->i];

        if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_desk_set(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
        } else if ((status = hook_action(&args, "desk-set"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }

        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    ctx.units = tec_unit_free(ctx.units);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _desk_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    tec_argvec_t keys;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_unit *units = NULL;
    struct tec_unit *unitpgn = NULL;
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot show env units '%s': %s";
    const char *unitfmt = "%-" xstr(PADDING_UNIT) "s : %s\n";
    const char *keyfmt = "cannot show units '%s': '%s' no such key";

    argvec_init(&keys);
    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":e:hk:q")) != -1) {
        switch (c) {
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
            return tec_cli_help_usage("desk-cat");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-cat");
        }
    }
    argvec->i = optind;

    if (opts.help) {
        retcode = TEC_OK;
        tec_cli_help_usage("desk-cat");
        goto err;
    } else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet))) {
        retcode = EXIT_FAILURE;
        goto err;
    }

    do {
        args.desk = argvec->argv[argvec->i];

        if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }
        // TODO: validate unit keys and values
        if ((status = tec_desk_get(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
        } else if ((status = hook_cat(&unitpgn, &args, "desk-cat"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.desk, "failed to execute hooks");
        }
        retcode = status == TEC_OK ? retcode : status;

        if (retcode == TEC_OK) {
            units = tec_unit_add(units, "id", args.desk);
            units = tec_unit_join(units, ctx.units);
            units = tec_unit_join(units, unitpgn);

            /* Show all keys.  */
            if (argvec_is_empty(&keys) == true) {
                for (tec_unit_t * tmp = units; tmp; tmp = tmp->next)
                    printf(unitfmt, tmp->key, tmp->val);
            } else {            /* Show specific keys only.  */
                for (int i = 0; i < keys.used; i++) {
                    status = aux_show_key(keys.argv[i], units);
                    if (status && opts.quiet == false)
                        TEC_LOG_E(keyfmt, args.env, keys.argv[i]);
                    retcode = status == EXIT_SUCCESS ? retcode : EXIT_FAILURE;
                }
            }
        }

        units = ctx.units = unitpgn = tec_unit_free(units);
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

 err:
    argvec_deinit(&keys);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _desk_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = TEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot switch to '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":e:hnqN")) != -1) {
        switch (c) {
        case 'h':
            opts.help = true;
            break;
        case 'n':
            opts.change_tog = false;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-cd");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-cd");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("desk-cd");
    else if ((status = tec_cli_check_env(&args, errfmt, opts.quiet)))
        return status;
    else if (!tec_aux_check_cd_alias(argvec))
        return TEC_LOG_E("alias '-' is used alone");

    /* Resolve alias '-' to switch to previous environment.  */
    if (argvec->argv[argvec->i] && strcmp("-", argvec->argv[argvec->i]) == 0) {
        if ((status = toggle_desk_get_prev(cfg->base.task, &args)))
            return TEC_LOG_E(errfmt, "PREV", "no previous desk");
        argvec_replace(argvec, argvec->i, args.desk);
    }

    do {
        args.desk = argvec->argv[argvec->i];

        if ((status = tec_cli_check_desk(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = hook_action(&args, "desk-cd"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opts.change_tog == true) {
            if ((status = toggle_desk_set_curr(cfg->base.task, &args))) {
                if (opts.quiet == false)
                    TEC_LOG_E("could not update toggles");
            }
        }
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static const tec_cmd_t desk_commands[] = {
    {.name = "add",.func = &_desk_add},
    {.name = "cat",.func = &_desk_cat},
    {.name = "cd",.func = &_desk_cd},
    {.name = "ls",.func = &_desk_ls},
    {.name = "mv",.func = &_desk_mv},
    {.name = "rm",.func = &_desk_rm},
    {.name = "set",.func = &_desk_set},
};

int tec_cli_desk(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    const char *cmd = argvec->argv[1] != NULL ? argvec->argv[1] : "ls";

    argvec_offset(argvec, 1);   /* Skip desk from argvec.  */
    for (size_t i = 0; i < ARRAY_SIZE(desk_commands); ++i) {
        if (strcmp(cmd, desk_commands[i].name) == 0) {
            return desk_commands[i].func(argvec, cfg);
        }
    }
    return TEC_LOG_E("'%s': no such desk subcommand", cmd);
}
