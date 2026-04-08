#ifndef TEC_AUX_H
#define TEC_AUX_H

#include <stdbool.h>
#include "../../lib/libtec.h"

#define PROGRAM     "tec"

// TODO: Get rid of it (set options with default boolean values)
#define NONEBOOL        -1      /* Not yet set boolean value */

bool yesno(void);
bool do_change_user_cwd(tec_arg_t * args);

#endif
