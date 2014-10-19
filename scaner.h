#ifndef __SCANER_H__
#define __SCANER_H__

#include <stdio.h>

#define TOKEN_IDENTIFIER_MAX_LENGTH 127


/*
 * Very early pre-alpha release.
 *
 * Can operate only when:
 *  - all tokens are separated by space
 *  - no floats
 *  - no strings
 *  - no comments
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
    COLON,          // :
    PARENTHESIS_L,  // (
    PARENTHESIS_R,  // )
};


/* List of all Token types */
enum token_type {
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
        char               *value_string; 
        enum token_keyword  value_keyword;
        enum token_symbol   value_symbol;
    } value;
} Token;


/* State of the lexical analyzer FSM */
enum lexer_state {
    START,
    ID_KEYWORD,
    STRING_LOADING,
    INT_LOADING,
    FLOAT_LOADING,
    COMMENT,
    MAYBE_ASSIGNMENT,
};


/* Get a token */
void get_token(Token *token, FILE *input);


#endif //__SCANER_H__
