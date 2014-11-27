/**
 * @file    gc.c
 * @name    Garbage Collector
 * @author  Pavel Tobias (xtobia01)
 * @brief   Implementation of simple garbage collector
 ****************************************************************************/

#include <stdlib.h>
#include <string.h>


#include "gc.h"

struct branch {
    char *tag;
    unsigned length;
    void **data;
    struct branch *next;
};

static void *allocate(const char *tag, size_t size, void *old_pointer);
static struct branch *create_branch(const char *tag);
static void free_branch(struct branch **branch_ptr, const char *tag);
static void *realloc_branch(struct branch *branch, size_t size, void *old_pointer);

void *gc_calloc(const char *tag, size_t number, size_t size)
{
    void *new_pointer;

    if (!number || !size)
        return NULL;

    if ((new_pointer = allocate(tag, number * size, NULL)) == NULL)
        return NULL;

    memset(new_pointer, 0, number * size);

    return new_pointer;
}

void gc_free(const char *tag)
{
    allocate(tag, 0, NULL);
}

void *gc_malloc(const char *tag, size_t size)
{
    return size ? allocate(tag, size, NULL) : NULL;
}

void *gc_realloc(const char *tag, void *pointer, size_t size)
{
    void *new_pointer;

    if (!size)
        return NULL;

    if ((new_pointer = allocate(tag, size, pointer)) == NULL)
        return NULL;

    return new_pointer;
}

static void *allocate(const char *tag, size_t size, void *old_pointer)
{
    static struct branch *first;
    struct branch **branch_ptr = &first;

    if (tag == NULL)
        return NULL;

    if (!size) {
        free_branch(&first, tag);
        return NULL;
    }

    while (*branch_ptr != NULL) {
        if (!strcmp((*branch_ptr)->tag, tag))
            break;
        branch_ptr = &(*branch_ptr)->next;
    }

    if (*branch_ptr == NULL) {
        if ((*branch_ptr = create_branch(tag)) == NULL)
            return NULL;
    }

    return realloc_branch(*branch_ptr, size, old_pointer);
}

static struct branch *create_branch(const char *tag)
{
    struct branch *branch;

    if ((branch = malloc(sizeof(struct branch))) == NULL)
        return NULL;

    if ((branch->tag = malloc(strlen(tag) + 1)) == NULL) {
        free(branch);
        return NULL;
    }

    strcpy(branch->tag, tag);
    branch->length = 0;
    branch->data = NULL;
    branch->next = NULL;

    return branch;
}

static void free_branch(struct branch **branch_ptr, const char *tag)
{
    struct branch *branch;

    while (*branch_ptr != NULL) {
        if (!strcmp((*branch_ptr)->tag, tag))
            break;
        branch_ptr = &(*branch_ptr)->next;
    }

    if (*branch_ptr == NULL)
        return;

    branch = *branch_ptr;
    *branch_ptr = branch->next;

    while (branch->length--)
        free(branch->data[branch->length]);

    free(branch->data);
    free(branch->tag);
    free(branch);
}

static void *realloc_branch(struct branch *branch, size_t size, void *old_pointer)
{
    void **pointer_ptr = NULL;
    void **temp_data;
    void *temp_pointer;

    if (old_pointer != NULL) {
        for (unsigned i = 0; i < branch->length; i++) {
            if (branch->data[i] == old_pointer) {
                pointer_ptr = &branch->data[i];
                break;
            }
        }
        if (pointer_ptr == NULL)
            return NULL;
    } else {
        if ((temp_data = realloc(branch->data,
                                 (branch->length + 1) * sizeof(void *)))
            == NULL)
            return NULL;
        branch->data = temp_data;
        branch->data[branch->length] = NULL;
        pointer_ptr = &branch->data[branch->length];
        branch->length++;
    }

    if ((temp_pointer = realloc(*pointer_ptr, size)) == NULL)
        return NULL;

    *pointer_ptr = temp_pointer;

    return *pointer_ptr;
}
