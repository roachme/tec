#include <stdlib.h>
#include "osdep.h"

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef __linux__
char *tec_cli_osdep_getenv(char *name)
{
    /* Get logical current working directory via shell variable, because
     * user current working directory might a symlink.  */
    return getenv(name);
}

char *tec_cli_osdep_getenv_home(void)
{
    return tec_cli_osdep_getenv("HOME");
}

char *tec_cli_osdep_getenv_cwd(void)
{
    return tec_cli_osdep_getenv("PWD");
}

int tec_cli_osdep_chdir(char *path)
{
    return chdir(path);
}
#endif
