#include "parser.h"
#include "parser_private.h"

static int nt_program(FILE *input, Tree *globals, Tree *functions, 
                      Instruction **first_instr, Variables *vars);
static int nt_var_section(FILE *input, Tree *vars, int *count);
static int nt_var_list(FILE *input, Tree *vars, int *count);
static int nt_var_sublist(FILE *input, Tree *vars, int *count);
static int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var, int count);
static int t_symbol(FILE *input, enum token_symbol symbol);
static int t_symbol_catch(FILE *input, enum token_symbol symbol);
static int nt_type(FILE *input, Type *vars);
static int nt_func_section(FILE *input, Tree *globals, Tree *functions,
                           Variables *vars);
static int nt_func(FILE *input, Tree *globals, Tree *functions, Variables *vars);
static int t_id(FILE *input, cstring **_id);
static int nt_paramlist(FILE *input, Func_record *func, int *count);
static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                        Instruction *instr, unsigned *_var_count, Variables *vars);
static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **intr, Variables *vars);
static int t_keyword(FILE *input, enum token_keyword keyword);
//static int t_keyword_catch(FILE *input, enum token_keyword keyword);
static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars);
static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                          Instruction **instr, Variables *vars);
static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                  Instruction **instr, Variables *vars);
static int nt_main(FILE *input, Tree *globals, Tree *functions,
                   Instruction **first_instr, Variables *vars);
static int nt_arg_list_write(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                             Instruction **instr, Variables *vars);
static int nt_else(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                   Instruction **instr, Variables *vars);
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     unsigned locals_count, Instruction *alt_instr);
static int search_trees(cstring *id, Tree *locals, Tree *globals, int *_ret);

int parse(FILE *input, Instruction *first_instr, Variables *vars)
{
    int ret = SUCCESS;
    Tree globals;
    Tree functions;

    tree_init(&globals);
    tree_init(&functions);
    ret = nt_program(input, &globals, &functions, &first_instr, vars);

    tree_free(&globals);
    tree_free(&functions);

    return ret;
}

static int nt_program(FILE *input, Tree *globals, Tree *functions,
                      Instruction **first_instr, Variables *vars)
{
    int ret;
    int global_count;
    CATCH_VALUE(nt_var_section(input, globals, &global_count), ret);
    variables_occupy(vars, global_count);
    CATCH_VALUE(nt_func_section(input, globals, functions, vars), ret);
    CHECK_VALUE(nt_main(input, globals, functions, first_instr, vars), ret);

    return SUCCESS;
}

static int nt_var_section(FILE *input, Tree *vars, int *count)
{
    int ret;
    *count = 0;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars, count), ret);
        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

static int nt_var_list(FILE *input, Tree *vars, int *count)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, false, NULL, *count), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    (*count)++;
    CHECK_VALUE(nt_var_sublist(input, vars, count), ret);

    return SUCCESS;
}

static int nt_var_sublist(FILE *input, Tree *vars, int *count)
{
    int ret;
    CHECK_VALUE(nt_var(input, vars, true, NULL, *count), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    (*count)++;
    CHECK_VALUE(nt_var_sublist(input, vars, count), ret);

    return SUCCESS;
}

static int nt_var(FILE *input, Tree *vars, bool eps, Var_record **_var, int id)
{
    int ret;
    Token token;
    Var_record *var = NULL;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_ID) {
        MALLOC_VAR(var, __FILE__);

        CHECK_VALUE(tree_insert(vars, cstr_create_str(token.value.value_name), var), ret);
        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, &(var->type)), ret);
        var->index = id;
        if (_var != NULL)
            *_var = var;
        return SUCCESS;
    } else {
        if (eps) {
            unget_token(&token);
            return RETURNING;
        } else {
            return SYNTAX_ERROR;
        }
    }
}

static int t_symbol(FILE *input, enum token_symbol symbol)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == symbol) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

static int t_symbol_catch(FILE *input, enum token_symbol symbol)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == symbol) {
        return SUCCESS;
    }
    unget_token(&token);
    return RETURNING;
}

static int nt_type(FILE *input, Type *type)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    ret = (token.type == TOKEN_KEYWORD &&
          (token.value.value_keyword == KEYWORD_INTEGER ||
           token.value.value_keyword == KEYWORD_REAL ||
           token.value.value_keyword == KEYWORD_STRING ||
           token.value.value_keyword == KEYWORD_BOOLEAN));
    switch (token.value.value_keyword) {
    case KEYWORD_INTEGER:
        *type = TYPE_INT;
        break;
    case KEYWORD_REAL:
        *type = TYPE_REAL;
        break;
    case KEYWORD_STRING:
        *type = TYPE_STRING;
        break;
    case KEYWORD_BOOLEAN:
        *type = TYPE_BOOL;
        break;
    default:
        break;
    }
    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

