#ifndef TEC_CLI_ERRNO_H
#define TEC_CLI_ERRNO_H

#include "../../lib/libtec.h"

#define TEC_CLI_EBUF        100

#define ETEC_NOCURR         "NOCURR"
#define ETEC_NOPREV         "NOCURR"

#define FMT_UNIT            "%-" xstr(PADDING_UNIT) "s : %s\n"

#define EFMT_DEV             "'-%c': option is under development"

#define EFMT_TASK_CAT_UNIT  "cannot show task units '%s': '%s' no such key"
#define EFMT_DESK_CAT_UNIT  "cannot show desk units '%s': '%s' no such key"
#define EFMT_ENV_CAT_UNIT   "cannot show environment units '%s': %s no such key"

#define EFMT_INIT           "cannot make core filesystem: %s"

#define EFMT_TASK_ADD       "cannot add task '%s': %s"
#define EFMT_TASK_CAT       "cannot show task units '%s': %s"
#define EFMT_TASK_CD        "cannot switch to task '%s': %s"
#define EFMT_TASK_LS        "cannot list tasks '%s': %s"
#define EFMT_TASK_MV        "cannot move (rename) task '%s': %s"
#define EFMT_TASK_RM        "cannot remove task '%s': %s"
#define EFMT_TASK_SET       "cannot set task units '%s': %s"

#define EFMT_DESK_ADD       "cannot add desk '%s': %s"
#define EFMT_DESK_CAT       "cannot show desk units '%s': %s"
#define EFMT_DESK_CD        "cannot switch to desk '%s': %s"
#define EFMT_DESK_LS        "cannot list desk '%s': %s"
#define EFMT_DESK_MV        "cannot move (rename) desk '%s': %s"
#define EFMT_DESK_RM        "cannot remove desk '%s': %s"
#define EFMT_DESK_SET       "cannot set desk unit value '%s': %s"

#define EFMT_ENV_ADD        "cannot add environment '%s': %s"
#define EFMT_ENV_CAT        "cannot show environment units '%s': %s"
#define EFMT_ENV_CD         "cannot switch to environment '%s': %s"
#define EFMT_ENV_LS         "cannot list environment '%s': %s"
#define EFMT_ENV_REN        "cannot rename environment '%s': %s"
#define EFMT_ENV_RM         "cannot remove environment '%s': %s"
#define EFMT_ENV_SET        "cannot set environment unit value '%s': %s"

enum tec_cli_errno {
    ETEC_ALIAS = E__TEC_STATUS_LAST,
    ETEC_HOOK_EXEC,

    ETEC_TOGG_UPDATE,

    ETEC_TOGG_ENV_GET_CURR,
    ETEC_TOGG_ENV_GET_PREV,
    ETEC_TOGG_ENV_SET_CURR,
    ETEC_TOGG_ENV_UNSET_CURR,
    ETEC_TOGG_ENV_UNSET_PREV,

    ETEC_TOGG_DESK_GET_CURR,
    ETEC_TOGG_DESK_GET_PREV,
    ETEC_TOGG_DESK_SET_CURR,
    ETEC_TOGG_DESK_UNSET_CURR,
    ETEC_TOGG_DESK_UNSET_PREV,

    ETEC_TOGG_TASK_GET_CURR,
    ETEC_TOGG_TASK_GET_PREV,
    ETEC_TOGG_TASK_SET_CURR,
    ETEC_TOGG_TASK_UNSET,
    ETEC_TOGG_TASK_UNSET_CURR,
    ETEC_TOGG_TASK_UNSET_PREV,

    ETEC_ARG_ENV_REQ,
    ETEC_ARG_DESK_REQ,
    ETEC_ARG_ENV_TOO_LONG,
    ETEC_ARG_DESK_TOO_LONG,
    ETEC_ARG_TASK_TOO_LONG,

    ETEC_UNIT_INV_KEY,
    ETEC_UNIT_GEN_FAIL,

    __TEC_CLI_STATUS_LAST
};

char *tec_strerror(int errnum);

#endif
