#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define TOKEN_IDENTIFIER_MAX_LENGTH 127


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

/*
 * Get a keyword code from it's string representation.
 */
static enum token_keyword _get_keyword(char *name) {
    switch (name[0]) {
    case 'b':
        if (!strcmp(name, "begin"))
            return KEYWORD_BEGIN;

        if (!strcmp(name, "boolean"))
            return KEYWORD_BOOLEAN;

        break;

    case 'd':
        if (!strcmp(name, "do"))
            return KEYWORD_DO;

        break;

    case 'e':
        if (!strcmp(name, "else"))
            return KEYWORD_ELSE;

        if (!strcmp(name, "end"))
            return KEYWORD_END;

        break;

    case 'f':
        if (!strcmp(name, "false"))
            return KEYWORD_FALSE;

        if (!strcmp(name, "find"))
            return KEYWORD_FIND;

        if (!strcmp(name, "forward"))
            return KEYWORD_FORWARD;

        if (!strcmp(name, "function"))
            return KEYWORD_FUNCTION;

        break;

    case 'i':
        if (!strcmp(name, "if"))
            return KEYWORD_IF;

        if (!strcmp(name, "integer"))
            return KEYWORD_INTEGER;

        break;

    case 'r':
        if (!strcmp(name, "readln"))
            return KEYWORD_READLN;

        if (!strcmp(name, "real"))
            return KEYWORD_REAL;

        break;

    case 's':
        if (!strcmp(name, "sort"))
            return KEYWORD_SORT;

        if (!strcmp(name, "string"))
            return KEYWORD_STRING;

        break;

    case 't':
        if (!strcmp(name, "then"))
            return KEYWORD_THEN;

        if (!strcmp(name, "true"))
            return KEYWORD_TRUE;

        break;

    case 'v':
        if (!strcmp(name, "var"))
            return KEYWORD_VAR;

        break;

    case 'w':
        if (!strcmp(name, "while"))
            return KEYWORD_WHILE;

        if (!strcmp(name, "write"))
            return KEYWORD_WRITE;

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