static int nt_func_section(FILE *input, Tree *globals, Tree *functions,
                           Variables *vars)
{
    int ret;
    CHECK_VALUE(nt_func(input, globals, functions, vars), ret);
    CHECK_VALUE(nt_func_section(input, globals, functions, vars), ret);

    return SUCCESS;
}

static int nt_func(FILE *input, Tree *globals, Tree *functions, Variables *vars)
{
    int ret;
    int count = 0;
    Token token;
    Func_record *func = NULL;
    //Var_record *params = NULL;
    cstring *id;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FUNCTION) {
        MALLOC_FUNC(func, __FILE__);
        tree_create(&(func->locals));
        func->first_instr = NULL;

        CHECK_VALUE(t_id(input, &id), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
        CATCH_VALUE(nt_paramlist(input, func, &count), ret);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
        CHECK_VALUE(nt_type(input, &(func->ret_value.type)), ret);
        tree_insert(func->locals, id, &(func->ret_value));
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        CHECK_VALUE(nt_func_body(input, func->locals, globals, functions,
                                 func->first_instr, &(func->local_count), vars), ret);
        func->local_count += func->param_count;

        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

static int t_id(FILE *input, cstring **functions)
{
    bool ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if ((ret = (token.type == TOKEN_ID))) {
        *functions = cstr_create_str(token.value.value_name);
    }

    return (ret) ? SUCCESS : SYNTAX_ERROR;
}

static int nt_paramlist(FILE *input, Func_record *func, int *count)
{
    int ret;
    Var_record *var = NULL;
    CHECK_VALUE(nt_var(input, func->locals, true, &var, *count), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    (*count)++;
    if ((ret = nt_paramlist(input, func, count)) == RETURNING) {
        func->param_count = *count;
        func->params = (Var_record **)gc_malloc(__FILE__, (*count) * sizeof(Var_record *));
        if (func->params == NULL) {
            return INTERNAL_ERROR;
        }
        func->params[*count - 1] = var;
        return SUCCESS;
    } else if (ret == SUCCESS) {
        func->params[*count - 1] = var;
        return SUCCESS;
    } else {
        return ret;
    }

    return SUCCESS;
}

static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                        Instruction *instr, unsigned *_var_count, Variables *vars)
{
    int ret;
    int count = 0;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FORWARD) {
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        return SUCCESS;
    } else {
        unget_token(&token);
        CATCH_VALUE(nt_var_section(input, functions, &count), ret);
        if (_var_count != NULL)
            *_var_count = count;
        CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, &instr, vars), ret);
        CHECK_VALUE(gen_instr(&instr, I_HALT, 0, 0, NULL), ret);
        return SUCCESS;
    }
}

static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars)
{
    int ret;
    CHECK_VALUE(t_keyword(input, KEYWORD_BEGIN), ret);
    CATCH_VALUE(nt_cmd_list(input, locals, globals, functions, instr, vars), ret);
    CHECK_VALUE(t_keyword(input, KEYWORD_END), ret);

    return SUCCESS;
}

static int t_keyword(FILE *input, enum token_keyword keyword)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == keyword) {
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

//TODO:remove
/*
static int t_keyword_catch(FILE *input, enum token_keyword keyword)
{
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == keyword) {
        return SUCCESS;
    }
    unget_token(&token);
    return RETURNING;
}
*/

static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars)
{
    int ret;
    CATCH_VALUE(nt_cmd_sublist(input, locals, globals, functions, instr, vars), ret);

    return SUCCESS;
}

static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                          Instruction **instr, Variables *vars)
{
    int ret;
    CHECK_VALUE(nt_cmd(input, locals, globals, functions, instr, vars), ret);
    CHECK_VALUE(t_symbol_catch(input, SEMICOLON), ret);
    CHECK_VALUE(nt_cmd_sublist(input, locals, globals, functions, instr, vars), ret);

    return SUCCESS;
}

