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
    int start; //because inbuild copy function needs it
    int count;    //because inbuild copy function needs it
    Instruction_type instruction;
} T_ITEM;

// Must be also for string, bool etc.?
bool f_less(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) < (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) < (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.s.str) < (item->op2->data.s.str));
    return false;
}

bool f_greater(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) > (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) > (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.s.str) > (item->op2->data.s.str));
    return print_error(INCOMPATIBLE_TYPE);
}

bool f_less_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) <= (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) <= (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.s.str) <= (item->op2->data.s.str));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) >= (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) >= (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.s.str) >= (item->op2->data.s.str));
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) == (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) == (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return strcmp(item->op1->data.s.str, item->op2->data.s.str); //is this correct for equal / not equal?
    else
        return print_error(INCOMPATIBLE_TYPE);
}

bool f_not_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.i) != (item->op2->data.i));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.d) != (item->op2->data.d));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return strcmp(item->op1->data.s.str, item->op2->data.s.str);
    else
        return print_error(INCOMPATIBLE_TYPE);
}

//TODO: ERROR CODES
int interpret(T_ITEM *testValue)
{
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
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: string
    case I_READLN:
        if (testValue->op1->type == Type_INT)
            //scanf("%d", &testValue->data.i);
            printf("I_READLN - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            //scanf("%lf", &testValue->data.d);
            printf("I_READLN - integer\n");
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: Strings --- Do we need this for bool? I dont think so.----
    //Is it concat for strings?
    case I_ADD:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i + testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d + testValue->op2->data.d;

        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
        {
            debug("I_ADD for strings\n");
        }
        //do we really need this condition? Naah i dont this so
        else if (((testValue->op1->type == Type_STRING) && (testValue->op2->type != Type_STRING)))
        {
            debug("Non compatible type for string concat\n");
            return print_error(INCOMPATIBLE_TYPE);
            break;
        }
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: Strings, ----Do we need this for string/bool? I dont think so.----
    case I_SUB:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i - testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d - testValue->op2->data.d;
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: Strings,bool, etc
    case I_MULTIPLY:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.i = testValue->op1->data.i * testValue->op2->data.i;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.d = testValue->op1->data.d * testValue->op2->data.d;
        else print_error(INCOMPATIBLE_TYPE);
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
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_LESS:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.b = f_less(&*testValue);
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.b = f_less(&*testValue);
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            testValue->result->data.b = f_less(&*testValue);
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
    //works fine
    case I_LEN:
        if (testValue->op1->type == Type_STRING)
        {
            testValue->result->data.i = length(&(testValue->op1->data.s));
            debug("I_LEN - string\n");
        }
        else
            return print_error(RUNTIME_OTHER);
        break;

    case I_COPY:
        if (testValue->op1->type == Type_STRING)
        {
            //error: incompatible types when assigning to type ‘cstring’ from type ‘struct cstring *’
            //testValue->result->data.s = copy(&(testValue->op1->data.s), testValue->start, testValue->count);
            debug("I_COPY - string\n");
        }
        else
            return print_error(RUNTIME_OTHER);
        break;

    //works fine
    case I_FIND:
        if (testValue->op1->type == Type_STRING)
        {
            testValue->result->data.i = find(&(testValue->op1->data.s), &(testValue->op2->data.s));
            debug("I_FIND - string\n");
        }
        else
            return print_error(RUNTIME_OTHER);
        break;

    // FIX for mastermind Vojtisek
    case I_SORT:
        if (testValue->op1->type == Type_STRING)
        {
            // testValue->result->data.s = sort(&(testValue->op1->data.s));
            debug("I_FIND - string\n");
        }
        else
            return print_error(RUNTIME_OTHER);
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
    item.instruction = I_FIND; //instruction type

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

/*
    item.op1 = malloc(sizeof(cstring));
    item.op2 = malloc(sizeof(cstring));
    item.result = malloc(sizeof(int));
    item.result->type = Type_INT;

    item.op1->type = Type_STRING;
    item.op2->type = Type_STRING;
    item.result->type = Type_INT;

    cstr_append_str(&(item.op1->data.s), "ahoj vojto ako sa mas");
    cstr_append_str(&(item.op2->data.s), "to");
    item.start = 1;
    item.count = 2;



    interpret(&item);

    printf("hodnota: %d", (item.result->data.i));

    return 0;
}

*/