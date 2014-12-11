/**
 * @file    debug.h
 * @brief   DEBUG (header)
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__


#include "scaner.h"
#include "instruction.h"

/* Macro for gcc to stop nagging about unused parameters */
#define IGNORE_PARAM(id) (void)(id)


#ifdef DEBUG

/**
 * Debbuging macro
 * Prints out message + info about file, line and the current function.
 * format DEBUG: file.c:line:[function]: Message
 */
#define debug(...) debug_printf(__FILE__, __LINE__, __func__, __VA_ARGS__)

#define debug_token(token) debug_print_token(token)

#define debug_print_instruction(instruction) debug_instruction_print(instruction)

#else

#define debug(...)
#define debug_token(token)
#define debug_print_instruction(instruction)

#endif



extern void debug_printf(const char* file, const int line, const char* func, const char *fmt, ...);
extern void debug_print_token(Token *token);
extern void debug_instruction_print(Instruction *instruction);



#endif //__DEBUG_H__
