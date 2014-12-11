#ifndef __SCANER_H__
#define __SCANER_H__

#include <stdio.h>
#include <ctype.h>
#define TOKEN_IDENTIFIER_MAX_LENGTH 127

#include "cstring.h"

/*
 * Very early pre-alpha release.
 *
 * Can operate only when:
 *  - limited strings (no special chars)
 *  - no errors in syntax
 *
 * Author: Adam Samalik
 *
 */


/* List of all possible keywords. */
enum token_keyword {
    NOT_A_KEYWORD = 0,
    KEYWORD_AND,        // a
    KEYWORD_BEGIN,      // b
    KEYWORD_BOOLEAN,
    KEYWORD_DO,         // d
    KEYWORD_ELSE,       // e
    KEYWORD_END,
    KEYWORD_FALSE,      // f
    KEYWORD_FOR,
    KEYWORD_FORWARD,
    KEYWORD_FUNCTION,
    KEYWORD_IF,         // i
    KEYWORD_INTEGER,
    KEYWORD_NOT,        // n
    KEYWORD_OR,         // o
    KEYWORD_READLN,     // r
    KEYWORD_REAL,
    KEYWORD_REPEAT,
    KEYWORD_STRING,     // s
    KEYWORD_THEN,       // t
    KEYWORD_TRUE,
    KEYWORD_TO,
    KEYWORD_UNTIL,      // u
    KEYWORD_VAR,        // v
    KEYWORD_WHILE,      // w
    KEYWORD_WRITE,
    KEYWORD_XOR,        // x
};


/* List of operations and other symbols */
enum token_symbol {
    ADDITION,       // +
    SUBTRACTION,    // -
    NEGATION,       // unary -
    MULTIPLICATION, // *
    DIVISION,       // /
    COMPARISON,     // =
    ASSIGNMENT,     // :=
    SEMICOLON,      // ;
    DOT,            // .
    DOUBLE_DOT,     // ..
    COLON,          // :
    COMMA,          // ,
    PARENTHESIS_L,  // (
    PARENTHESIS_R,  // )
    GREATER_THAN,   // >
    LESS_THAN,      // <
    GREATER_EQUAL,  // >=
    LESS_EQUAL,     // <=
    NOT_EQUAL,      // <>
};


/* List of all Token types */
enum token_type {
    TOKEN_EOF,        //  --
    TOKEN_ID,         //  value_name
    TOKEN_INT,        //  value_int
    TOKEN_FLOAT,      //  value_float
    TOKEN_STRING,     // *value_string
    TOKEN_KEYWORD,    //  value_keyword
    TOKEN_SYMBOL,     //  value_symbol
};


/* Token which represens a lexeme. */
typedef struct {
    enum token_type type;
    union {
        int                 value_int;
        double              value_float;
        char                value_name[TOKEN_IDENTIFIER_MAX_LENGTH];
        cstring            *value_string;
        enum token_keyword  value_keyword;
        enum token_symbol   value_symbol;
    } value;
} Token;


/* State of the lexical analyzer FSM */
enum lexer_state {
    LEXER_START,
    LEXER_ID_KEYWORD,
    LEXER_STR_START,
    LEXER_STR_AP,
    LEXER_STR_LOAD,
    LEXER_STR_SPEC,
    LEXER_STR_SPEC_BIN,
    LEXER_STR_SPEC_BIN_FIRST,
    LEXER_STR_SPEC_OCT,
    LEXER_STR_SPEC_OCT_FIRST,
    LEXER_STR_SPEC_HEX,
    LEXER_STR_SPEC_HEX_FIRST,
    LEXER_INT_LOADING,
    LEXER_FLOAT_LOADING_FIRST,
    LEXER_FLOAT_LOADING,
    LEXER_FLOAT_EXP_LOADING_FIRST,
    LEXER_FLOAT_EXP_LOADING,
    LEXER_COMMENT,
    LEXER_MAYBE_ASSIGNMENT,
    LEXER_MAYBE_DOUBLE_DOT,
    LEXER_MAYBE_GREATER_EQUAL,
    LEXER_MAYBE_LESS_EQUAL, // also applies for <>
    LEXER_BINARY_LOADING,
    LEXER_BINARY_LOADING_FIRST,
    LEXER_OCT_LOADING,
    LEXER_OCT_LOADING_FIRST,
    LEXER_HEX_LOADING,
    LEXER_HEX_LOADING_FIRST,
};


/* Get a token */
int get_token(Token *token, FILE *input);

/* Unget a token */
void unget_token(Token *token);

/* Frees token */
void free_token(Token *token);

int convert_binary(int binary_number);

#endif //__SCANER_H__
