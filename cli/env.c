#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "tec.h"
#include "aux/aux.h"
#include "aux/opts.h"
#include "aux/argvec.h"
#include "aux/config.h"
#include "aux/toggle.h"

// TODO: remove parameter 'quiet', return status, and let the caller to
static char *get_unit_desc(tec_ctx_t *ctx, tec_arg_t *args, int quiet)
{
    int status;
    char *desc = NULL;

    if ((status = tec_env_get(teccfg.base.task, args, ctx))) {
        if (quiet == false)
            TEC_LOG_E("'%s': %s", args->env, tec_strerror(status));
    } else if ((desc = tec_unit_get(ctx->units, "desc")) == NULL) {
        if (quiet == false)
            TEC_LOG_E("'%s': %s", args->env, "description not found");
    }
    return desc;
}

static int generate_units(tec_ctx_t *ctx, char *env)
{
    struct tec_unit *units = NULL;
    char desc[100] = "Generated desciption for environment ";

    strcat(desc, env);
    units = tec_unit_add(units, "desc", desc);

    if ((ctx->units = units) == NULL)
        return 1;
    return 0;
}

static int valid_desc(const char *val)
{
    if (!isalnum(*val++))
        return false;
    for (; *val; ++val)
        if (!(isalnum(*val) || isspace(*val) || *val == '_' || *val == '-'))
            return false;
    return isalnum(*--val) != 0;
}

