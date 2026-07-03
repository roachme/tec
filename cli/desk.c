#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "tec.h"
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

static int valid_desc(const char *val)
{
    if (!isalnum(*val++))
        return false;
    for (; *val; ++val)
        if (!(isalnum(*val) || isspace(*val) || *val == '_' || *val == '-'))
            return false;
    return isalnum(*--val) != 0;
}

static int generate_units(tec_ctx_t *ctx, char *desk)
{
    struct tec_unit *units = NULL;
    char desc[100] = "Generated desciption for desk ";

    strcat(desc, desk);
    units = tec_unit_add(units, "desc", desc);

    if ((ctx->units = units) == NULL)
        return 1;
    return 0;
}

// TODO: add support to generate desk name
static int _desk_add(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char c;
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int i, retcode, status;
    const char *errfmt = "cannot add desk '%s': %s";
    int opt_quiet, opt_help, opt_cd_dir, opt_cd_toggle;

    retcode = TEC_OK;
    opt_help = opt_quiet = false;
    opt_cd_dir = opt_cd_toggle = true;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":e:hnqN")) != -1) {
        switch (c) {
        case 'e':
            args.env = optarg;
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
            return tec_cli_help_usage("desk-add");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-add");
        }
    }
    i = optind;

    if (opt_help)
        return tec_cli_help_usage("desk-add");

    if (optind == argvec->used)
        return TEC_LOG_E("desk name required");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    do {
        args.desk = argvec->argv[i];

        if (tec_cli_len_valid(args.desk, DESKSIZ) == false) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.desk, "desk name is too long");
            retcode = EXIT_FAILURE;
            continue;
        } else if (tec_desk_valid(cfg->base.task, &args)) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.desk, tec_strerror(status));
            retcode = EXIT_FAILURE;
            continue;
        } else if (!tec_desk_exist(cfg->base.task, &args)) {
            char *desk = args.desk;
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, desk, tec_strerror(TEC_ARG_EXISTS));
            retcode = EXIT_FAILURE;
            continue;
        } else if (generate_units(&ctx, args.desk)) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.desk, "unit generation failed");
            retcode = EXIT_FAILURE;
            continue;
        }

        if ((status = tec_desk_add(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            ctx.units = tec_unit_free(ctx.units);
        } else if ((status = hook_action(&args, "desk-add"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opt_cd_toggle == true) {
            if ((status = toggle_desk_set_curr(cfg->base.task, &args))) {
                if (opt_quiet == false)
                    TEC_LOG_E("could not update toggles");
            }
        }
        ctx.units = tec_unit_free(ctx.units);
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    if (retcode == TEC_OK && opt_cd_dir)
        retcode = tec_cli_pwd_set(&args) == TEC_OK ? retcode : status;

    return retcode;
}

static int _desk_rm(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int c, i, retcode, status;
    int opt_quiet, opt_help;
    const char *errfmt = "cannot remove desk '%s': %s";
    int opt_ask_once, opt_ask_every, opt_verbose;

    retcode = TEC_OK;
    opt_ask_every = true;       /* prompt before every removal.  */
    opt_ask_once = false;       /* prompt before once for all desks.  */
    args.env = args.desk = args.task = NULL;
    opt_quiet = opt_help = opt_verbose = false;

    while ((c = getopt(argvec->used, argvec->argv, ":fhiqvI")) != -1) {
        switch (c) {
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
            opt_verbose = false;
            break;
        case 'I':
            opt_ask_every = false;
            opt_ask_once = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-rm");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-rm");
        }
    }
    i = optind;

    if (opt_help)
        return tec_cli_help_usage("desk-rm");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    if (opt_ask_once == true) {
        printf("Are you sure to remove desk(s)? [y/N] ");
        if (tec_aux_yesno() == false) {
            return TEC_OK;
        }
    }

    do {
        args.desk = argvec->argv[i];

        if ((status = tec_cli_check_desk(&args, errfmt, opt_quiet))) {
            retcode = EXIT_FAILURE;
            continue;
        } else if (opt_ask_every == true) {
            printf("Are you sure to remove desk '%s'? [y/N] ", args.desk);
            if (tec_aux_yesno() == false) {
                continue;
            }
        }

        if ((status = hook_action(&args, "desk-rm"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.desk, "failed to execute hooks");
        } else if ((status = tec_desk_rm(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
        }
        /* TODO: handle current and previos task IDs.  */

        if (opt_verbose == true)
            TEC_LOG_I("removed desk '%s'", args.task);
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    // TODO: update current directory if current env got deleted.
    return retcode == TEC_OK ? tec_cli_pwd_set(&args) : retcode;
}

// TODO: show tasks in desk
static int _desk_ls(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    char *desc = NULL;
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int c, i, status;
    int opt_help, opt_quiet;
    const char *errfmt = "cannot list desk(s) '%s': %s";

    opt_help = opt_quiet = false;
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
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int atleast_one_key_set;
    int opt_quiet, opt_help;
    int c, i, retcode, status;
    const char *errfmt = "could not set desk unit value '%s': %s";

    retcode = TEC_OK;
    opt_quiet = opt_help = false;
    atleast_one_key_set = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":hqD:")) != -1) {
        switch (c) {
        case 'h':
            opt_help = true;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'D':
            if (valid_desc(optarg) == false) {
                TEC_LOG_E("invalid description '%s'", optarg);
                return tec_cli_help_usage("desk-set");
            }
            atleast_one_key_set = true;
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

    if (opt_help)
        return tec_cli_help_usage("desk-set");
    if (atleast_one_key_set == false) {
        TEC_LOG_E("gotta supply one of the options");
        return tec_cli_help_usage("desk-set");
    }

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    i = optind;
    do {
        args.desk = argvec->argv[i];

        if ((status = tec_cli_check_desk(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = tec_desk_set(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
        } else if ((status = hook_action(&args, "desk-set"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        }

        ctx.units = tec_unit_free(ctx.units);
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    return retcode;
}

static int _desk_cat(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int opt_quiet, opt_help;
    int c, i, retcode, status;
    struct tec_unit *units, *unitpgn;
    const char *errfmt = "cannot show env units '%s': %s";

    retcode = TEC_OK;
    units = unitpgn = NULL;
    opt_quiet = opt_help = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":hq")) != -1) {
        switch (c) {
        case 'h':
            opt_help = true;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case ':':
            TEC_LOG_E(FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("desk-cat");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-cat");
        }
    }
    i = optind;

    if (opt_help)
        return tec_cli_help_usage("desk-cat");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    do {
        args.desk = argvec->argv[i];
        if ((status = tec_cli_check_desk(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = tec_desk_get(cfg->base.task, &args, &ctx))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, argvec->argv[i], tec_strerror(status));
            ;
        } else {
            printf("%-7s : %s\n", "desk", args.desk);
            for (units = ctx.units; units; units = units->next)
                printf("%-7s : %s\n", units->key, units->val);

            // TODO: add plugin output
        }

        unitpgn = tec_unit_free(unitpgn);
        ctx.units = tec_unit_free(ctx.units);
        retcode = status == TEC_OK ? retcode : status;
    } while (++i < argvec->used);

    return retcode;
}

static int _desk_cd(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_arg_t args;
    int c, i, retcode, status;
    const char *errfmt = "cannot switch to '%s': %s";
    int opt_cd_dir, opt_cd_toggle, opt_help, opt_quiet;

    retcode = TEC_OK;
    opt_quiet = opt_help = false;
    opt_cd_toggle = opt_cd_dir = true;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":e:hnqN")) != -1) {
        switch (c) {
        case 'h':
            opt_help = true;
            break;
        case 'n':
            opt_cd_toggle = false;
            break;
        case 'e':
            args.env = optarg;
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
            return tec_cli_help_usage("desk-cd");
        default:
            TEC_LOG_E(FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("desk-cd");
        }
    }

    i = optind;

    if (opt_help == true)
        return tec_cli_help_usage("desk-cd");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;

    /* Check that alias '-' is not passed with other desks nor duplicated.  */
    for (int idx = i; idx < argvec->used; ++idx) {
        if (strcmp(argvec->argv[idx], "-") == 0 && argvec->used - i > 1)
            return TEC_LOG_E("alias '-' is used alone");
    }

    /* Resolve alias '-' to switch to previous environment.  */
    if (argvec->argv[i] && strcmp("-", argvec->argv[i]) == 0) {
        if ((status = toggle_desk_get_prev(cfg->base.task, &args)))
            return TEC_LOG_E(errfmt, "PREV", "no previous desk");
        argvec_replace(argvec, i, args.desk, DESKSIZ);
    }

    do {
        args.desk = argvec->argv[i];

        if ((status = tec_cli_check_desk(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = hook_action(&args, "desk-cd"))) {
            if (opt_quiet == false)
                TEC_LOG_E(errfmt, args.task, "failed to execute hooks");
        } else if (opt_cd_toggle == true) {
            if ((status = toggle_desk_set_curr(cfg->base.task, &args))) {
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
