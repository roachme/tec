#ifndef TEC_CLI_ARGVEC_H
#define TEC_CLI_ARGVEC_H

#include <stdbool.h>

typedef struct argvec {
    int i;                      /* index of current argument */
    int used;                   /* number of used slots in dynamic array */
    int size;                   /* capacity of dynamic array */
    int offset;                 /* offset in dynamic array of arguments */
    char **argv;                /* argument vector */
} tec_argvec_t;

void argvec_init(tec_argvec_t * vec);
bool argvec_is_empty(const tec_argvec_t * vec);
void argvec_deinit(tec_argvec_t * vec);
void argvec_offset(tec_argvec_t * vec, int offset);
void argvec_add(tec_argvec_t * vec, const char *arg);
void argvec_parse(tec_argvec_t * vec, int argc, const char **argv);
void argvec_replace(tec_argvec_t * vec, int vec_idx, char *arg);

#endif
