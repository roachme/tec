#include <string.h>
#include <stdlib.h>

#include "tec.h"
#include "aux/config.h"

typedef struct keyvec {
    char **keys;
    size_t used;
    size_t size;
} keyvec_t;

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

static bool argument_keys_is_empty(keyvec_t *vec)
{
    return vec->used == 0;
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

int tec_cli_cat(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    keyvec_t vec;
    tec_arg_t args;
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
        } else if ((status = hook_cat(&unitpgn, &args, "cat"))) {
            if (opt_quiet == false)
                elog(status, errfmt, args.taskid, "failed to execute hooks");
        }

        if (status == LIBTEC_OK) {
            units = tec_unit_add(units, "id", args.taskid);
            units = tec_unit_join(units, ctx->units);
            units = tec_unit_join(units, unitpgn);

            if (argument_keys_is_empty(&vec) == false) {
                for (int i = 0; i < vec.used; i++) {
                    int found;
                    for (tec_unit_t * tmp = units; tmp; tmp = tmp->next) {
                        if ((found = strcmp(vec.keys[i], tmp->key)) == 0) {
                            printf("%s\n", tmp->val);
                            break;
                        }
                    }
                    if (found && opt_quiet == false)
                        elog(1, errfmt, vec.keys[i], "no such key");
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

    argument_keys_free(&vec);
    return retcode;
}
