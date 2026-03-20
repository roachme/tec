#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "log.h"
#include "argvec.h"

void argvec_init(tec_argvec_t *vec)
{
    int size = 2;

    if ((vec->argv = malloc(size * sizeof(vec->argv))) == NULL) {
        elog(1, "'%s': memory allocation failed", __FUNCTION__);
        exit(1);
    }

    /* Prevent invalid pointer dereference. Used in case when default
     * env name, desk name, task ID used.  */
    for (int i = 0; i < size; ++i)
        vec->argv[i] = NULL;

    vec->used = 0;
    vec->offset = 0;
    vec->size = size;
}

void argvec_show(tec_argvec_t *vec)
{
    for (int i = 0; i < vec->used; ++i) {
        printf("argvec_show: %s[%d] - used: %d\n", vec->argv[i], i, vec->used);
    }
}

void argvec_add(tec_argvec_t *vec, const char *arg)
{
    /* The last element is never used that's why minus one.
     * It is a NULL terminator to mimic C argv.  */
    if (vec->used >= vec->size - 1) {
        vec->size *= 2;
        if ((vec->argv =
             realloc(vec->argv, vec->size * sizeof(char *))) == NULL) {
            elog(1, "'%s': memory allocation failed", __FUNCTION__);
            exit(1);
        }

        /* Prevent invalid pointer dereference. Used in case when default
         * env name, desk name, task ID used.  */
        for (int i = vec->used; i < vec->size; ++i)
            vec->argv[i] = NULL;
    }
    vec->argv[vec->used++] = strdup(arg);
}

void argvec_parse(tec_argvec_t *vec, int argc, const char **argv)
{
    for (int i = 0; i < argc; i++)
        argvec_add(vec, argv[i]);
}

void argvec_replace(tec_argvec_t *vec, int vec_idx, char *arg, int argsiz)
{
    assert(vec_idx > 0 && vec_idx < vec->used - vec->offset);
    free(vec->argv[vec_idx]);   /* free previous key value.  */
    if ((vec->argv[vec_idx] = strndup(arg, argsiz)) == NULL) {
        elog(1, "'%s': memory allocation failed", __FUNCTION__);
        exit(1);
    }
}

void argvec_offset(tec_argvec_t *vec, int offset)
{
    assert((vec->used - offset) >= 0);
    vec->offset += offset;
    vec->used -= offset;
    vec->argv += offset;
}

void argvec_deinit(tec_argvec_t *vec)
{
    vec->argv = vec->argv - vec->offset;        /* Restore pointer to first element.  */
    for (int i = 0; i < vec->size; ++i)
        free(vec->argv[i]);
    free(vec->argv);
}
