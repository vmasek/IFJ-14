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

static void *allocate(const char *tag, size_t size);
static struct branch *create_branch(const char *tag);
static void free_branch(struct branch **branch_ptr, const char *tag);
static void *realloc_branch(struct branch *branch, size_t size);

void *gc_calloc(const char *tag, size_t number, size_t size)
{
    return (number && size) ? allocate(tag, number * size) : NULL;
}

void gc_free(const char *tag)
{
    gc_malloc(tag, 0);
}

void *gc_malloc(const char *tag, size_t size)
{
    return size ? allocate(tag, size) : NULL;
}

void *gc_realloc(const char *tag, void *pointer, size_t size)
{
    void *new_pointer;

    if (!size)
        return NULL;

    if ((new_pointer = allocate(tag, size)) == NULL)
        return NULL;

    if (pointer != NULL)
        memcpy(new_pointer, pointer, size);

    return new_pointer;
}

static void *allocate(const char *tag, size_t size)
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

    return realloc_branch(*branch_ptr, size);
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

static void *realloc_branch(struct branch *branch, size_t size)
{
    void *pointer;
    void **temp_data;

    if ((pointer = malloc(size)) == NULL)
        return NULL;

    if ((temp_data = realloc(branch->data, (branch->length + 1) * sizeof(void *)))
        == NULL) {
        free(pointer);
        return NULL;
    }

    branch->data = temp_data;
    branch->data[branch->length] = pointer;
    branch->length++;

    return pointer;
}
