#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libconfig.h>

#include "../../lib/osdep.h"
#include "aux.h"
#include "log.h"
#include "config.h"
#include "hook.h"

// TODO: gotta add config checker so a program doesn't fail.

static int config_set_default_options(struct config *cfg)
{
    if (cfg->opts.color == NONEBOOL)
        cfg->opts.color = false;
    if (cfg->opts.debug == NONEBOOL)
        cfg->opts.debug = false;
    if (cfg->opts.hook == NONEBOOL)
        cfg->opts.hook = false;
    return 0;
}

static void resolve_env_var_home(char *dst, const char *src)
{
    char *home = getenv("HOME");
    char *found = strstr(src, "$HOME");

    if (found) {
        strcpy(dst, home);      /* TODO: it might be unset in some envs.  */
        strcat(dst, src + strlen("$HOME"));
    } else {
        strcpy(dst, src);
    }
}

static int tec_config_set_default_base(tec_cfg_t *tec_config)
{
    char pathname[PATH_MAX + 1] = { 0 };

    if (tec_config->base.task == NULL) {
        resolve_env_var_home(pathname, "$HOME/tectask");
        tec_config->base.task = strdup(pathname);
    }
    if (tec_config->base.pgn == NULL) {
        resolve_env_var_home(pathname, "$HOME/.local/lib/tec/pgn");
        tec_config->base.pgn = strdup(pathname);
    }
    return 0;
}

static int tec_config_set_default_config(tec_cfg_t *tec_config)
{
    tec_config_set_default_base(tec_config);
    config_set_default_options(tec_config);
    return 0;
}

static struct tec_hook *make_hook()
{
    struct tec_hook *hook;

    if ((hook = malloc(sizeof(struct tec_hook))) == NULL)
        return NULL;
    memset(hook, 0, sizeof(struct tec_hook));
    return hook;
}

static tec_alias_t *make_alias()
{
    tec_alias_t *alias;

    if ((alias = malloc(sizeof(tec_alias_t))) == NULL)
        return NULL;
    memset(alias, 0, sizeof(tec_alias_t));
    return alias;
}

static int tec_config_get_hooks(config_t *cfg, tec_cfg_t *tec_config)
{
    unsigned int count;
    struct tec_hook *hook;
    config_setting_t *setting;
    int retcode = EXIT_SUCCESS;

    if ((setting = config_lookup(cfg, "hooks.cat")) != NULL) {
        count = config_setting_length(setting);
        for (unsigned int i = 0; i < count; ++i) {
            const char *bincmd, *pgname, *pgncmd;
            config_setting_t *hook_conf = config_setting_get_elem(setting, i);

            if ((hook = make_hook()) != NULL) {
                if (!(config_setting_lookup_string(hook_conf, "bincmd", &bincmd)
                      && config_setting_lookup_string(hook_conf, "pgname",
                                                      &pgname)
                      && config_setting_lookup_string(hook_conf, "pgncmd",
                                                      &pgncmd))) {
                    TEC_LOG_E("FAILED: to parse hook.cat");
                    retcode = EXIT_FAILURE;
                    continue;
                }
                strcpy(hook->cmd, bincmd);
                strcpy(hook->pgname, pgname);
                strcpy(hook->pgncmd, pgncmd);
                hook->next = tec_config->hooks;
                tec_config->hooks = hook;
            }
        }
    }

    if ((setting = config_lookup(cfg, "hooks.action")) != NULL) {
        count = config_setting_length(setting);
        for (unsigned int i = 0; i < count; ++i) {
            const char *bincmd, *pgname, *pgncmd;
            config_setting_t *hook_conf = config_setting_get_elem(setting, i);

            if ((hook = make_hook()) != NULL) {
                if (!(config_setting_lookup_string(hook_conf, "bincmd", &bincmd)
                      && config_setting_lookup_string(hook_conf, "pgname",
                                                      &pgname)
                      && config_setting_lookup_string(hook_conf, "pgncmd",
                                                      &pgncmd))) {
                    TEC_LOG_E("FAILED: to parse hook.action");
                    retcode = EXIT_FAILURE;
                    continue;
                }
                strcpy(hook->cmd, bincmd);
                strcpy(hook->pgname, pgname);
                strcpy(hook->pgncmd, pgncmd);
                hook->next = tec_config->hooks;
                tec_config->hooks = hook;
            }
        }
    }
    // TODO: ls hooks
    return retcode;
}

static int tec_config_get_aliases(config_t *cfg, tec_cfg_t *tec_config)
{
    tec_alias_t *alias;
    const char *name, *value;
    config_setting_t *setting;

    if ((setting = config_lookup(cfg, "alias")) == NULL)
        return 0;

    // TODO: trim whitespaces
    for (int i = 0; i < config_setting_length(setting); ++i) {
        config_setting_t *_setting = config_setting_get_elem(setting, i);
        name = config_setting_name(_setting);
        if (config_setting_lookup_string(setting, name, &value)) {
            if ((alias = make_alias()) != NULL) {
                strcpy(alias->name, name);
                strcpy(alias->cmd, value);
                alias->next = tec_config->alias;
                tec_config->alias = alias;
            }
        }
    }
    return 0;
}

