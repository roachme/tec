#include "tec.h"
#include "aux/log.h"

int tec_cli_version(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    printf("%s version %s\n", PROGRAM, VERSION);
    return 0;
}
