#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define TOKEN_IDENTIFIER_MAX_LENGTH 32


/*
 * Very early pre-alpha release.
 *
 * Can operate only when:
 *  - all tokens are separated by space
 *  - only one digit ints
 *  - no floats
 *  - no strings
 *  - no comments
 *  - no errors in syntax
 *
 * Author: Adam Samalik
 *
 */


/*
 * List of all possible keywords.
 */
enum token_keyword {
    NOT_A_KEYWORD = 0,   
    BEGIN,      // b
    BOOLEAN,
    DO,         // d
    ELSE,       // e
    END,
    FALSE,      // f
    FIND,
    FORWARD,
    FUNCTION,
    IF,         // i
    TOKEN_INTEGER,
    READLN,     // r
    REAL,
    SORT,       // s
    STRING,
    THEN,       // t
    TRUE,
    VAR,        // v
    WHILE,      // w
    WRITE,
};

/*
 * Get a keyword code from it's string representation.
 */
static enum token_keyword _get_keyword(char *name) {
    switch (name[0]) {
    case 'b':
        if (!strcmp("begin", name))
            return   BEGIN;

        if (!strcmp("boolean", name))
            return   BOOLEAN;

        break;

    case 'd':
        if (!strcmp("do", name))
            return   DO;

        break;

    case 'e':
        if (!strcmp("else",name))
            return   ELSE;

        if (!strcmp("end",name))
            return   END;

        break;

    case 'f':
        if (!strcmp("false",name))
            return   FALSE;

        if (!strcmp("find",name))
            return   FIND;

        if (!strcmp("forward",name))
            return   FORWARD;

        if (!strcmp("function",name))
            return   FUNCTION;

        break;

    case 'i':
        if (!strcmp("if",name))
            return   IF;

        if (!strcmp("integer",name))
            return   TOKEN_INTEGER;

        break;

    case 'r':
        if (!strcmp("readln",name))
            return   READLN;

        if (!strcmp("real",name))
            return   REAL;

        break;

    case 's':
        if (!strcmp("sort",name))
            return   SORT;

        if (!strcmp("string",name))
            return   STRING;

        break;

    case 't':
        if (!strcmp("then",name))
            return   THEN;

        if (!strcmp("true",name))
            return   TRUE;

        break;

    case 'v':
        if (!strcmp("var",name))
            return   VAR;

        break;

    case 'w':
        if (!strcmp("while",name))
            return   WHILE;

        if (!strcmp("write",name))
            return   WRITE;

        break;
    }
    return NOT_A_KEYWORD;
}

/*
 * List of operations and other symbols
 */
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

/*
 * List of all Token types
 */
enum token_type {
    TOKEN_ID,         //  value_name
    TOKEN_INT,        //  value_int
    TOKEN_FLOAT,      //  value_float
    TOKEN_STRING,     // *value_string
    TOKEN_KEYWORD,    //  value_keyword
    TOKEN_SYMBOL,     //  value_symbol
    TOKEN_EOF,        //  --
};

/*
 * Token which represens a lexeme.
 */
typedef struct {
    enum token_type type;
    union {
        int                 value_int;
        float               value_float;
        char                value_name[TOKEN_IDENTIFIER_MAX_LENGTH];
        char               *value_string; 
        enum token_keyword  value_keyword;
        enum token_symbol   value_symbol;
    };
} Token;

/*
 * State of the lexical analyzer FSM
 */
enum lexer_state {
    START,
    ID_KEYWORD,
    STRING_LOADING,
    COMMENT,
    MAYBE_ASSIGNMENT,
};

static FILE *input = NULL;

/*
 * Load (another) lexeme from the source file
 * and return it's Token representation
 */
void get_token(Token *token) {
    char symbol = 0;
    enum lexer_state state = START;

    int token_name_pos = 0;
    enum token_keyword keyword;

    while (1) {
        symbol = getc(input);
        switch (state) {
        case START:
            if (isspace(symbol))
                break;

            if (symbol == EOF) {
                token->type = TOKEN_EOF;
                return;
            }

            if (symbol >= 'a' && symbol <= 'z') { // FIXME: more rules!
                token->value_name[token_name_pos] = symbol;
                token_name_pos ++;
                state = ID_KEYWORD;
                break;
            }

            if (symbol >= '0' && symbol <= '9') {
                token->value_int = symbol - '0'; // FIXME: ugly conversion!
                token->type = TOKEN_INT;
                return;
            }

            switch (symbol) {
            case '+':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = ADDITION;
                return;

            case '-':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = SUBSTRACTION;
                return;

            case '*':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = MULTIPLICATION;
                return;

            case '/':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = DIVISION;
                return;

            case '=':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = COMPARISON;
                return;

            case ':':
                token->type = TOKEN_SYMBOL;
                state = MAYBE_ASSIGNMENT;
                break;

            case ';':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = SEMICOLON;
                return;

            case '.':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = DOT;
                return;

            case '(':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = PARENTHESIS_L;
                return;

            case ')':
                token->type = TOKEN_SYMBOL;
                token->value_symbol = PARENTHESIS_R;
                return;
            }

            break;

        case MAYBE_ASSIGNMENT:
            if (symbol == '=') {
                token->value_symbol = ASSIGNMENT;
                return;
            } else {
                // FIXME: return the symbol back!
                token->value_symbol = COLON;
                return;
            }

            break;

        case ID_KEYWORD:
            if (isspace(symbol)) { // FIXME: tokens are not separated by spaces!
                token->value_name[token_name_pos] = 0;
                token_name_pos = 0;
                keyword = _get_keyword(token->value_name);
                if (!keyword) {
                    token->type = TOKEN_ID;
                } else {
                    token->type = TOKEN_KEYWORD;
                    token->value_keyword = keyword;
                }
                return;
            } 

            if (symbol == EOF) {
                token->type = TOKEN_EOF;
                return;
            }

            if (symbol >= 'a' && symbol <= 'z') { // FIXME: more rules!
                token->value_name[token_name_pos] = symbol;
                token_name_pos ++;
                break;
            }

            break;
        }
    }
}

/* 
 * Just for testing purpose
 */

/*
int main() {
    input = fopen("test", "r");
    Token *token = malloc(sizeof(Token *));

    while(1) {
        get_token(token);
        if (token->type == TOKEN_ID)
            printf (" TOKEN_ID %s, ", token->value_name);
        else if (token->type == TOKEN_KEYWORD)
            printf (" TOKEN_KEYWORD %d, ", token->value_keyword);
        else if (token->type == TOKEN_INT)
            printf (" TOKEN_INT %d, ", token->value_int);
        else if (token->type == TOKEN_SYMBOL)
            printf (" TOKEN_SYMBOL %d, ", token->value_int);
        else
            break;
    }

    printf("\n");

    fclose(input);

    return 0;
} */
