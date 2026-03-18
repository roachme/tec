#include <string.h>
#include <stdlib.h>

#include "tec.h"
#include "aux/config.h"

typedef struct keyvec {
    char **keys;
    size_t used;
    size_t size;
} keyvec_t;

static const char *errfmt = "cannot show units '%s': %s";

static void argument_keys_init(keyvec_t *vec)
{
    int size = 2;

    if ((vec->keys = malloc(size * sizeof(vec->keys))) == NULL) {
        elog(1, "'%s': memory allocation failed", __FUNCTION__);
        exit(1);
    }

    vec->used = 0;
    vec->size = size;
}

static int argument_keys_add(keyvec_t *vec, char *key)
{
    if (vec->used >= vec->size) {
        vec->size *= 2;
        if ((vec->keys =
             realloc(vec->keys, vec->size * sizeof(char *))) == NULL) {
            elog(1, "'%s': memory allocation failed", __FUNCTION__);
            exit(1);
        }
    }
    vec->keys[vec->used++] = strdup(key);
    return 0;
}

static void argument_keys_free(keyvec_t *vec)
{
    for (size_t i = 0; i < vec->used; ++i)
        free(vec->keys[i]);
    free(vec->keys);
}

static int valid_unitkeys(tec_unit_t *units)
{
    for (int i = 0; units && i < nunitkey; units = units->next) {
        if (strcmp(units->key, unitkeys[i]) != 0)
            return 1;
        ++i;
    }
    return 0;
}

static int _show_key(char *task, tec_unit_t *unitbin, tec_unit_t *unitpgn,
                     char *key)
{
    struct tec_unit *units;

    if (strcmp(key, "id") == 0) {
        printf("%s\n", task);
        return 0;
    }

    for (units = unitbin; units; units = units->next) {
        if (strcmp(key, units->key) == 0) {
            printf("%s\n", units->val);
            return 0;
        }
    }

    for (units = unitpgn; units; units = units->next) {
        if (strcmp(key, units->key) == 0) {
            printf("%s\n", units->val);
            return 0;
        }
    }

    return 1;
}

static int show_key(char *task, tec_unit_t *unitbin,
                    tec_unit_t *unitpgn, keyvec_t *vec, int quiet)
{
    int status;
    int retcode = LIBTEC_OK;
    const char *errfmt = "cannot show unit key '%s': no such key";

    for (int i = 0; i < vec->used; i++) {
        if ((status = _show_key(task, unitbin, unitpgn, vec->keys[i]))) {
            if (quiet == false)
                elog(1, errfmt, vec->keys[i]);
            retcode = status == LIBTEC_OK ? retcode : status;
        }
    }
    return retcode;
}

static int show_keys(char *task, tec_unit_t *unitbin, tec_unit_t *unitpgn)
{
    const char *fmt = "%-" xstr(PADDING_UNIT) "s : %s\n";

    printf(fmt, "id", task);

    for (struct tec_unit * units = unitbin; units; units = units->next)
        printf(fmt, units->key, units->val);

    for (; unitpgn; unitpgn = unitpgn->next)
        printf(fmt, unitpgn->key, unitpgn->val);

    return 0;
}

int tec_cli_cat(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    keyvec_t vec;
    tec_arg_t args;
    tec_unit_t *unitpgn;
    int opt_quiet, opt_help;
    int c, i, retcode, status;
    int opt_show_specific_key;

    unitpgn = NULL;
    retcode = LIBTEC_OK;
    opt_quiet = opt_help = false;
    opt_show_specific_key = false;
    args.env = args.desk = args.taskid = NULL;

    argument_keys_init(&vec);
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
            opt_show_specific_key = true;
            argument_keys_add(&vec, optarg);
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
        }

        else if ((status = hook_show(&unitpgn, &args, "cat"))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, "failed to execute hooks");
        }

        else if (opt_show_specific_key == true) {
            if ((status =
                 show_key(args.taskid, ctx->units, unitpgn, &vec, opt_quiet))) {
                ;
            }
        } else if ((status = show_keys(args.taskid, ctx->units, unitpgn))) {
            if (opt_quiet == false)
                elog(1, errfmt, args.taskid, "internal error");
        }
        unitpgn = tec_unit_free(unitpgn);
        ctx->units = tec_unit_free(ctx->units);
        retcode = status == LIBTEC_OK ? retcode : status;
    } while (++i < argvec->used);

    argument_keys_free(&vec);
    return retcode;
}
