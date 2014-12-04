/**
 * @file	common.c
 * @name	Common functions that could be used across modules
 * @author	Albert Uchytil (xuchyt03)
 ****************************************************************************/

#include "common.h"
#include "scaner.h"

/**
 * @brief Function converts char to int.
 * @param c char that's going to be converted
 * @returns integer value between 0 and 9
 *
 * Function converts char to numeric value represented by the char.
 * Returns 0 when the char is not a digit.
 */
inline int char_to_int(char c)
{
    return ('0' <= c && c <= '9') ? (int)(c - '0') : 0;
}

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

#ifdef DEBUG

const char *keywords[] = {
    "none",
    "begin",
    "boolean",
    "do",
    "else",
    "end",
    "false",
    "find",
    "forward",
    "function",
    "if",
    "integer",
    "readln",
    "real",
    "repeat",
    "sort",
    "string",
    "then",
    "true",
    "until",
    "var",
    "while",
    "write"
};

const char *symbols[] = {
    "+",
    "-",
    "*",
    "/",
    "=",
    ":=",
    ";",
    ".",
    "..",
    ",",
    "(",
    ")",
    ">",
    "<",
    ">=",
    "<=",
    "<>"
};

void debug_print_token(Token *token)
{
	switch (token->type) {
	case TOKEN_ERROR:
		debug("Error\n");
		break;
	case TOKEN_ID:
		debug("ID\n");
        debug("%s\n", token->value.value_name);
		break;
	case TOKEN_INT:
		debug("INT\n");
		break;
	case TOKEN_FLOAT:
		debug("FLOAT\n");
		break;
	case TOKEN_STRING:
		debug("STRING\n");
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
#endif

//__FILE__, __LINE__, __func__
