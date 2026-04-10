#ifndef TEC_CLI_ADD_H
#define TEC_CLI_ADD_H

struct tec_cli_add_options {
    bool help;
    bool quiet;
    bool chdir;
    bool chtog;
};

void tec_cli_add_option_init(struct tec_cli_add_options *opts);

#endif