static int _env_add(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char c;
    int status = TEC_OK;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot add env '%s': %s";
    const char *errfmt_desk = "cannot add desk '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:hnqD:N")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
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
            return TEC_LOG_E("'%c': this option is not implemented yet", c);
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-add");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-add");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("env-add");
    else if (optind == argvec->used)
        return TEC_LOG_E("env name required");

    /* Set default desk name to create.  */
    if (args.desk == NULL)
        args.desk = "desk";

    for (int i = argvec->i; i < argvec->used; ++i) {
        args.env = argvec->argv[i];

        if (tec_cli_len_valid(args.env, ENVSIZ) == false) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, "env name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_env_valid(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_env_exist(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_desk_valid(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (tec_cli_len_valid(args.desk, DESKSIZ) == false) {
            status = 1;
            if (opts.quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, "desk name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_desk_exist(cfg->base.task, &args))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        }

        if (generate_units(&ctx, args.env)) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, "unit generation failed");
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_env_add(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_desk_add(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        }
        ctx.units = tec_unit_free(ctx.units);
    }

    if ((opts.change_tog && status == TEC_OK)
        && toggle_env_set_curr(cfg->base.task, &args)) {
        if (opts.quiet == false)
            TEC_LOG_E("could not update env toggles");
        return 1;
    } else if ((opts.change_tog && status == TEC_OK)
               && toggle_desk_set_curr(cfg->base.task, &args)) {
        if (opts.quiet == false)
            TEC_LOG_E("could not update desk toggles");
        return 1;
    }

    if (retcode == TEC_OK && opts.change_dir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _env_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int ntasks;
    int status = TEC_OK;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_rm_options opts;
    const char *errfmt = "cannot remove env '%s': %s";

    tec_cli_rm_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:fhiqvI")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
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
            return tec_cli_help_usage("env-rm");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-rm");
        }
    }
    argvec->i = optind;
    ntasks = argvec->used - argvec->i;

    if (opts.help)
        return tec_cli_help_usage("env-rm");
    else if (ntasks > 3 && opts.mode == RMI_SOMETIMES) {
        TEC_LOG_P("remove %d environments? [y/N] ", ntasks);
        if (!tec_aux_yesno()) {
            return EXIT_SUCCESS;
        }
    }

    do {
        args.env = argvec->argv[argvec->i];

        if (tec_cli_check_env(&args, errfmt, opts.quiet)) {
            retcode = EXIT_FAILURE;
            continue;
        } else if (opts.mode == RMI_ALWAYS) {
            TEC_LOG_P("remove environment '%s'? [y/N] ", args.env);
            if (!tec_aux_yesno())
                continue;
        }

        if ((status = hook_action(&args, "env-rm"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, "failed to execute hooks");
        } else if ((status = tec_env_rm(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(status));
        }
        // TODO: update current directory if current env got deleted.

        if (opts.verbose == true)
            TEC_LOG_I("removed environment '%s'", args.env);
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    if (retcode == TEC_OK && opts.change_dir) {
        args.env = NULL;        /* Force to get current environment.  */
        if (toggle_env_get_curr(cfg->base.task, &args))
            args.env = "";
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;
    }
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _env_ls(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    char *desc = NULL;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    int opt_help, opt_quiet;

    opt_quiet = false;
    opt_help = false;

    while ((c = getopt(argvec->used, argvec->argv, ":hq")) != -1) {
        switch (c) {
        case 'q':
            opt_quiet = true;
            break;
        case 'h':
            opt_help = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-ls");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-ls");
        }
    }

    if (opt_help == true)
        return tec_cli_help_usage("env-ls");

    if ((retcode = tec_env_list(cfg->base.task, &args, &ctx))) {
        if (opt_quiet == false) {
            const char *errfmt = "cannot list env(s) '%s': %s";
            TEC_LOG_E(errfmt, "ENV", tec_strerror(retcode));
            return retcode;
        }
    }

    for (tec_list_t * obj = ctx.list; obj != NULL; obj = obj->next) {
        args.env = obj->name;
        if ((desc = get_unit_desc(&ctx, &args, opt_quiet)) == NULL) {
            continue;
        }
        LIST_OBJ_UNITS(obj->name, "", desc, ENVSIZ, teccfg.opts.color);
        ctx.units = tec_unit_free(ctx.units);
    }

    ctx.list = tec_list_free(ctx.list);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _env_rename(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    tec_arg_t src = ARGS_INIT();
    tec_arg_t dst = ARGS_INIT();
    struct tec_cli_rm_options opts;
    const char *errfmt = "could not rename env '%s': %s";

    tec_cli_rm_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":hq")) != -1) {
        switch (c) {
        case 'h':
            opts.help = true;
            break;
        case 'q':
            opts.quiet = true;
            break;
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-rename");
        }
    }

    if (opts.help)
        return tec_cli_help_usage("env-rename");
    else if (argvec->used - optind != 2)
        return TEC_LOG_E("source or destination env name missing");

    src.env = argvec->argv[optind];
    dst.env = argvec->argv[optind + 1];

    if ((status = tec_cli_check_env(&src, errfmt, opts.quiet)))
        return status;
    else if ((status = tec_cli_check_env(&src, errfmt, opts.quiet)))
        return status;
    else if (!tec_env_exist(cfg->base.task, &dst)) {
        if (opts.quiet == false)
            TEC_LOG_E(errfmt, dst.env, "destination env already exists");
        return status;
    }

    if ((status = tec_env_rename(cfg->base.task, &src, &dst, NULL))) {
        if (opts.quiet == false)
            TEC_LOG_E(errfmt, "ENV", tec_strerror(status));
        return status;
    } else if ((status = hook_action(&dst, "env-rename"))) {
        if (opts.quiet == false)
            TEC_LOG_E(errfmt, dst.env, "failed to execute hooks");
        return status;
    }
    return tec_cli_pwd_set(&dst);
}

