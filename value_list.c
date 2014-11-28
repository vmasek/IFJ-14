/**
 * @file    value_list.c
 * @name    Value_list Datatype
 * @author  Pavel Tobias (xtobia01)
 * @brief   Multipurpose list of constant-address'd My_value structures
 ****************************************************************************/

#include <stdlib.h>

#include "value_list.h"
#include "errors.h"
#include "common.h"

struct value_list {
    unsigned value_count;
    My_value **value_ptrs;
};

int vl_destroy(struct value_list **list)
{
    if (list == NULL || *list == NULL)
        return INTERNAL_ERROR;

    while((*list)->value_count--)
        free((*list)->value_ptrs[(*list)->value_count]);

    free((*list)->value_ptrs);
    free(*list);
    *list = NULL;

    return SUCCESS;
}

int vl_get(struct value_list **list, unsigned index, My_value **value_ret)
{
    My_value **ptrs_temp;
    My_value *pointer;

    if (list == NULL || *list == NULL)
        return INTERNAL_ERROR;

    if (index >= (*list)->value_count) {
        if (index > (*list)->value_count)
            return INTERNAL_ERROR;
        if ((ptrs_temp = realloc((*list)->value_ptrs,
            ((*list)->value_count + 1) * sizeof(My_value *))) == NULL)
            return INTERNAL_ERROR;
        (*list)->value_ptrs = ptrs_temp;
        (*list)->value_ptrs[(*list)->value_count] = NULL;
        (*list)->value_count++;
        if ((pointer = malloc(sizeof(My_value))) == NULL)
            return INTERNAL_ERROR;
        (*list)->value_ptrs[(*list)->value_count - 1] = pointer;
    }

    *value_ret = (*list)->value_ptrs[index];

    return SUCCESS;
}

int vl_get_last(struct value_list **list, My_value **value_ret)
{
    return vl_get(list, (*list)->value_count, value_ret);
}

int vl_init(struct value_list **list)
{
    if (list == NULL)
        return INTERNAL_ERROR;

    if ((*list = malloc(sizeof(struct value_list))) == NULL)
        return INTERNAL_ERROR;

    (*list)->value_count = 0;
    (*list)->value_ptrs = NULL;

    return SUCCESS;
}
