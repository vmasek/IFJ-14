/**
 * @file    interpret.c
 * @brief   Implementation of interpret
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *******************************************************************/

#include "interpret.h"



// Must be also for string, bool etc.?
bool f_less(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) < (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) < (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.string.str) < (item->op2->data.string.str));
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) < (item->op2->data.boolean));
    return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) > (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) > (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.string.str) > (item->op2->data.string.str));
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) > (item->op2->data.boolean));
    return print_error(INCOMPATIBLE_TYPE);
}

bool f_less_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) <= (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) <= (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.string.str) <= (item->op2->data.string.str));
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) <= (item->op2->data.boolean));

    return print_error(INCOMPATIBLE_TYPE);
}

bool f_greater_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) >= (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) >= (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return ((item->op1->data.string.str) >= (item->op2->data.string.str));
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) >= (item->op2->data.boolean));
    return print_error(INCOMPATIBLE_TYPE);
}

bool f_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) == (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) == (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return strcmp(item->op1->data.string.str, item->op2->data.string.str); //is this correct for equal / not equal?
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) == (item->op2->data.boolean));
    return print_error(INCOMPATIBLE_TYPE);
}

bool f_not_equal(T_ITEM *item)
{
    if ((item->op1->type == Type_INT)  && (item->op2->type == Type_INT))
        return ((item->op1->data.integer) != (item->op2->data.integer));
    else if ((item->op1->type == Type_DOUBLE) && (item->op2->type == Type_DOUBLE))
        return ((item->op1->data.real) != (item->op2->data.real));
    else if ((item->op1->type == Type_STRING) && (item->op2->type == Type_STRING))
        return strcmp(item->op1->data.string.str, item->op2->data.string.str); // is this correct for equal/notequal?
    else if ((item->op1->type == Type_BOOL) && (item->op2->type == Type_BOOL))
        return ((item->op1->data.boolean) != (item->op2->data.boolean));
    return print_error(INCOMPATIBLE_TYPE);
}

