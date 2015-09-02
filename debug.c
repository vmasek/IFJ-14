/**
 * @file    debug.c
 * @brief   Debugging functions
 * @author  Vojtech Mašek (xmasek15)
 ****************************************************************************/



#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "scaner.h"

#ifdef DEBUG
static const char *keywords[] = {
    "none",
    "and",
    "begin",
    "boolean",
    "do",
    "else",
    "end",
    "false",
    "forward",
    "function",
    "if",
    "integer",
    "not",
    "or",
    "readln",
    "real",
    "repeat",
    "string",
    "then",
    "true",
    "until",
    "var",
    "while",
    "write",
    "xor",
};

static const char *symbols[] = {
    "+",
    "-",
    "-",
    "*",
    "/",
    "=",
    ":=",
    ";",
    ".",
    "..",
    ":",
    ",",
    "(",
    ")",
    ">",
    "<",
    ">=",
    "<=",
    "<>"
};

/*
const char *TYPE_NAMES[] = {
	"TYPE_OTHER",
	"TYPE_INT",
	"TYPE_REAL",
	"TYPE_STRING",
	"TYPE_BOOL"
};*/

static const char *INSTRUCTION_NAME[] = {
	"I_ERR",
	"I_WRITE",
	"I_READLN",
	"I_PUSH",
	"I_ASSIGN",
	"I_ADD",
	"I_NEG",
	"I_SUB",
	"I_MUL",
	"I_DIV",
	"I_INC",
	"I_DEC",
	"I_LESS",
	"I_GREATER",
	"I_LESS_EQUAL",
	"I_GREATER_EQUAL",
	"I_EQUAL",
	"I_NOT_EQUAL",
	"I_AND",
	"I_OR",
	"I_XOR",
	"I_NOT",
	"I_LEN",
	"I_COPY",
	"I_FIND",
	"I_SORT",
	"I_PREP",
	"I_PASS",
	"I_JMP",
	"I_CALL",
	"I_HALT",
	"I_NOP"
};

#endif

inline void debug_printf(const char* file, const int line, const char* func, const char *fmt, ...)
{
	va_list arg;
	static char format[1000]={0};
	sprintf(format, "DEBUG: %s:%d:[%s]:\t\t", file, line, func);
	va_start(arg, fmt);
	strcat(format, fmt);
	vfprintf(stderr, format, arg);
	va_end(arg);
}

void debug_print_token(Token *token)
{
	switch (token->type) {
	case TOKEN_ID:
		debug("ID\n");
        debug("%s\n", token->value.value_name);
		break;
	case TOKEN_INT:
		debug("INT\n");
        debug("%d\n", token->value.value_int);
		break;
	case TOKEN_FLOAT:
		debug("FLOAT\n");
        debug("%f\n", token->value.value_float);
		break;
	case TOKEN_STRING:
		debug("STRING\n");
        debug("%s\n", token->value.value_string->str);
		break;
	case TOKEN_KEYWORD:
		debug("KEYWORD\n");
        debug("%s\n", keywords[token->value.value_keyword]);
		break;
	case TOKEN_SYMBOL:
		debug("SYMBOL\n");
        debug("%s\n", symbols[token->value.value_symbol]);
		break;
	case TOKEN_EOF:
		debug("EOF\n");
		break;
	default:
		break;
	}
}

void debug_instruction_print(Instruction *instruction)
{
#ifndef DEBUG
	char *INSTRUCTION_NAME[1];
#endif
	unsigned int i = 1;
	Instruction *temp = instruction;
	do
	{
		if(i>50)
		{
			fprintf(stderr, "\ttoo many instructions\n");
			return;
		}
		fprintf(stderr, "\tInstruction %d:\t%s\n", i, INSTRUCTION_NAME[temp->instruction]);
		i++;
        if (i > 100)
            break;
	} while ((temp=temp->next_instruction));
}



