/**
 * @file    interpret.c
 * @name    Implementation of interpret
 * @author  Tomas Paulus (xpaulu01)
 *******************************************************************/

#include "ial.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cstring.h"
#include "buildin.h"

// TESTING PURPOSES
// T for Type of variable

/*typedef enum
{
    T_Undefined = 0,
    Type_INT,
    Type_DOUBLE,
    Type_STRING,
    Type_BOOL,
} Type;*/

typedef enum
{
    Type_OTHER = 0,  //  --
    Type_INT,        //  value_int
    Type_DOUBLE,     //  value_double
    Type_CHAR,       //  value_char
    Type_STRING,     //  value_char*
    Type_CSTRING,    //  value_cstring
    Type_BOOL,      // value bool
} Type;

typedef union
{
    double d;
    int i;
    cstring s;
    bool b;
} Value_data;

typedef struct
{
    Value_data data;
    Type type;
} My_value;

typedef enum
{
    I_IF = 1,
    I_ELSE,
    I_WHILE,
    I_WRITE,
    I_READLN,
    I_ADD,
    I_SUB,
    I_MULTIPLY,
    I_DIV,
    I_LESS,
    I_GREATER,
    I_LESS_EQUAL,
    I_GREATER_EQUAL,
    I_EQUAL,
    I_NOT_EQUAL,
    I_LEN,
    I_COPY,
    I_FIND,
    I_SORT,
} Instruction_type;

typedef struct
{
    My_value *op1;
    My_value *op2;
    My_value *result;
    Instruction_type instruction;
} T_ITEM;

// Must be also for string, bool etc.?
bool f_less(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) < (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) < (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) > (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) > (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_less_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) <= (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) <= (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) >= (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) >= (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) == (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) == (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_not_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) != (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) != (item->op2->data.d));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