//TODO: ERROR CODES
int interpret(T_ITEM *testValue)
{
    switch (testValue->instruction)
    {
    case I_WRITE:
        if (testValue->op1->type == Type_INT)
            printf("%d\n", (testValue->op1->data.integer));
        else if (testValue->op1->type == Type_DOUBLE)
            printf("%f\n", testValue->op1->data.real);
        else if (testValue->op1->type == Type_STRING)
            printf("%s\n", testValue->op1->data.string.str);
        else if (testValue->op1->type == Type_BOOL)
            printf("%s", testValue->op1->data.boolean ? "true\n" : "false\n");
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: string
    case I_READLN:
        if (testValue->op1->type == Type_INT)
            //scanf("%d", &testValue->data.integer);
            printf("I_READLN - integer\n");
        else if (testValue->op1->type == Type_DOUBLE)
            //scanf("%lf", &testValue->data.real);
            printf("I_READLN - integer\n");
        else print_error(INCOMPATIBLE_TYPE);
        break;

    // can be string := string ?
    case I_ASSIGN:
        if (testValue->op1->type == Type_INT)
            testValue->result->data.integer = testValue->op1->data.integer;
        else if (testValue->op1->type == Type_DOUBLE)
            testValue->result->data.real = testValue->op1->data.real;
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: Strings --- Do we need this for bool? I dont think so.----
    //Is it concat for strings?
    case I_ADD:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.integer = testValue->op1->data.integer + testValue->op2->data.integer;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.real = testValue->op1->data.real + testValue->op2->data.real;

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
            testValue->result->data.integer = testValue->op1->data.integer - testValue->op2->data.integer;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.real = testValue->op1->data.real - testValue->op2->data.real;
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: Strings,bool, etc
    case I_MULTIPLY:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.integer = testValue->op1->data.integer * testValue->op2->data.integer;
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.real = testValue->op1->data.real * testValue->op2->data.real;
        else print_error(INCOMPATIBLE_TYPE);
        break;

    //TODO: strings,bool, modulo for int?
    case I_DIV:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
        {
            testValue->result->type = Type_DOUBLE;
            testValue->result->data.real = testValue->op1->data.integer / testValue->op2->data.integer;
            testValue->op1->type = Type_DOUBLE;
            testValue->op1->data.real = testValue->result->data.real;
        }
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.real = testValue->op1->data.real / testValue->op2->data.real;
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_LESS:
        if ((testValue->op1->type == Type_INT) && (testValue->op2->type == Type_INT))
            testValue->result->data.boolean = f_less(&*testValue);
        else if ((testValue->op1->type == Type_DOUBLE) && (testValue->op2->type == Type_DOUBLE))
            testValue->result->data.boolean = f_less(&*testValue);
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            testValue->result->data.boolean = f_less(&*testValue);
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_less(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_GREATER:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.boolean = f_greater(&*testValue);
            debug("I_GREATER - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.boolean = f_greater(&*testValue);
            debug("I_GREATER - integer\n");
        }
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
        {
            testValue->result->data.boolean = f_greater(&*testValue);
            debug("I_GREATER - string\n");
        }
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_greater(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_LESS_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.boolean = f_less_equal(&*testValue);
            debug("I_LESS_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.boolean = f_less_equal(&*testValue);
            debug("I_LESS_EQUAL - double\n");
        }
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
        {
            testValue->result->data.boolean = f_less_equal(&*testValue);
            debug("I_GREATER - string\n");
        }
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_less_equal(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_GREATER_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.boolean = f_greater_equal(&*testValue);
            debug("I_GREATER_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.boolean = f_greater_equal(&*testValue);
            debug("I_GREATER_EQUAL - double\n");
        }
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
        {
            testValue->result->data.boolean = f_greater_equal(&*testValue);
            debug("I_GREATER - string\n");
        }
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_greater_equal(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.boolean = f_equal(&*testValue);
            debug("I_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.boolean = f_equal(&*testValue);
            debug("I_EQUAL - double\n");
        }
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            testValue->result->data.boolean = f_greater_equal(&*testValue);
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_greater_equal(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;

    case I_NOT_EQUAL:
        if (testValue->op1->type == Type_INT)
        {
            testValue->result->data.boolean = f_not_equal(&*testValue);
            debug("I_NOT_EQUAL - integer\n");
        }
        else if (testValue->op1->type == Type_DOUBLE)
        {
            testValue->result->data.boolean = f_not_equal(&*testValue);
            debug("I_NOT_EQUAL - double\n");
        }
        else if ((testValue->op1->type == Type_STRING) && (testValue->op2->type == Type_STRING))
            testValue->result->data.boolean = f_not_equal(&*testValue);
        else if ((testValue->op1->type == Type_BOOL) && (testValue->op2->type == Type_BOOL))
            testValue->result->data.boolean = f_not_equal(&*testValue);
        else print_error(INCOMPATIBLE_TYPE);
        break;
    //works fine
    case I_LEN:
        if (testValue->op1->type == Type_STRING)
        {
            testValue->result->data.integer = length(&(testValue->op1->data.string));
            debug("I_LEN - string\n");
        }
        else
            return print_error(INCOMPATIBLE_TYPE);
        break;

    case I_COPY:
        if (testValue->op1->type == Type_STRING)
        {
            //error: incompatible types when assigning to type ‘cstring’ from type ‘struct cstring *’
            //testValue->result->data.string = copy(&(testValue->op1->data.string), testValue->start, testValue->count);
            debug("I_COPY - string\n");
        }
        else
            return print_error(INCOMPATIBLE_TYPE);
        break;

    //works fine
    case I_FIND:
        if (testValue->op1->type == Type_STRING)
        {
            testValue->result->data.integer = find(&(testValue->op1->data.string), &(testValue->op2->data.string));
            debug("I_FIND - string\n");
        }
        else
            return print_error(INCOMPATIBLE_TYPE);
        break;

    // FIX for mastermind Vojtisek
    case I_SORT:
        if (testValue->op1->type == Type_STRING)
        {
            // testValue->result->data.string = sort(&(testValue->op1->data.string));
            debug("I_FIND - string\n");
        }
        else
            return print_error(INCOMPATIBLE_TYPE);
        break;

    default:
        return print_error(RUNTIME_OTHER);
        break;
    }
    return 0;
}

// TESTING PURPOSES
/*
int main(void)
{
    T_ITEM item;
    item.instruction = I_ASSIGN; //instruction type

    //if you want to test INT
    item.op1 = malloc(sizeof(int));
    item.op2 = malloc(sizeof(int));
    item.result = malloc(sizeof(int));

    item.op1->type = Type_INT;
    item.op2->type = Type_INT;
    item.result->type = item.op1->type;
    item.op1->data.integer = 10;
    item.op2->data.integer = 4;

*/
/*if you want to test Double remove
item.op1 = malloc(sizeof(double));
item.op2 = malloc(sizeof(double));
item.result = malloc(sizeof(double));

item.op1->type = Type_DOUBLE;
item.op2->type = Type_DOUBLE;

item.op1->data.real = 2.00;
item.op2->data.real = 4.00;

item.result->type = item.op1->type;
item.result->data.integer = 0.0; */


/* item.op1 = malloc(sizeof(cstring));
 item.op2 = malloc(sizeof(cstring));
 item.result = malloc(sizeof(int));
 item.result->type = Type_INT;

 item.op1->type = Type_STRING;
 item.op2->type = Type_STRING;
 item.result->type = Type_INT;

 cstr_append_str(&(item.op1->data.string), "ahoj vojto ako sa mas");
 cstr_append_str(&(item.op2->data.string), "to");
 item.start = 1;
 item.count = 2;
*/

/*
    interpret(&item);

    printf("hodnota: %d", (item.result->data.integer));

    return 0;
}*/