static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                  Instruction **instr, Variables *vars)
{
    int ret;
    int unique_id;
    Token token;
    cstring *id;
    Instruction *tmp_instr = NULL;
    Instruction *tmp_instr2 = NULL;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);

    if (token.type == TOKEN_KEYWORD) {
        if (token.value.value_keyword == KEYWORD_READLN) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(t_id(input, &id), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(search_trees(id, locals, globals, &unique_id), ret);
            CHECK_VALUE(gen_instr(instr, I_READLN, unique_id, 0, NULL), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WRITE) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(nt_arg_list_write(input, locals, globals, functions, instr, vars), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_IF) {
            CHECK_VALUE(parse_expr(input, locals, globals, functions, vars, instr, NULL),
                        ret);
            CHECK_VALUE(t_symbol(input, KEYWORD_THEN), ret);
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            CATCH_VALUE(nt_else(input, locals, globals, functions,
                                &(tmp_instr->alt_instruction), vars), ret);
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, 0, NULL), ret);
            if (tmp_instr->alt_instruction == NULL) {
                tmp_instr->alt_instruction = *instr;
            } else {
                tmp_instr->alt_instruction->next_instruction = *instr;
            }
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WHILE) {
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(parse_expr(input,locals, globals, functions, vars, instr, NULL),
                        ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_DO), ret);
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, 0, NULL), ret);
            tmp_instr2 = *instr;
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            (*instr)->next_instruction = tmp_instr;
            CHECK_VALUE(gen_instr(&(tmp_instr2->alt_instruction), I_NOP, 0, 0, NULL), ret);
            *instr = tmp_instr2->alt_instruction;
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_REPEAT) {
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(nt_cmd_list(input, locals, globals, functions, instr, vars), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_UNTIL), ret);
            CHECK_VALUE(parse_expr(input, locals, globals, functions, vars, instr, NULL),
                        ret);
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, 0, NULL), ret);
            (*instr)->alt_instruction = tmp_instr;
            return SUCCESS;
        }
    } else if (token.type == TOKEN_ID) {
        //TODO: array
        CHECK_VALUE(t_symbol(input, ASSIGNMENT), ret);
        CHECK_VALUE(parse_expr(input, locals, globals, functions, vars, instr, NULL), ret);
        CHECK_VALUE(search_trees(cstr_create_str(token.value.value_name),
                                locals, globals, &unique_id), ret);
        CHECK_VALUE(gen_instr(instr, I_ASSIGN, unique_id, 0, NULL), ret);
        return SUCCESS;
    }
    return SYNTAX_ERROR;
}

static int nt_main(FILE *input, Tree *globals, Tree *functions,
                   Instruction **first_instr, Variables *vars)
{
    int ret;
    CHECK_VALUE(nt_comp_cmd(input, NULL, globals, functions, first_instr, vars), ret);
    CHECK_VALUE(t_symbol(input, DOT), ret);

    return SUCCESS;
}

static int nt_arg_list_write(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                             Instruction **instr, Variables *vars) {
    int ret;
    Token token;
    CHECK_VALUE(parse_expr(input, locals, globals, functions, vars, instr, NULL), ret);
    CHECK_VALUE(gen_instr(instr, I_WRITE, 1337, 0, NULL), ret);
    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_SYMBOL &&
        token.value.value_symbol == COMMA) {
        CHECK_VALUE(nt_arg_list_write(input, locals, globals, functions, instr, vars), ret);
    } else {
        unget_token(&token);
    }
    return SUCCESS;
}

static int nt_else(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                   Instruction **instr, Variables *vars) {
    int ret;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_ELSE) {
        CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
    }
    unget_token(&token);
    return RETURNING;
}

/* DEFINITION OF MISCELLANEOUS FUNCTIONS */
static int gen_instr(Instruction **instr_ptr, Instruction_type type, int index,
                     unsigned locals_count, Instruction *alt_instr)
{
    Instruction *new_ptr = NULL;
    if ((new_ptr = gc_malloc(GC_INSTR, sizeof(Instruction))) == NULL) {
        return INTERNAL_ERROR;
    }
    *new_ptr = (Instruction) {
        .instruction = type,
        .index = index,
        .locals_count = locals_count,
        .next_instruction = NULL,
        .alt_instruction = alt_instr
    };

    if (*instr_ptr == NULL) {
        *instr_ptr = new_ptr;
    } else {
        (*instr_ptr)->next_instruction = new_ptr;
    }

    return SUCCESS;
}

static int search_trees(cstring *id, Tree *locals, Tree *globals, int *_ret)
{
    Tree_Node *node = NULL;

    if (locals != NULL && (node = tree_find_key(locals, id)) != NULL) {
        *_ret = (int)((Var_record *)(node->data))->index;
    } else if (globals == NULL) {
        return INTERNAL_ERROR;
    } else if ((node = tree_find_key(globals, id)) != NULL) {
        *_ret = -((int)((Var_record *)(node->data))->index + 1);
    }

    return (node != NULL) ? SUCCESS : UNDEFINED_IDENTIFIER;
}
