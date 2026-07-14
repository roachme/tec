#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "aux.h"
#include "errno.h"
#include "osdep.h"
#include "config.h"

char *unitkeys[] = { "prio", "type", "date", "desc", };

unsigned int nunitkey = sizeof(unitkeys) / sizeof(unitkeys[0]);

bool tec_aux_yesno(void)
{
    char choice[10] = { 0 };

    fgets(choice, sizeof(choice), stdin);
    if (choice[0] == 'y' || choice[0] == 'Y')
        return true;
    return false;
}

bool tec_aux_do_change_user_cwd(tec_arg_t *args)
{
    char *base = teccfg.base.task;
    char buf[FILENAME_MAX + 1] = { 0 };

    sprintf(buf, "%s/%s/%s/%s", base, args->env, args->desk, args->task);
    return strcmp(buf, tec_cli_osdep_getenv_cwd()) == 0;
}

int tec_aux_check_cd_alias(tec_argvec_t *argvec)
{
    for (int idx = argvec->i; idx < argvec->used; ++idx) {
        if (strcmp(argvec->argv[idx], "-") == 0 && argvec->used - argvec->i > 1)
            return TEC_ALIAS;
    }
    return TEC_OK;
}

int tec_aux_is_valid_desc(const char *val)
{
    if (!isalnum(*val++))
        return false;
    for (; *val; ++val)
        if (!(isalnum(*val) || isspace(*val) || *val == '_' || *val == '-'))
            return false;
    return isalnum(*--val) != 0;
}

/* roachme: replace all prios if user specifies any in config file */
int tec_aux_is_valid_prio(const char *val)
{
    char *prios[] = { "lowest", "low", "mid", "high", "highest" };
    int size = sizeof(prios) / sizeof(prios[0]);

    for (int i = 0; i < size; ++i)
        if (strncmp(val, prios[i], 10) == 0)
            return true;
    return false;
}

/* roachme: replace all types if user specifies any in config file */
int tec_aux_is_valid_type(const char *val)
{
    char *types[] = { "task", "bugfix", "feature", "hotfix" };
    int size = sizeof(types) / sizeof(types[0]);

    for (int i = 0; i < size; ++i) {
        if (strncmp(val, types[i], 10) == 0)
            return true;
    }
    return false;
}

int aux_show_key(char *key, tec_unit_t *units)
{
    for (; units; units = units->next) {
        if (!strcmp(key, units->key)) {
            printf("%s\n", units->val);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