//TODO: ERROR CODES
int interpret(T_ITEM *testValue)
{
    //TODO: IF?
    switch (testValue->instruction)
    {
    case I_WRITE:
        if (testValue->op1->type == Type_INT)
            printf("%d\n", (testValue->op1->data.i));
        else if (testValue->op1->type == Type_DOUBLE)
            printf("%f\n", testValue->op1->data.d);
        else if (testValue->op1->type == Type_STRING)
            printf("%s\n", testValue->op1->data.s.str);
        else if (testValue->op1->type == Type_BOOL)
            printf("%s", testValue->op1->data.b ? "true\n" : "false\n");
        else print_error(SYNTAX_ERROR);
        break;
    //TODO:
    case I_READLN:
        if (testValue->op1->type == Type_INT)
            //scanf("%d", &testValue->data.i);
            printf("I_READLN - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            //scanf("%lf", &testValue->data.d);
            printf("I_READLN - integer\n");
        else print_error(SYNTAX_ERROR);
        break;

    //TODO: Strings, bool, etc.
    case I_ADD:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i + testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d + testValue->op2->data.d;

        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            printf("%s\n", testValue->op1->data.s.str);
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            printf("%s", testValue->op1->data.b ? "true\n" : "false\n");
        else print_error(SYNTAX_ERROR);
        break;

    //TODO: Strings,bool, etc
    case I_SUB:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i - testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d - testValue->op2->data.d;

        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            printf("I_SUB - string\n");
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            printf("I_SUB - bool\n");
        break;

    //TODO: Strings,bool, etc
    case I_MULTIPLY:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i * testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d * testValue->op2->data.d;

        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            printf("I_MULTIPLY - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_MULTIPLY - bool\n");
        break;

    //TODO: strings,bool, modulo for int?
    case I_DIV:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
        {
            testValue->result->type = Type_DOUBLE;
            testValue->result->data.d = testValue->op1->data.i / testValue->op2->data.i;
            testValue->op1->type = Type_DOUBLE;
            testValue->op1->data.d = testValue->result->data.d;
        }
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d / testValue->op2->data.d;

        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            printf("I_DIV - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_DIV - bool\n");
        break;

    case I_LESS:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.b = f_less(&*testValue);
        else if (testValue->op1->type == Type_DOUBLE)
            testValue->result->data.b = f_less(&*testValue);

        else if (testValue->op1->type == Type_STRING)
            printf("I_LESS - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_LESS - bool\n");
        break;

    case I_GREATER:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.b = f_greater(&*testValue);
            debug("I_GREATER - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.b = f_greater(&*testValue);
            debug("I_GREATER - integer\n");
        }

        else if (testValue->op1->type == Type_STRING)
            printf("I_GREATER - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_GREATER - bool\n");
        break;

    case I_LESS_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.b = f_less_equal(&*testValue);
            debug("I_LESS_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.b = f_less_equal(&*testValue);
            debug("I_LESS_EQUAL - double\n");
        }

        else if (testValue->op1->type == Type_STRING)
            printf("I_LESS_EQUAL - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_LESS_EQUAL - bool\n");
        break;

    case I_GREATER_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.b = f_greater_equal(&*testValue);
            debug("I_GREATER_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.b = f_greater_equal(&*testValue);
            debug("I_GREATER_EQUAL - double\n");
        }

        else if (testValue->op1->type == Type_STRING)
            printf("I_GREATER_EQUAL - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_GREATER_EQUAL - bool\n");
        break;

    case I_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.b = f_equal(&*testValue);
            debug("I_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.b = f_equal(&*testValue);
            debug("I_EQUAL - double\n");
        }

        else if (testValue->op1->type == Type_STRING)
            printf("I_EQUAL - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_EQUAL - bool\n");
        break;

    case I_NOT_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.b = f_not_equal(&*testValue);
            debug("I_NOT_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.b = f_not_equal(&*testValue);
            debug("I_NOT_EQUAL - double\n");
        }

        else if (testValue->op1->type == Type_STRING)
            printf("I_NOT_EQUAL - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_NOT_EQUAL - bool\n");
        break;

    case I_LEN:
        if (testValue->op1->type == Type_STRING)
        {
            //TODO: incompatible type cstring. Expecting *
            //testValue->result->data.i = length(testValue->op1->data.s);
            debug("I_LEN - string\n");
        }
        else if (testValue->op1->type == Type_BOOL)
            printf("I_LEN - bool\n");
        break;

    case I_COPY:
        if (testValue->op1->type == Type_INT)
            printf("I_COPY - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            printf("I_COPY - double\n");
        else if (testValue->op1->type == Type_STRING)
            printf("I_COPY - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_COPY - bool\n");
        break;

    case I_FIND:
        if (testValue->op1->type == Type_INT)
            printf("I_FIND - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            printf("I_FIND - double\n");
        else if (testValue->op1->type == Type_STRING)
            printf("I_FIND - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_FIND - bool\n");
        break;
    case I_SORT:
        if (testValue->op1->type == Type_INT)
            printf("I_SORT - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            printf("I_SORT - double\n");
        else if (testValue->op1->type == Type_STRING)
            printf("I_SORT - string\n");
        else if (testValue->op1->type == Type_BOOL)
            printf("I_SORT - bool\n");
        break;
    default: break;
    }
    return 0;
}



// TESTING PURPOSES
/*
int main(void)
{


    T_ITEM item;
    item.instruction = I_LESS; //instruction type

*/

/* if you want to test INT
item.op1 = malloc(sizeof(int));
item.op2 = malloc(sizeof(int));
item.result = malloc(sizeof(int));

item.op1->type = Type_INT;
item.op2->type = Type_INT;

item.op1->data.i = 10;
item.op2->data.i = 4;
*/

/*if you want to test Double remove
item.op1 = malloc(sizeof(double));
item.op2 = malloc(sizeof(double));
item.result = malloc(sizeof(double));

item.op1->type = Type_DOUBLE;
item.op2->type = Type_DOUBLE;

item.op1->data.d = 2.00;
item.op2->data.d = 4.00;

item.result->type = item.op1->type;
item.result->data.i = 0.0; */

//interpret(&item);

// return 0;
//}

