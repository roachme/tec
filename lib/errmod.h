#ifndef TEC_ERRMOD_H
#define TEC_ERRMOD_H

#define TEC_ERRMSGSIZ   100
#define NERRCODE    50

int emod_set(int err);
char *emod_strerror(int errnum);

#endif
