#ifndef __SCANER_H__
#define __SCANER_H__

#include <stdio.h>

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
    KEYWORD_BEGIN,      // b
    KEYWORD_BOOLEAN,
    KEYWORD_DO,         // d
    KEYWORD_ELSE,       // e
    KEYWORD_END,
    KEYWORD_FALSE,      // f
    KEYWORD_FIND,
    KEYWORD_FORWARD,
    KEYWORD_FUNCTION,
    KEYWORD_IF,         // i
    KEYWORD_INTEGER,
    KEYWORD_READLN,     // r
    KEYWORD_REAL,
    KEYWORD_SORT,       // s
    KEYWORD_STRING,
    KEYWORD_THEN,       // t
    KEYWORD_TRUE,
    KEYWORD_VAR,        // v
    KEYWORD_WHILE,      // w
    KEYWORD_WRITE,
};


/* List of operations and other symbols */
enum token_symbol {
    ADDITION,       // +
    SUBSTRACTION,   // -
    MULTIPLICATION, // *
    DIVISION,       // /
    COMPARISON,     // =
    ASSIGNMENT,     // :=
    SEMICOLON,      // ;
    DOT,            // .
    DOUBLE_DOT,     // ..
    COLON,          // :
    PARENTHESIS_L,  // (
    PARENTHESIS_R,  // )
};


/* List of all Token types */
enum token_type {
    TOKEN_ERROR = 0,  //  --
    TOKEN_ID,         //  value_name
    TOKEN_INT,        //  value_int
    TOKEN_FLOAT,      //  value_float
    TOKEN_STRING,     // *value_string
    TOKEN_KEYWORD,    //  value_keyword
    TOKEN_SYMBOL,     //  value_symbol
    TOKEN_EOF,        //  --
};


/* Token which represens a lexeme. */
typedef struct {
    enum token_type type;
    union {
        int                 value_int;
        float               value_float;
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
    LEXER_INT_LOADING,
    LEXER_FLOAT_LOADING,
    LEXER_COMMENT,
    LEXER_MAYBE_ASSIGNMENT,
    LEXER_MAYBE_DOUBLE_DOT,
};


/* Get a token */
void get_token(Token *token, FILE *input);


#endif //__SCANER_H__
