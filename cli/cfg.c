#include <string.h>
#include "tec.h"
#include "aux/config.h"

static int _show_aliases(tec_alias_t *aliases)
{
    tec_alias_t *alias;

    for (alias = aliases; alias; alias = alias->next)
        printf("  %s\t\t: %s\n", alias->name, alias->cmd);
    return 0;
}

static int _act_hooks(struct tec_hook *hooks)
{
    struct tec_hook *hook;

    for (hook = hooks; hook; hook = hook->next) {
        if (strcmp("cat", hook->cmd) && strcmp("ls", hook->cmd))
            printf("%s.%s\n", hook->pgname, hook->pgncmd);
    }
    return 0;
}

static int _cat_hooks(struct tec_hook *hooks)
{
    struct tec_hook *hook;

    for (hook = hooks; hook; hook = hook->next)
        if (strcmp("cat", hook->cmd) == 0)
            printf("%s.%s\n", hook->pgname, hook->pgncmd);
    return 0;
}

static int _ls_hooks(struct tec_hook *hooks)
{
    struct tec_hook *hook;

    for (hook = hooks; hook; hook = hook->next)
        if (strcmp("ls", hook->cmd) == 0)
            printf("%s.%s\n", hook->pgname, hook->pgncmd);
    return 0;

}

static int _cfg_get(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    if (argvec->used == 0)
        return elog(1, "wrong number of arguments, should at least 1");

    for (int i = 0; i < argvec->used; ++i) {
        if (strcmp("taskbase", argvec->argv[i]) == 0)
            printf("%s\n", teccfg.base.task);
        else if (strcmp("pgnbase", argvec->argv[i]) == 0)
            printf("%s\n", teccfg.base.pgn);
        else if (strcmp("opts.color", argvec->argv[i]) == 0)
            printf("%s\n", teccfg.opts.color ? "true" : "false");
        else if (strcmp("opts.debug", argvec->argv[i]) == 0)
            printf("%s\n", teccfg.opts.debug ? "true" : "false");
        else if (strcmp("opts.hook", argvec->argv[i]) == 0)
            printf("%s\n", teccfg.opts.hook ? "true" : "false");
        else if (strcmp("hook.cat", argvec->argv[i]) == 0)
            _cat_hooks(teccfg.hooks);
        else if (strcmp("hook.ls", argvec->argv[i]) == 0)
            _ls_hooks(teccfg.hooks);
        else if (strcmp("hook.act", argvec->argv[i]) == 0)
            _act_hooks(teccfg.hooks);
        else if (strcmp("alias", argvec->argv[i]) == 0)
            _show_aliases(teccfg.alias);
        else
            elog(1, "'%s': no such config value", argvec->argv[i]);
    }

    return 0;
}

// TODO: show config values from config file. Not option set via CLI
static int _cfg_ls(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    tec_alias_t *alias;
    struct tec_hook *hook;

    printf("Paths:\n");
    printf("  taskbase\t: %s\n", teccfg.base.task);
    printf("  pgnbase\t: %s\n", teccfg.base.pgn);
    printf("\nOptions:\n");
    printf("  debug\t\t: %s\n", teccfg.opts.debug ? "true" : "false");
    printf("  color\t\t: %s\n", teccfg.opts.color ? "true" : "false");
    printf("  hook\t\t: %s\n", teccfg.opts.hook ? "true" : "false");
    printf("\nHooks:\n");
    for (hook = teccfg.hooks; hook; hook = hook->next)
        printf("  %s\t\t: %s\t%s\n", hook->cmd, hook->pgname, hook->pgncmd);
    printf("\nAliases:\n");
    for (alias = teccfg.alias; alias; alias = alias->next)
        printf("  %s\t\t: %s\n", alias->name, alias->cmd);
    return 0;
}

static int _cfg_set(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    return 0;
}

static const builtin_t cfg_commands[] = {
    {.name = "get",.func = &_cfg_get},
    {.name = "ls",.func = &_cfg_ls},
    {.name = "set",.func = &_cfg_set},
};

int tec_cli_cfg(tec_argvec_t *argvec, tec_ctx_t *ctx)
{
    const char *cmd = argvec->argv[0] != NULL ? argvec->argv[0] : "ls";

    for (int i = 0; i < ARRAY_SIZE(cfg_commands); ++i)
        if (strcmp(cmd, cfg_commands[i].name) == 0) {
            if (cmd == argvec->argv[0])
                argvec_offset(argvec, 1);       /* Shift cfg and subcommand.  */
            return cfg_commands[i].func(argvec, ctx);
        }

    return elog(1, "'%s': no such cfg command", cmd);
}
