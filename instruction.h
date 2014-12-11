/**
 * @file    instructions.h
 * @brief   Instructions (header)
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__


typedef enum
{
    I_ERR,				// error !!!
    I_WRITE,    		// printf
    I_READLN,   		// scanf
    I_PUSH,   			// push
    I_ASSIGN,   		// :=
    I_ADD,      		// +
    I_NEG,              // unary -
    I_SUB,      		// -
    I_MUL,				// *
    I_DIV,      		// /
    I_INC,              // ++
    I_DEC,              // --
    I_LESS,     		// <
    I_GREATER,  		// >
    I_LESS_EQUAL,   	// <=
    I_GREATER_EQUAL,    // >=
    I_EQUAL,    		// ==
    I_NOT_EQUAL,    	// !=
    I_AND,              // and
    I_OR,               // or
    I_XOR,              // xor
    I_NOT,              // not
    I_LEN,  			// length
    I_COPY, 			// copy
    I_FIND, 			// find
    I_SORT, 			// sort
    I_PREP,             // push uninited local var
    I_PASS,             // push argument
	I_JMP,				// jump
	I_CALL,				// call
	I_HALT,				// halt
	I_NOP,				// nop
} Instruction_type;


typedef struct Instruction
{
	Instruction_type	instruction;
	int					index;
	struct Instruction	*next_instruction;
	struct Instruction	*alt_instruction;
}Instruction;


int gen_instr(Instruction **instr_ptr, Instruction_type type, int index, Instruction *alt_instr);
void free_instr(void);


#endif //__INSTRUCTIONS_H__

