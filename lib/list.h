#ifndef TEC_LIST_H
#define TEC_LIST_H

#include "libtec.h"

void list_free(tec_list_t * head);
tec_list_t *list_add(tec_list_t * head, char *id, int status);

#endif
