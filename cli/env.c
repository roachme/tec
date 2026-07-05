#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "tec.h"
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

// TODO: Find a good error message in case option fails.  */
static int _env_add(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char c;
    tec_arg_t args;
    int switch_dir, switch_env;
    int i, quiet, showhelp, retcode, status;
    tec_ctx_t ctx = CTX_INIT();
    const char *errfmt = "cannot add env '%s': %s";
    const char *errfmt_desk = "cannot add desk '%s': %s";

    retcode = TEC_OK;
    status = TEC_OK;
    showhelp = quiet = false;
    switch_dir = switch_env = true;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:hnqN")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'h':
            showhelp = true;
            break;
        case 'n':
            switch_env = false;
            break;
        case 'q':
            quiet = true;
            break;
        case 'N':
            switch_dir = false;
            switch_env = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-add");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-add");
        }
    }

    if (showhelp)
        return tec_cli_help_usage("env-add");
    if (optind == argvec->used) {
        return TEC_LOG_E("env name required");
    }

    /* Set default desk name to create.  */
    if (args.desk == NULL)
        args.desk = "desk";

    for (i = optind; i < argvec->used; ++i) {
        args.env = argvec->argv[i];

        if (tec_cli_len_valid(args.env, ENVSIZ) == false) {
            if (quiet == false)
                TEC_LOG_E(errfmt, args.env, "env name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_env_valid(cfg->base.task, &args))) {
            if (quiet == false)
                TEC_LOG_E(errfmt, args.env, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_env_exist(cfg->base.task, &args))) {
            char *env = args.env;
            if (quiet == false)
                TEC_LOG_E(errfmt, env, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_desk_valid(cfg->base.task, &args))) {
            if (quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (tec_cli_len_valid(args.desk, DESKSIZ) == false) {
            status = 1;
            if (quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, "desk name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if (!(status = tec_desk_exist(cfg->base.task, &args))) {
            if (quiet == false)
                TEC_LOG_E(errfmt_desk, args.desk, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        }

        if (generate_units(&ctx, args.env)) {
            if (quiet == false)
                TEC_LOG_E(errfmt, args.env, "unit generation failed");
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_env_add(cfg->base.task, &args, &ctx))) {
            if (quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if ((status = tec_desk_add(cfg->base.task, &args, &ctx))) {
            if (quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        }
        ctx.units = tec_unit_free(ctx.units);
    }

    if ((switch_env && status == TEC_OK)
        && toggle_env_set_curr(cfg->base.task, &args)) {
        if (quiet == false)
            TEC_LOG_E("could not update env toggles");
        return 1;
    } else if ((switch_env && status == TEC_OK)
               && toggle_desk_set_curr(cfg->base.task, &args)) {
        if (quiet == false)
            TEC_LOG_E("could not update desk toggles");
        return 1;
    }

    if (retcode == TEC_OK && switch_dir)
        retcode = tec_cli_pwd_set(&args);
    return retcode == TEC_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int _env_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args;
    int i, c, retcode, status;
    const char *errfmt = "cannot delete env '%s': %s";
    int opt_ask_once, opt_ask_every, opt_quiet, opt_help, opt_verbose;

    retcode = TEC_OK;
    opt_ask_every = true;       /* prompt before every removal.  */
    opt_ask_once = false;       /* prompt before once for all environments.  */
    opt_quiet = opt_help = opt_verbose = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:fhiqvI")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'f':
            opt_ask_every = false;
            opt_ask_once = false;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'i':
            opt_ask_every = true;
            opt_ask_once = false;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'v':
            opt_verbose = true;
            break;
        case 'I':
            opt_ask_every = false;
            opt_ask_once = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-rm");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-rm");
        }
    }
    i = optind;

    if (opt_help)
        return tec_cli_help_usage("env-rm");

    if (opt_ask_once == true) {
        printf("Are you sure to remove environment(s)? [y/N] ");
        if (tec_aux_yesno() == false) {
            return TEC_OK;
        }
    }

    do {
        args.env = argvec->argv[i];
        if (tec_cli_check_env(&args, errfmt, opt_quiet)) {
            retcode = EXIT_FAILURE;
            continue;
        } else if (opt_ask_every == true) {
            printf("Are you sure to remove environment '%s'? [y/N] ", args.env);
            if (tec_aux_yesno() == false) {
                continue;
            }
        }

        if ((status = hook_action(&args, "env-rm"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.env, "failed to execute hooks");
        } else if ((status = tec_env_rm(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
        }

        if (opt_verbose == true)
            TEC_LOG_I("removed environment '%s'", args.env);
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    // TODO: update current directory if current env got deleted.

    if (retcode == TEC_OK) {
        status = tec_cli_pwd_set(&args);
        retcode = status == TEC_OK ? retcode : status;
    }
    return retcode;
}

static int _env_ls(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int c;
    char *desc = NULL;
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args;
    int status = TEC_OK;
    int opt_help, opt_quiet;

    opt_quiet = false;
    opt_help = false;
    args.env = args.desk = args.task = NULL;

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

    if ((status = tec_env_list(cfg->base.task, &args, &ctx))) {
        if (opt_quiet == false) {
            const char *errfmt = "cannot list env(s) '%s': %s";
            TEC_LOG_E(errfmt, "ENV", tec_strerror(status));
            return status;
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
    return status;
}

static int _env_rename(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int status;
    const char *errfmt;
    tec_arg_t src, dst;
    int c, opt_quiet, opt_help;

    opt_quiet = opt_help = false;
    errfmt = "could not rename env '%s': %s";
    src.env = src.desk = src.task = NULL;
    dst.env = dst.desk = dst.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":hq")) != -1) {
        switch (c) {
        case 'h':
            opt_help = true;
            break;
        case 'q':
            opt_quiet = true;
            break;
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-rename");
        }
    }

    if (opt_help)
        return tec_cli_help_usage("env-rename");

    if (argvec->used - optind != 2)
        return TEC_LOG_E("source or destination env name missing");

    src.env = argvec->argv[optind];
    dst.env = argvec->argv[optind + 1];

    /* TODO: trigger hooks if any */

    if ((status = tec_cli_check_env(&src, errfmt, opt_quiet)))
        return status;
    else if ((status = tec_cli_check_env(&src, errfmt, opt_quiet)))
        return status;
    else if (!tec_env_exist(cfg->base.task, &dst)) {
        if (opt_quiet == false)
            TEC_LOG_E(errfmt, src.env, "such destination env exists");
        return status;
    }

    if ((status = tec_env_rename(cfg->base.task, &src, &dst, NULL))) {
        if (opt_quiet == false)
            TEC_LOG_E(errfmt, "ENV", tec_strerror(status));
        return status;
    }
    // FIXME:TODO: free argvec
    return tec_cli_pwd_set(&dst);
}

static int _env_set(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args;
    int atleast_one_key_set;
    int c, i, retcode, status;
    int opt_quiet, opt_help;
    const char *errfmt = "could not set env unit value '%s': %s";

    retcode = TEC_OK;
    opt_quiet = opt_help = false;
    atleast_one_key_set = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:hqD:")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'D':
            if (valid_desc(optarg) == false) {
                TEC_LOG_E("invalid description '%s'", optarg);
                return tec_cli_help_usage("env-set");
            }
            atleast_one_key_set = true;
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

    if (opt_help)
        return tec_cli_help_usage("env-set");
    else if (atleast_one_key_set == false) {
        TEC_LOG_E("gotta supply one of the options");
        return tec_cli_help_usage("env-set");
    }

    i = optind;
    do {
        args.env = argvec->argv[i];

        if ((status = tec_cli_check_env(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = tec_env_set(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
        } else if ((status = hook_action(&args, "env-set"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    ctx.units = tec_unit_free(ctx.units);
    return status;
}

static int _env_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT();
    tec_arg_t args;
    int c, i, quiet, showhelp, status;
    struct tec_unit *unitbin, *unitpgn;
    const char *errfmt = "cannot show env units '%s': %s";

    status = TEC_OK;
    unitbin = unitpgn = NULL;
    quiet = showhelp = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:hq")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'h':
            showhelp = true;
            break;
        case 'q':
            quiet = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-cat");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-cat");
        }
    }

    if (showhelp)
        return tec_cli_help_usage("env-cat");

    i = optind;

    do {
        args.env = argvec->argv[i];
        if ((status = tec_cli_check_env(&args, errfmt, quiet))) {
            continue;
        } else if ((status = tec_env_get(cfg->base.task, &args, &ctx))) {
            if (quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            continue;
        }

        printf("%-7s : %s\n", "env", args.env);
        for (unitbin = ctx.units; unitbin; unitbin = unitbin->next)
            printf("%-7s : %s\n", unitbin->key, unitbin->val);

        // TODO: add plugin output
        ctx.units = tec_unit_free(ctx.units);
    } while (++i < argvec->used);

    return status;
}

static int _env_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_arg_t args;
    int c, i, retcode, status;
    const char *errfmt = "cannot switch to '%s': %s";
    int opt_quiet, opt_help, opt_cd_toggle, opt_cd_dir;

    retcode = TEC_OK;
    opt_quiet = opt_help = false;
    opt_cd_toggle = opt_cd_dir = true;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:hnqN")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'n':
            opt_cd_toggle = false;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'N':
            opt_cd_dir = false;
            opt_cd_toggle = false;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("env-cd");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("env-cd");
        }
    }

    i = optind;

    if (opt_help == true)
        return tec_cli_help_usage("env-cd");

    /* Check that alias '-' is not passed with other envs nor duplicated.  */
    for (int idx = i; idx < argvec->used; ++idx) {
        if (strcmp(argvec->argv[idx], "-") == 0 && argvec->used - i > 1)
            return TEC_LOG_E("alias '-' is used alone");
    }

    /* Resolve alias '-' to switch to previous environment.  */
    if (argvec->argv[i] && strcmp("-", argvec->argv[i]) == 0) {
        if ((status = toggle_env_get_prev(cfg->base.task, &args)))
            return TEC_LOG_E(errfmt, "PREV", "no previous environment");
        argvec_replace(argvec, i, args.env, ENVSIZ);
    }

    do {
        args.env = argvec->argv[i];
        if ((status = tec_cli_check_env(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = hook_action(&args, "env-cd"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opt_cd_toggle == true) {
            if ((status = toggle_env_set_curr(cfg->base.task, &args))) {
                if (opt_quiet == false)
                    TEC_LOG_E("could not update toggles");
            }
        }
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    if (retcode == TEC_OK && opt_cd_dir)
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;

    return retcode;
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
