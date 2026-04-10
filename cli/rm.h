#ifndef TEC_CLI_RM_H
#define TEC_CLI_RM_H

#include <stdbool.h>

enum tec_cli_rm_interactive {
    RMI_ALWAYS,
    RMI_SOMETIMES,
    RMI_NEVER,
};

struct tec_cli_rm_options {
    bool help;
    bool quiet;
    bool verbose;
    bool change_dir;
    enum tec_cli_rm_interactive interactive;
};

void tec_cli_rm_option_init(struct tec_cli_rm_options *opts);

#endif
