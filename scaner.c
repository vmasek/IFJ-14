#include "scaner.h"
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
 *  - limited strings (no special chars)
 *  - no errors in syntax
 *
 * Author: Adam Samalik
 *
 */


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
 * Append a character to a string
 */
static void strcatc(char * destination, char c) {
    char buffer[2] = "\0\0";
    buffer[0] = c;
    strcat(destination, buffer);
}

/*
 * Load (another) lexeme from the source file
 * and return it's Token representation
 */
void get_token(Token *token, FILE *input) {
    char symbol = 0;
    enum lexer_state state = LEXER_START;

    int token_name_pos = 0;
    enum token_keyword keyword;

    char buffer[100] = "";

    while (1) {
        symbol = getc(input);
        switch (state) {
        case LEXER_START:
            if (isspace(symbol))
                break;

            if (symbol == EOF) {
                token->type = TOKEN_EOF;
                return;
            }

            if (symbol >= 'a' && symbol <= 'z') { // FIXME: more rules!
                token->value.value_name[token_name_pos] = symbol;
                token_name_pos ++;
                state = LEXER_ID_KEYWORD;
                break;
            }

            if (symbol >= '0' && symbol <= '9') {
                strcatc(buffer, symbol);
                token->type = TOKEN_INT;
                state = LEXER_INT_LOADING;
                break;
            }

            switch (symbol) {
            case '+':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = ADDITION;
                return;

            case '-':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = SUBSTRACTION;
                return;

            case '*':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = MULTIPLICATION;
                return;

            case '/':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = DIVISION;
                return;

            case '=':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = COMPARISON;
                return;

            case ':':
                token->type = TOKEN_SYMBOL;
                state = LEXER_MAYBE_ASSIGNMENT;
                break; // can be ':=' or ':'

            case ';':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = SEMICOLON;
                return;

            case '.':
                token->type = TOKEN_SYMBOL;
                state = LEXER_MAYBE_DOUBLE_DOT;
                break; // can be '..' or '.'

            case '(':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = PARENTHESIS_L;
                return;

            case ')':
                token->type = TOKEN_SYMBOL;
                token->value.value_symbol = PARENTHESIS_R;
                return;


            case '\'':
                token->type = TOKEN_STRING;
                state = LEXER_STR_START;
                break;

            case '{':
                state = LEXER_COMMENT;
                break;

            }
            break;


        case LEXER_COMMENT:
            if (symbol == '}') {
                state = LEXER_START;
                break;
            }
            break;


        case LEXER_STR_START:
            if (symbol == '\'') {
                state = LEXER_STR_AP;
                break;
            }

            token->value.value_string = cstr_create_str(&symbol);
            state = LEXER_STR_LOAD;
            break;


        case LEXER_STR_LOAD:
            if (symbol == '\'') {
                state = LEXER_STR_AP;
                break;
            }

            cstr_append_str(token->value.value_string, &symbol);
            break;


        case LEXER_STR_AP:
            if (symbol == '\'') {
                cstr_append_str(token->value.value_string, &symbol);
                state = LEXER_STR_LOAD;
                break;
            }
    
            if (symbol =='#') {
                state = LEXER_STR_SPEC;
                break;
            }

            //ungetchar
            return;


        case LEXER_STR_SPEC:
            if (symbol == '\'') {
                // FIXME transform that number into a char and append that shit
                cstr_append_str(token->value.value_string, "[SPECIAL]");
                state = LEXER_STR_LOAD;
                break;
            }

            // FIXME
            // load another number if it's number and if not, go fuck yourself
            break;


        case LEXER_MAYBE_ASSIGNMENT:
            if (symbol == '=') {
                token->value.value_symbol = ASSIGNMENT;
                return;
            } else {
                // FIXME: ungetc()
                token->value.value_symbol = COLON;
                return;
            }

            break;


        case LEXER_MAYBE_DOUBLE_DOT:
            if (symbol == '.') {
                token->value.value_symbol = DOUBLE_DOT;
                return;
            } else {
                // FIXME: ungetc()
                token->value.value_symbol = DOT;
                return;
            }

            break;


        case LEXER_INT_LOADING:
            if (symbol >= '0' && symbol <= '9') {
                strcatc(buffer, symbol);
                break;

            } else if (symbol == '.' || symbol == 'e') {
                strcatc(buffer, symbol);
                state = LEXER_FLOAT_LOADING;
                token->type = TOKEN_FLOAT;
                token->value.value_float = (float)token->value.value_int;
                break;

            } else {
                token->value.value_int = (int)atof(buffer);
                return; // FIXME: ungetc
            }

            break;

        case LEXER_FLOAT_LOADING:
            if (! (symbol >= '0' && symbol <= '9')) {
                token->value.value_float = atof(buffer);
                // FIXME ungetc()
                return;
            }

            strcatc(buffer, symbol);
            break;

        case LEXER_ID_KEYWORD:
            if (isspace(symbol)) { // FIXME: tokens are not separated by spaces!
                token->value.value_name[token_name_pos] = 0;
                token_name_pos = 0;
                keyword = _get_keyword(token->value.value_name);
                if (!keyword) {
                    token->type = TOKEN_ID;
                } else {
                    token->type = TOKEN_KEYWORD;
                    token->value.value_keyword = keyword;
                }
                return;
            } 

            if (symbol == EOF) {
                token->type = TOKEN_EOF;
                return;
            }

            if (symbol >= 'a' && symbol <= 'z') { // FIXME: more rules!
                token->value.value_name[token_name_pos] = symbol;
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
    FILE *input = fopen("test", "r");
    Token *token = malloc(sizeof(Token *));

    while(1) {
        get_token(token, input);
        if (token->type == TOKEN_ID)
            printf (" TOKEN_ID          %s", token->value.value_name);
        else if (token->type == TOKEN_KEYWORD)
            printf (" TOKEN_KEYWORD     %d", token->value.value_keyword);
        else if (token->type == TOKEN_INT)
            printf (" TOKEN_INT         %d", token->value.value_int);
        else if (token->type == TOKEN_FLOAT)
            printf (" TOKEN_FLOAT       %f", token->value.value_float);
        else if (token->type == TOKEN_SYMBOL)
            printf (" TOKEN_SYMBOL      %d", token->value.value_int);
        else if (token->type == TOKEN_STRING)
            printf (" TOKEN_STRING      %s", token->value.value_string->str);
        else
            break;
        printf("\n");
    }


    fclose(input);

    return 0;
} */
