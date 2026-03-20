#include "tec.h"

int tec_cli_version(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    printf("%s version %s\n", PROGRAM, VERSION);
    return 0;
}
