#ifndef TEC_OSDEP_H
#define TEC_OSDEP_H

#include <stdbool.h>

bool MKDIR(char *path);
bool RMDIR(char *path);
bool MOVE(char *src, char *dst);
bool ISFILE(char *fname);
bool ISDIR(char *fname);

#endif
