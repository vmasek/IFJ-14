/**
 * @file    gc.h
 * @name    Garbage Collector (interface)
 * @author  Pavel Tobias (xtobia01)
 * @brief   Interface for simple garbage collector
 ****************************************************************************/

#ifndef __GC_H__
#define __GC_H__

#include <stdlib.h>

void *gc_calloc(const char *tag, size_t number, size_t size);
void gc_free(const char *tag);
void *gc_malloc(const char *tag, size_t size);
void *gc_realloc(const char *tag, void *pointer, size_t size);

/*  USAGE EXAMPLE:

    #include "gc.h"

    int main()
    {
        int *pepa_int = gc_malloc("pepik", sizeof(int));
        float *pepa_float = gc_malloc("pepik", sizeof(float));

        int *karel_int = gc_malloc("karlik", sizeof(int));
        float *karel_float = gc_malloc("karlik", sizeof(float));

        if (pepa_int == NULL || pepa_float == NULL ||
            karel_int == NULL || karel_float == NULL)
            goto cleanup;   // SOME OF THEM COULD'VE BEEN ALLOCATED SUCCESSFULLY

        gc_free("karlik");  // KAREL GAVE UP ALL MEMORY HE HAS ALLOCATED
        *pepa_int = 42;     // PEPA CAN STILL USE HIS MEMORY

    cleanup:
        gc_free("pepik");
        gc_free("karlik");  // ( TRYING TO DEALLOCATE NEVER-ALLOCATED OR ALREADY
        gc_free("jarda");   //   DEALLOCATED MEMORY DOESN'T DO HARM )

        return 0;
    }
*/

#endif