static int _env_set(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = TEC_OK;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_set_options opts;
    const char *errfmt = "could not set env unit value '%s': %s";

    tec_cli_set_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:hqD:")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'h':
            opts.help = true;
            break;
        case 'q':
            opts.quiet = true;
            break;
        case 'D':
            if (valid_desc(optarg) == false) {
                TEC_LOG_E("invalid description '%s'", optarg);
                return tec_cli_help_usage("env-set");
            }
            ctx.units = tec_unit_add(ctx.units, "desc", optarg);
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-set");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-set");
        }
    }
    argvec->i = optind;

    if (opts.help)
        return tec_cli_help_usage("env-set");

    do {
        args.env = argvec->argv[argvec->i];

        if ((status = tec_cli_check_env(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_env_set(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(status));
        } else if ((status = hook_action(&args, "env-set"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }
        retcode = status == TEC_OK ? retcode : status;
    } while (++argvec->i < argvec->used);

    ctx.units = tec_unit_free(ctx.units);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _env_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    tec_argvec_t keys;
    int retcode = TEC_OK;
    tec_unit_t *units = NULL;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    struct tec_unit *unitpgn = NULL;
    const char *errfmt = "cannot show env units '%s': %s";
    const char *unitfmt = "%-" xstr(PADDING_UNIT) "s : %s\n";
    const char *keyfmt = "cannot show units '%s': '%s' no such key";

    argvec_init(&keys);
    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:hk:q")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
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
            return tec_cli_help_usage("env-cat");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-cat");
        }
    }
    argvec->i = optind;

    if (opts.help) {
        retcode = TEC_OK;
        tec_cli_help_usage("env-cat");
        goto err;
    }

    do {
        args.env = argvec->argv[argvec->i];

        if ((status = tec_cli_check_env(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }
        // TODO: validate unit keys and values
        if ((status = tec_env_get(cfg->base.task, &args, &ctx))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(status));
            continue;
        } else if ((status = hook_cat(&unitpgn, &args, "env-cat"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }
        retcode = status == TEC_OK ? retcode : status;

        if (retcode == TEC_OK) {
            units = tec_unit_add(units, "id", args.env);
            units = tec_unit_join(units, ctx.units);
            units = tec_unit_join(units, unitpgn);

            /* Show all keys.  */
            if (argvec_is_empty(&keys) == true) {
                for (tec_unit_t * tmp = units; tmp; tmp = tmp->next)
                    printf(unitfmt, tmp->key, tmp->val);
            } else {            /* Show specific keys only.  */
                for (int i = 0; i < keys.used; i++) {
                    int notfound = false;
                    for (tec_unit_t * tmp = units; tmp; tmp = tmp->next) {
                        if ((notfound = strcmp(keys.argv[i], tmp->key)) == 0) {
                            printf("%s\n", tmp->val);
                            break;
                        }
                    }
                    if (notfound && opts.quiet == false)
                        TEC_LOG_E(keyfmt, args.env, keys.argv[i]);
                    retcode = notfound == 0 ? retcode : 1;
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

static int _env_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    int status;
    int retcode = TEC_OK;
    tec_arg_t args = ARGS_INIT();
    struct tec_cli_cd_options opts;
    const char *errfmt = "cannot switch to '%s': %s";

    tec_cli_cd_option_init(&opts);
    while ((c = getopt(argvec->used, argvec->argv, ":d:hnqN")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
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
        case 'N':
            opts.change_dir = false;
            opts.change_tog = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-cd");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-cd");
        }
    }
    argvec->i = optind;

    if (opts.help == true)
        return tec_cli_help_usage("env-cd");
    else if (!tec_aux_check_cd_alias(argvec))
        return TEC_LOG_E("alias '-' is used alone");

    /* Resolve alias '-' to switch to previous environment.  */
    if (argvec->argv[argvec->i] && strcmp("-", argvec->argv[argvec->i]) == 0) {
        if ((status = toggle_env_get_prev(cfg->base.task, &args)))
            return TEC_LOG_E(errfmt, "PREV", "no previous environment");
        argvec_replace(argvec, argvec->i, args.env, ENVSIZ);
    }

    do {
        args.env = argvec->argv[argvec->i];

        if ((status = tec_cli_check_env(&args, errfmt, opts.quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = hook_action(&args, "env-cd"))) {
            if (opts.quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opts.change_tog == true) {
            if ((status = toggle_env_set_curr(cfg->base.task, &args))) {
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

static const tec_cmd_t env_commands[] = {
    {.name = "add",.func = &_env_add},
    {.name = "cat",.func = &_env_cat},
    {.name = "cd",.func = &_env_cd},
    {.name = "ls",.func = &_env_ls},
    {.name = "rename",.func = &_env_rename},
    {.name = "rm",.func = &_env_rm},
    {.name = "set",.func = &_env_set},
};

int tec_cli_env(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    const char *cmd = argvec->argv[1] != NULL ? argvec->argv[1] : "ls";

    argvec_offset(argvec, 1);   /* Skip env from argvec.  */
    for (size_t i = 0; i < ARRAY_SIZE(env_commands); ++i) {
        if (strcmp(cmd, env_commands[i].name) == 0) {
            return env_commands[i].func(argvec, cfg);
        }
    }
    return TEC_LOG_E("'%s': no such env command", cmd);
}
