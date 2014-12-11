/**
 * @file    instructions.c
 * @brief   Instructions
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/

#include <stdlib.h>

#include "errors.h"
#include "gc.h"
#include "instruction.h"

/* GC tag for instructions */
#define GC_INSTR "instructions"

/* DEFINITION OF MISCELLANEOUS FUNCTIONS */
int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
              Instruction *alt_instr)
{
    Instruction *new_ptr = NULL;
    if ((new_ptr = gc_malloc(GC_INSTR, sizeof(Instruction))) == NULL) {
        return INTERNAL_ERROR;
    }
    *new_ptr = (Instruction) {
        .instruction = type,
        .index = index,
        .next_instruction = NULL,
        .alt_instruction = alt_instr
    };

    if (*instr_ptr == NULL) {
        *instr_ptr = new_ptr;
    } else {
        (*instr_ptr)->next_instruction = new_ptr;
    }

    return SUCCESS;
}

void free_instr(void)
{
    gc_free(GC_INSTR);
}
