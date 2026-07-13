#ifndef TEC_CLI_OPTS_H
#define TEC_CLI_OPTS_H

#include <stdbool.h>

enum tec_cli_rm_mode {
    RMI_ALWAYS,
    RMI_SOMETIMES,
    RMI_NEVER,
};

struct tec_cli_cat_options {
    bool help;                  /* Output a usage message ad exit */
    bool quiet;                 /* Do not write anything to standard output */
};

struct tec_cli_cd_options {
    bool help;                  /* Output a usage message ad exit */
    bool quiet;                 /* Do not write anything to standard output */
    bool verbose;               /* Explain what is being done */
    bool change_dir;            /* Change working directory */
    bool change_tog;            /* Update toggles */
};

struct tec_cli_ls_options {
    bool help;                  /* Output a usage message ad exit */
    bool quiet;                 /* Do not write anything to standard output */
    bool togg;                  /* Show only toggle objects */
    bool head;                  /* Show headers */
};

struct tec_cli_rm_options {
    bool help;                  /* Output a usage message ad exit */
    bool quiet;                 /* Do not write anything to standard output */
    bool verbose;               /* Explain what is being done */
    bool change_dir;            /* Change working directory, chosen automatically */
    enum tec_cli_rm_mode mode;  /* Set remove mode */
};

struct tec_cli_set_options {
    bool help;                  /* Output a usage message ad exit */
    bool quiet;                 /* Do not write anything to standard output */
};

void tec_cli_cat_option_init(struct tec_cli_cat_options *opts);
void tec_cli_cd_option_init(struct tec_cli_cd_options *opts);
void tec_cli_ls_option_init(struct tec_cli_ls_options *opts);
void tec_cli_rm_option_init(struct tec_cli_rm_options *opts);
void tec_cli_set_option_init(struct tec_cli_set_options *opts);

#endif
