#include "opts.h"

void tec_cli_cat_option_init(struct tec_cli_cat_options *opts)
{
    opts->help = false;
    opts->quiet = false;
}

void tec_cli_cd_option_init(struct tec_cli_cd_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->change_dir = true;
    opts->change_tog = true;
}

void tec_cli_rm_option_init(struct tec_cli_rm_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->verbose = false;
    opts->change_dir = false;
    opts->mode = RMI_ALWAYS;
}

void tec_cli_ls_option_init(struct tec_cli_ls_options *opts)
{
    opts->help = false;
    opts->quiet = false;
    opts->togg = false;
}

void tec_cli_set_option_init(struct tec_cli_set_options *opts)
{
    opts->help = false;
    opts->quiet = false;
}
