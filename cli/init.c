#include "tec.h"
#include "aux/errno.h"
#include "aux/config.h"

int tec_cli_init(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    (void)cfg;
    (void)argvec;
    int status = ETEC_OK;

    if ((status = tec_make_db(teccfg.base.task)))
        return TEC_LOG_E(EFMT_INIT, tec_strerror(status));
    return ETEC_OK;
}
