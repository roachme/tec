#include <ctype.h>
#include <string.h>

#include "tec.h"
#include "aux/config.h"

// "prio",  /* task priority */
// "type",  /* task type: bugfix, hotfix, feature */
// "date",  /* task date of creation */
// "desc",  /* task description */
// "user"   /* who created, who's woring on it */
// "users"  /* list of users */
// "teams"  /* list of teams */
// "label"  /* list of labels */
// "time"   /* time tracker */

/* roachme: replace all prios if user specifies any in config file */
static int valid_prio(const char *val)
{
    char *prios[] = { "lowest", "low", "mid", "high", "highest" };
    int size = sizeof(prios) / sizeof(prios[0]);

    for (int i = 0; i < size; ++i)
        if (strncmp(val, prios[i], 10) == 0)
            return true;
    return false;
}

/* roachme: replace all types if user specifies any in config file */
static int valid_type(const char *val)
{
    char *types[] = { "task", "bugfix", "feature", "hotfix" };
    int size = sizeof(types) / sizeof(types[0]);

    for (int i = 0; i < size; ++i) {
        if (strncmp(val, types[i], 10) == 0)
            return true;
    }
    return false;
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
int tec_cli_set(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    tec_ctx_t ctx = CTX_INIT;
    tec_arg_t args;
    int c, i, retcode, status;
    int opt_help, opt_interactive, opt_quiet;
    const char *errfmt = "cannot set task units '%s': %s";

    retcode = LIBTEC_OK;
    opt_help = opt_interactive = opt_quiet = false;
    args.env = args.desk = args.task = NULL;

    while ((c = getopt(argvec->used, argvec->argv, ":d:e:hiqD:T:P:")) != -1) {
        switch (c) {
        case 'd':
            args.desk = optarg;
            break;
        case 'e':
            args.env = optarg;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'i':
            opt_interactive = true;
            return elog(1, "this option is under development");
            break;
        case 'T':
            if (valid_type(optarg) == false) {
                elog(1, "invalid priority '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "type") == NULL)
                ctx.units = tec_unit_add(ctx.units, "type", optarg);
            break;
        case 'D':
            if (valid_desc(optarg) == false) {
                elog(1, "invalid description '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "desc") == NULL)
                ctx.units = tec_unit_add(ctx.units, "desc", optarg);
            break;
        case 'P':
            if (valid_prio(optarg) == false) {
                elog(1, "invalid priority '%s'", optarg);
                return tec_cli_help_usage("set");
            }
            if (tec_unit_get(ctx.units, "prio") == NULL)
                ctx.units = tec_unit_add(ctx.units, "prio", optarg);
            break;
        case ':':
            elog(EXIT_FAILURE, FMT_OPT_ARG_REQ, optopt);
            return tec_cli_help_usage("set");
        default:
            elog(EXIT_FAILURE, FMT_OPT_ARG_INV, optopt);
            return tec_cli_help_usage("set");
        }
    }
    i = optind;

    if (opt_help == true)
        return tec_cli_help_usage("set");

    if ((status = tec_cli_check_env(&args, errfmt, opt_quiet)))
        return status;
    else if ((status = tec_cli_check_desk(&args, errfmt, opt_quiet)))
        return status;

    do {
        args.task = argvec->argv[i];

        if ((status = tec_cli_check_task(&args, errfmt, opt_quiet))) {
            ;
        } else if ((status = tec_task_set(cfg->base.task, &args, &ctx))) {
            args.task = args.task ? args.task : "NOCURR";
            if (opt_quiet == false)
                elog(status, errfmt, args.task, tec_strerror(status));
        } else if ((status = hook_action(&args, "set"))) {
            if (opt_quiet == false)
                elog(1, errfmt, args.task, "failed to execute hooks");
        }

        ctx.units = tec_unit_free(ctx.units);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++i < argvec->used);

    return retcode;
}