static int tec_config_get_base(config_t *cfg, tec_cfg_t *tec_config)
{
    const char *path = NULL;
    config_setting_t *setting;
    char buf[PATH_MAX + 1] = { 0 };

    if ((setting = config_lookup(cfg, "base")) != NULL) {
        if (config_setting_lookup_string(setting, "task", &path)) {
            resolve_env_var_home(buf, path);

            /* If option is not passed via CLI then set it from config */
            if (!tec_config->base.task)
                tec_config->base.task = strdup(buf);
        }
        if (config_setting_lookup_string(setting, "pgn", &path)) {
            resolve_env_var_home(buf, path);

            /* If option is not passed via CLI then set it from config */
            if (!tec_config->base.pgn)
                tec_config->base.pgn = strdup(buf);
        }
    }
    return EXIT_SUCCESS;
}

static int tec_config_get_options(config_t *cfg, tec_cfg_t *tec_config)
{
    config_setting_t *setting;

    if ((setting = config_lookup(cfg, "options")) == NULL)
        return 0;

    /* Set these options only if they were not set by CLI options */
    if (tec_config->opts.color == NONEBOOL)
        config_setting_lookup_bool(setting, "color", &tec_config->opts.color);
    if (tec_config->opts.debug == NONEBOOL)
        config_setting_lookup_bool(setting, "debug", &tec_config->opts.debug);
    if (tec_config->opts.hook == NONEBOOL)
        config_setting_lookup_bool(setting, "hook", &tec_config->opts.hook);
    return EXIT_SUCCESS;
}

static int parseconf(tec_cfg_t *tec_config, const char *fname)
{
    config_t cfg;

    config_init(&cfg);
    // jq: parse error: Invalid numeric literal at line 3, column 0
    if (!config_read_file(&cfg, fname)) {
        return TEC_LOG_E("%s:%d - %s", config_error_file(&cfg),
                         config_error_line(&cfg), config_error_text(&cfg));
    }

    if (tec_config_get_base(&cfg, tec_config))
        TEC_LOG_D("tec_config_get_base: FAILED\n");
    else if (tec_config_get_options(&cfg, tec_config))
        TEC_LOG_D("tec_config_get_options: FAILED\n");
    else if (tec_config_get_hooks(&cfg, tec_config))
        TEC_LOG_D("tec_config_get_hooks: FAILED\n");
    else if (tec_config_get_aliases(&cfg, tec_config))
        TEC_LOG_D("tec_config_get_aliases: FAILED\n");

    config_destroy(&cfg);
    return 0;
}

void tec_config_init(tec_cfg_t *cfg)
{
    cfg->alias = NULL;

    cfg->opts.color = NONEBOOL;
    cfg->opts.debug = NONEBOOL;
    cfg->opts.hook = NONEBOOL;

    cfg->base.cfg = NULL;
    cfg->base.pgn = NULL;
    cfg->base.task = NULL;

    cfg->hooks = NULL;
}

// TODO: simplify this mess. add a separate function to find and check config file
int tec_config_parse(tec_cfg_t *tec_config)
{
    int status = TEC_OK;
    char cfgfile[CONFIGSIZ + 1];
    char *homedir = getenv("HOME");     // FIXME: use osdep module
    const char *cfgfmts[] = {
        "%s/.%s/%s.cfg",
        "%s/.config/%s/%s.cfg",
    };

    if (tec_config->base.cfg) {
        if (!ISFILE(tec_config->base.cfg)) {
            status = EXIT_FAILURE;
            TEC_LOG_E("'%s': no such config file", tec_config->base.cfg);
        } else
            status = parseconf(tec_config, tec_config->base.cfg);
    } else {
        for (unsigned int i = 0; i < ARRAY_SIZE(cfgfmts); ++i) {
            sprintf(cfgfile, cfgfmts[i], homedir, PROGRAM, PROGRAM);
            if (ISFILE(cfgfile)) {
                status = parseconf(tec_config, cfgfile);
            }
        }
    }
    tec_config_set_default_config(tec_config);
    return status == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}

void tec_config_destroy(tec_cfg_t *tec_config)
{
    tec_alias_t *alias;
    struct tec_hook *head;

    for (head = tec_config->hooks; head != NULL;) {
        struct tec_hook *tmp = head;
        head = head->next;
        free(tmp);
    }

    for (alias = tec_config->alias; alias != NULL;) {
        tec_alias_t *tmp = alias;
        alias = alias->next;
        free(tmp);
    }

    free(tec_config->base.cfg);
    free(tec_config->base.task);
    free(tec_config->base.pgn);
}
