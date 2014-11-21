
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

// TESTING PURPOSES
// T for Type of variable
typedef enum {
    T_Undefined = 0,
    T_Integer,
    T_Double,
    T_String,
    T_Bool,
} ValueType;

typedef union {
    double d;
    int i;
} ValueData;

typedef struct {
    ValueData data;
    ValueType type;
} Value;

typedef enum {
    I_print = 1,
    I_read = 2,
    I_add,
    I_multiply,
    I_div,
} InstructionType;

int interpret(int instruction, Value *testValue)
{
    switch (instruction) {
    case 1:
        if (testValue->type == T_Integer)
            printf("%d\n", testValue->data.i);
        else if (testValue->type == T_Double)
            printf("%f\n", testValue->data.d);
        else print_error(SYNTAX_ERROR);
        break;
    case 2:
        if (testValue->type == T_Integer)
            scanf("%d", &testValue->data.i);
        else if (testValue->type == T_Double)
            scanf("%lf", &testValue->data.d);

        else print_error(SYNTAX_ERROR);
        break;
    default: break;
    }
    return 0;
}

// TESTING PURPOSES
/*
int main()
{
    Value testValue;
    testValue.type = T_Integer;
    testValue.data.i = 2;
    interpret(I_read, &testValue);
    interpret(I_print, &testValue);

    return 0;
}*/

