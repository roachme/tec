#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "aux.h"
#include "osdep.h"
#include "config.h"

bool yesno(void)
{
    char choice[10] = { 0 };

    fgets(choice, sizeof(choice), stdin);
    if (choice[0] == 'y' || choice[0] == 'Y')
        return true;
    return false;
}

bool do_change_user_cwd(tec_arg_t *args)
{
    char *base = teccfg.base.task;
    char buf[FILENAME_MAX + 1] = { 0 };

    sprintf(buf, "%s/%s/%s/%s", base, args->env, args->desk, args->taskid);
    return strcmp(buf, tec_cli_osdep_getenv_cwd()) == 0;
}
