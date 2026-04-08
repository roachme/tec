#ifndef TEC_CLI_RM_H
#define TEC_CLI_RM_H

#include <stdbool.h>

enum rm_interactive {
    RMI_ALWAYS,
    RMI_SOMETIMES,
    RMI_NEVER,
};

struct rm_options {
    bool help;
    bool quiet;
    bool verbose;
    bool change_dir;
    enum rm_interactive interactive;
};

void rm_option_init(struct rm_options *opts);

#endif
