#ifndef TEC_CLI_CD_H
#define TEC_CLI_CD_H

#include <stdbool.h>

struct tec_cli_cd_options {
    bool help;
    bool quiet;
    bool chdir;
    bool chtog;
};

void tec_cli_cd_option_init(struct tec_cli_cd_options *opts);

#endif
