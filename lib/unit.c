#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "unit.h"

#define UNIT_DELIM        ":\n"
#define UNIT_FMT          "%s : %s\n"

static tec_unit_t *make_node(void)
{
    tec_unit_t *node;

    if ((node = malloc(sizeof(tec_unit_t))) == NULL)
        return NULL;

    node->key = node->val = NULL;
    node->next = NULL;
    return node;
}

static char *trim_whitespace_end(char *str)
{
    size_t len;

    len = strlen(str) - 1;
    while (len >= 0 && isspace(*(str + len)))
        len--;
    str[len + 1] = '\0';
    return str;
}

static char *trim_whitespace_beg(char *str)
{
    while ((isspace(*str)))
        str++;
    return str;
}

static char *trim_whitespace(char *str)
{
    str = trim_whitespace_beg(str);
    str = trim_whitespace_end(str);
    return str;
}

int unit_save(const char *filename, tec_unit_t *units)
{
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL)
        return 1;

    for (tec_unit_t * unit = units; unit; unit = unit->next)
        fprintf(fp, UNIT_FMT, unit->key, unit->val);
    return fclose(fp);
}

tec_unit_t *unit_parse(tec_unit_t *units, const char *str)
{
    char *token;
    char key[BUFSIZ + 1] = { 0 };
    char val[BUFSIZ + 1] = { 0 };
    char buf[BUFSIZ + 1] = { 0 };

    strcpy(buf, str);

    if ((token = strtok(buf, UNIT_DELIM)) != NULL)
        strcpy(key, trim_whitespace(token));
    if ((token = strtok(NULL, UNIT_DELIM)) != NULL)
        strcpy(val, trim_whitespace(token));
    if (key[0] != '\0' && val[0] != '\0')
        units = unit_add(units, key, val);
    return units;
}

/*
 * Get units all units.
*/
tec_unit_t *unit_load(const char *filename)
{
    FILE *fp;
    char buf[BUFSIZ + 1];
    tec_unit_t *units = NULL;

    if ((fp = fopen(filename, "r")) == NULL)
        return NULL;

    while (fgets(buf, BUFSIZ, fp) != NULL)
        units = unit_parse(units, buf);
    fclose(fp);
    return units;
}

/*
 * Add units one at a time.
*/
tec_unit_t *unit_add(tec_unit_t *head, const char *key, const char *val)
{
    tec_unit_t *unit, *tmp;

    if ((unit = make_node()) == NULL)
        return head;

    if (!(unit->key = strdup(key)) || !(unit->val = strdup(val)))
        return head;
    else if ((tmp = head) == NULL)
        return unit;

    while (tmp->next)
        tmp = tmp->next;
    tmp->next = unit;
    return head;
}

/**
 * Join two linked lists into one without copying data.
 */
tec_unit_t *unit_join(tec_unit_t *head, tec_unit_t *tail)
{
    tec_unit_t *units, *tmp;

    units = head;
    tmp = units;

    while (units->next)
        units = units->next;

    units->next = tail;
    return tmp;
}

/*
 * Set units one at a time.
*/
tec_unit_t *unit_set(tec_unit_t *head, const char *key, const char *val)
{
    for (; head != NULL; head = head->next) {
        if (strcmp(head->key, key) == 0) {
            free(head->val);
            head->val = strdup(val);
            return head;
        }
    }
    return unit_add(head, key, val);
}

/* TODO: make return value 'const char *'.  */
char *unit_get(tec_unit_t *head, const char *key)
{
    for (tec_unit_t * unit = head; unit; unit = unit->next)
        if (strcmp(unit->key, key) == 0)
            return unit->val;
    return NULL;
}

/*
 * Delete all units.
*/
void unit_free(tec_unit_t *head)
{
    tec_unit_t *curr, *next;

    for (curr = head; curr; curr = next) {
        next = curr->next;
        free(curr->key);
        free(curr->val);
        free(curr);
    }
}
