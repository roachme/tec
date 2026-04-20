#include <string.h>
#include <stdlib.h>

#include "tec.h"
#include "aux/config.h"

/* TODO:
    Structure: tec PGN -i -e -d COMMAND [OPTION]... [APRS]
    1. Can't use getopt cuz there might be different options for plugins and
       their commands.
    2. Use for/while loop to circle through options and their arguments.
    3. Separate plugin options from plugin command options.
    4. Or maybe it's better to let the plugin to handle plugin options and the rest.
*/
int tec_cli_pgn(tec_argvec_t *argvec, tec_cfg_t *cfg)
{
    int len;
    int i = 0;
    int status = EXIT_SUCCESS;
    char cmd[BUFSIZ + 1] = { 0 };
    size_t cmdsize = sizeof(cmd);
    char *name = argvec->argv[0];
    const char *fmt = "%s/%s/%s -T %s -P %s";

    argvec_offset(argvec, 1);
    len =
        snprintf(cmd, cmdsize, fmt, cfg->base.pgn, name, name, cfg->base.task,
                 cfg->base.pgn);
    if (len < 0 || len >= cmdsize) {
        status = EXIT_FAILURE;
        goto err;
    }

    for (; i < argvec->used; ++i) {
        len = snprintf(cmd + strlen(cmd), cmdsize, " %s", argvec->argv[i]);
        if (len < 0 || len >= cmdsize) {
            status = EXIT_FAILURE;
            goto err;
        }
    }

    dlog(1, "pgn: %s", cmd);

    if (status == EXIT_SUCCESS)
        status = system(cmd);

 err:
    return status == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}
