#ifndef TEC_CLI_OSDEP_H
#define TEC_CLI_OSDEP_H

int tec_cli_osdep_chdir(char *path);
char *tec_cli_osdep_getenv_cwd(void);
char *tec_cli_osdep_getenv_home(void);

#endif
