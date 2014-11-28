/**
 * @file    value_list.h
 * @name    Value_list Datatype (interface)
 * @author  Pavel Tobias (xtobia01)
 * @brief   Multipurpose list of constant-address'd My_value structures
 ****************************************************************************/

#ifndef __VALUE_LIST_H__
#define __VALUE_LIST_H__

#include "common.h"

typedef struct value_list *Value_list;

int vl_destroy(struct value_list **list);
int vl_get(struct value_list **list, unsigned index, My_value **value_ret);
int vl_get_last(struct value_list **list, My_value **value_ret);
int vl_init(struct value_list **list);

#endif
