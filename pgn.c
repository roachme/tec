#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMDSIZ      10

typedef struct pgn_cmd {
    char name[CMDSIZ + 1];
    char cmd[BUFSIZ + 1];
} pgn_cmd_t;

pgn_cmd_t *pgn_cmd_init(char *name)
{
    pgn_cmd_t *pgn;

    if ((pgn = malloc(sizeof(*pgn))) == NULL)
        return NULL;

    strncpy(pgn->name, name, CMDSIZ);
    return pgn;
}

int main(int argc, char **argv)
{
    return 0;
}
