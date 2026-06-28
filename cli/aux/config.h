#ifndef CONFIG_H
#define CONFIG_H

#include "../../lib/libtec.h"

#define CONFIGSIZ       256

// FIXME: add an expression max_hookact + max_hookshow + max_hookls
#define CONF_MAXHOOK    10
#define CONF_MAXCOLDEF  10
#define CONF_MAXBASE    256
#define CONF_MAXPGNINS  256

typedef struct tec_base {
    char *cfg;                  /* Path to config filename */
    char *pgn;                  /* Directory where plugins are stored */
    char *task;                 /* Directory where tasks are stored */
} tec_base_t;

typedef struct tec_option {
    int color;                  /* use colors */
    int debug;                  /* output debug info */
    int hook;                   /* execute hooks from config, by default set */
} tec_opt_t;

typedef struct tec_alias {
    char name[10];
    char cmd[30];
    struct tec_alias *next;
} tec_alias_t;

typedef struct config {
    tec_opt_t opts;
    tec_base_t base;
    tec_alias_t *alias;
    struct tec_hook *hooks;
} tec_cfg_t;

#define TEC_CONFIG_SET_OPTS { .opts.color = NONEBOOL, .opts.hook = NONEBOOL, }

extern tec_cfg_t teccfg;

void tec_config_init(tec_cfg_t * cfg);
int tec_config_parse(tec_cfg_t * tec_config);
void tec_config_destroy(tec_cfg_t * tec_config);

#endif
