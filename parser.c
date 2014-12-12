/**
 * @file    parser.c
 * @brief   Parser implementation
 * @author  Albert Uchytil (xuchyt03)
 ****************************************************************************/

#include <string.h>

#include "parser.h"
#include "parser_private.h"
#include "debug.h"
#include "instruction.h"
#include "gc.h"

#ifdef DEBUG
#include "ial.h"
#endif

/**
 * Value checking macro
 */
#define CHECK_VALUE(val, ret) if (((ret) = (val)) != SUCCESS) return (ret);
#define CATCH_VALUE(val, ret) if (((ret) = (val)) != SUCCESS && (ret) != RETURNING) return (ret)




static int nt_program(FILE *input, Tree *globals, Tree *functions,
                      Instruction **first_instr, Variables *vars);
static int nt_var_section(FILE *input, Tree *vars_tree, Tree *functions,
                          Var_record ***var_ar, unsigned *count, bool global, Variables *vars);
static int nt_var_list(FILE *input, Tree *vars_tree, Tree *functions, Var_record ***var_ar,
                       unsigned *count, bool global, Variables *vars);
static int nt_var_sublist(FILE *input, Tree *vars_tree, Tree *functions,
                          Var_record ***var_ar, unsigned *count, bool global, Variables *vars);
static int nt_var(FILE *input, Tree *vars, Tree *functions, bool eps,
                  Var_record **_var, int count);
static int t_symbol(FILE *input, enum token_symbol symbol);
static int t_symbol_catch(FILE *input, enum token_symbol symbol);
static int nt_type(FILE *input, Type *vars);
static int nt_func_section(FILE *input, Tree *globals, Tree *functions,
                           Variables *vars);
static int nt_func(FILE *input, Tree *globals, Tree *functions, Variables *vars);
static int t_id(FILE *input, cstring **_id);
static int nt_paramlist(FILE *input, Func_record *func, Tree *functions, unsigned *count);
static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                        Instruction **instr, unsigned *_var_count, Variables *vars,
                        Var_record ***var_ar);
static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **intr, Variables *vars);
static int t_keyword(FILE *input, enum token_keyword keyword);
static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars, bool empty);
static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                          Instruction **instr, Variables *vars, bool empty);
static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                  Instruction **instr, Variables *vars);
static int nt_main(FILE *input, Tree *globals, Tree *functions,
                   Instruction **first_instr, Variables *vars);
static int nt_arg_list_write(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                             Instruction **instr, Variables *vars);
static int nt_else(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                   Instruction **instr, Variables *vars);

static int search_trees(cstring *id, Tree *locals, Tree *globals, int *_ret, Type *_type);
static int insert_tree(Tree *insert, cstring *id, void *data, Tree *other);
static int update_tree(Tree *update, cstring *id, void *data, Tree *other);
static int check_functions_init(Tree *functions);
bool check_init(Tree_Node *node);
extern int file_end(FILE *input);

inline int file_end(FILE *input)
{
    int ret;
    Token token;
    CHECK_VALUE(get_token(&token, input), ret);
    if (token.type == TOKEN_EOF) {
        return SUCCESS;
    } else {
        return SYNTAX_ERROR;
    }
}

int parse(FILE *input, Instruction *first_instr, Variables *vars)
{
    int ret = SUCCESS;
    Tree globals;
    Tree functions;
    //Instruction *tmp = first_instr; TODO REMOVE

    tree_init(&globals);
    tree_init(&functions);
    ret = nt_program(input, &globals, &functions, &first_instr, vars);


#ifdef DEBUG
	debug("\n==============================PRINTING TREE globals================================\n");
	tree_print(&globals);
	fprintf(stderr, "\n===========================================================================\n\n");
	debug("\n==============================PRINTING TREE functions==============================\n");
	tree_print(&functions);
	fprintf(stderr, "\n===========================================================================\n\n");
#endif

    //*first_instr = *tmp; TODO REMOVE
    tree_free(&globals);
    tree_free(&functions);


    return ret;
}

static int nt_program(FILE *input, Tree *globals, Tree *functions,
                      Instruction **first_instr, Variables *vars)
{
    int ret;
    unsigned global_count;
    Var_record **var_ar = NULL;
    CATCH_VALUE(nt_var_section(input, globals, functions, &var_ar, &global_count,
                               true, vars), ret);
    CATCH_VALUE(nt_func_section(input, globals, functions, vars), ret);
    CHECK_VALUE(nt_main(input, globals, functions, first_instr, vars), ret);
    CHECK_VALUE(file_end(input), ret);
    CHECK_VALUE(check_functions_init(functions), ret);

    return SUCCESS;
}

static int nt_var_section(FILE *input, Tree *vars_tree, Tree *functions,
                          Var_record ***var_ar, unsigned *count, bool global,
                          Variables *vars)
{
    int ret;
    *count = 0;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_VAR) {
        CHECK_VALUE(nt_var_list(input, vars_tree, functions, var_ar, count,
                                global, vars), ret);
        return SUCCESS;
    }

    unget_token(&token);

    return RETURNING;
}

static int nt_var_list(FILE *input, Tree *vars_tree, Tree *functions, Var_record ***var_ar,
                       unsigned *count, bool global, Variables *vars)
{
    int ret;
    int id = *count;
    Var_record *var;
    CHECK_VALUE(nt_var(input, vars_tree, functions, false, &var, *count), ret);
    if (global && vars != NULL)
    {
        CHECK_VALUE(variables_add_empty(vars, var->type), ret);
    }
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    (*count)++;
    CHECK_VALUE(nt_var_sublist(input, vars_tree, functions, var_ar,
                               count, global, vars), ret);

    if ((ret = nt_var_sublist(input, vars_tree, functions, var_ar,
                              count, global, vars)) == RETURNING) {
       if ((*var_ar =
           (Var_record **)gc_realloc(__FILE__, *var_ar,
                                     ((*count) + 1) * sizeof(Var_record *)))
                   == NULL) {
           return INTERNAL_ERROR;
       }
       (*var_ar)[id] = var;
       return SUCCESS;
    } else if (ret == SUCCESS) {
        (*var_ar)[id] = var;
        return SUCCESS;
    } else {
        return ret;
    }
    return SUCCESS;
}

static int nt_var_sublist(FILE *input, Tree *vars_tree, Tree *functions,
                          Var_record ***var_ar, unsigned *count, bool global,
                          Variables *vars)
{
    int ret;
    int id = *count;
    Var_record *var;
    CHECK_VALUE(nt_var(input, vars_tree, functions, true, &var, *count), ret);
    if (global && vars != NULL)
        CHECK_VALUE(variables_add_empty(vars, var->type), ret);
    CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
    (*count)++;
    if ((ret = nt_var_sublist(input, vars_tree, functions, var_ar,
                              count, global, vars)) == RETURNING) {
        if ((*var_ar =
            (Var_record **)gc_realloc(__FILE__, *var_ar,
                                      ((*count) + 1) * sizeof(Var_record *)))
                    == NULL) {
            return INTERNAL_ERROR;
        }
        (*var_ar)[id] = var;
        return SUCCESS;
    } else if (ret == SUCCESS) {
        (*var_ar)[id] = var;
        return SUCCESS;
    } else {
        return ret;
    }
}

static int nt_var(FILE *input, Tree *vars, Tree *functions, bool eps,
                  Var_record **_var, int id)
{
    int ret;
    Token token;
    Var_record *var = NULL;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_ID) {
        MALLOC_VAR(var, __FILE__);

        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, &(var->type)), ret);
        CHECK_VALUE(insert_tree(vars, cstr_create_str(token.value.value_name),
                                var, functions), ret);
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
    Token token;
    Func_record *func = NULL;
    Instruction *instr = NULL;
    cstring *id;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FUNCTION) {
        MALLOC_FUNC(func, __FILE__);
        tree_create(&(func->locals));
        CHECK_VALUE(gen_instr(&(func->first_instr), I_NOP, 0, NULL), ret);
        instr = func->first_instr;

        CHECK_VALUE(t_id(input, &id), ret);
        CHECK_VALUE(tree_insert(func->locals, id, &(func->ret_value)), ret);
        func->ret_value.index = 0;
        func->local_count = 1;
        CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
        //TODO: Pavluv vymysl urezat
        CATCH_VALUE(nt_paramlist(input, func, functions, &func->local_count),
                    ret);
        if (func->params == NULL) {
            func->params = (Var_record **)gc_malloc(__FILE__, sizeof(Var_record *));
        }
        func->params[0] = &(func->ret_value);
        CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
        CHECK_VALUE(t_symbol(input, COLON), ret);
        CHECK_VALUE(nt_type(input, &(func->ret_value.type)), ret);
        func->ret_value.index = 0;
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);

        debug("function pre update: type: %d id: %s\n", func->params[0]->type, id->str);

        CHECK_VALUE(update_tree(functions, id, func, globals), ret);

        CHECK_VALUE(nt_func_body(input, func->locals, globals, functions,
                                 &instr, &(func->local_count), vars,
                                 &(func->params)), ret);
        if (!strcmp(id->str, BIF_COPY) || !strcmp(id->str, BIF_FIND) ||
            !strcmp(id->str, BIF_LENGTH) || !strcmp(id->str, BIF_SORT)) {
            return UNDEFINED_IDENTIFIER;
        }
        debug_print_instruction(func->first_instr);
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

static int nt_paramlist(FILE *input, Func_record *func, Tree *functions, unsigned *count)
{
    int ret;
    unsigned id = *count;
    Var_record *var = NULL;
    (*count)++;
    debug("*count = %d\n", *count);
    CHECK_VALUE(nt_var(input, func->locals, functions, true, &var, id), ret);
    if ((ret = t_symbol_catch(input, SEMICOLON)) == RETURNING) {
        debug("Allocating space for %u vars\n", *count);
        func->params = (Var_record **)gc_malloc(__FILE__, (*count) * sizeof(Var_record *));
        if (func->params == NULL) {
            return INTERNAL_ERROR;
        }
        func->params[id] = var;
        debug("Inserting var to index %u\n", id);
        func->param_count = (*count) - 1; /* Function identifier */
        return SUCCESS;
    } else if (ret == SUCCESS) {
        CHECK_VALUE(nt_paramlist(input, func, functions, count), ret);
        func->params[id] = var;
        debug("Inserting var to index %u\n", id);
        return SUCCESS;
    } else {
        return ret;
    }

    return SUCCESS;
}

static int nt_func_body(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                        Instruction **instr, unsigned *_var_count, Variables *vars,
                        Var_record ***var_ar)
{
    int ret;
    unsigned count = 0;
    Token token;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD &&
        token.value.value_keyword == KEYWORD_FORWARD) {
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret);
        return SUCCESS;
    } else {
        unget_token(&token);
        CATCH_VALUE(nt_var_section(input, locals, functions, var_ar,  &count,
                                   false, NULL), ret);
        if (_var_count != NULL)
            *_var_count += count;
        //TODO: consider removing I_NOP
        if (*instr == NULL) {
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret)
        }
        CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
        CHECK_VALUE(t_symbol(input, SEMICOLON), ret)
        CHECK_VALUE(gen_instr(instr, I_HALT, 0, NULL), ret);
        #ifdef DEBUG
	    debug("\n==============================PRINTING TREE LOCALS==================================\n");
	    tree_print(locals);
	    fprintf(stderr, "\n===========================================================================\n\n");
        #endif
        return SUCCESS;
    }
}

static int nt_comp_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars)
{
    int ret;
    CHECK_VALUE(t_keyword(input, KEYWORD_BEGIN), ret);
    CATCH_VALUE(nt_cmd_list(input, locals, globals, functions, instr, vars, true), ret);
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

static int nt_cmd_list(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                       Instruction **instr, Variables *vars, bool empty)
{
    int ret;
    CATCH_VALUE(nt_cmd_sublist(input, locals, globals, functions, instr, vars, empty), ret);

    return SUCCESS;
}

static int nt_cmd_sublist(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                          Instruction **instr, Variables *vars, bool empty)
{
    int ret;
    CATCH_VALUE(nt_cmd(input, locals, globals, functions, instr, vars), ret);
    if (ret == RETURNING) {
        return (empty) ? RETURNING : SYNTAX_ERROR;
    }
    CHECK_VALUE(t_symbol_catch(input, SEMICOLON), ret);
    CHECK_VALUE(nt_cmd_sublist(input, locals, globals, functions, instr, vars, false), ret);

    return SUCCESS;
}

static int nt_cmd(FILE *input, Tree *locals, Tree *globals, Tree *functions,
                  Instruction **instr, Variables *vars)
{
    int ret;
    int unique_id;
    Token token;
    Type type;
    Type type2;
    cstring *id;
    Instruction *tmp_instr = NULL;
    Instruction *tmp_instr2 = NULL;

    CHECK_VALUE(get_token(&token, input), ret);
    debug_token(&token);
    if (token.type == TOKEN_KEYWORD) {
        if (token.value.value_keyword == KEYWORD_READLN) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(t_id(input, &id), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
            CHECK_VALUE(search_trees(id, locals, globals, &unique_id, NULL), ret);
            debug("ret: %d\n", ret);
            CHECK_VALUE(gen_instr(instr, I_READLN, unique_id, NULL), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WRITE) {
            CHECK_VALUE(t_symbol(input, PARENTHESIS_L), ret);
            CHECK_VALUE(nt_arg_list_write(input, locals, globals, functions, instr, vars), ret);
            CHECK_VALUE(t_symbol(input, PARENTHESIS_R), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_IF) {
            CHECK_VALUE(parse_expr(input, locals, globals, functions,
                        vars, instr, &type, false), ret);
            if (type != TYPE_BOOL) {
                return INCOMPATIBLE_TYPE;
            }
            CHECK_VALUE(t_keyword(input, KEYWORD_THEN), ret);
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, NULL), ret);
            tmp_instr = *instr;
            tmp_instr2 = NULL;
            CHECK_VALUE(gen_instr(&tmp_instr2, I_NOP, 0, NULL),ret);
            tmp_instr->alt_instruction = tmp_instr2;
            //TODO: remove I_NOP
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            CATCH_VALUE(nt_else(input, locals, globals, functions,
                                &(tmp_instr2), vars), ret);
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret);
            tmp_instr2->next_instruction = *instr;
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_WHILE) {
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(parse_expr(input,locals, globals, functions, vars,
                                   instr, &type, false), ret);
            if (type != TYPE_BOOL) {
                return INCOMPATIBLE_TYPE;
            }
            CHECK_VALUE(t_keyword(input, KEYWORD_DO), ret);
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, NULL), ret);
            tmp_instr2 = *instr;
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            (*instr)->next_instruction = tmp_instr;
            tmp_instr = NULL;
            CHECK_VALUE(gen_instr(&tmp_instr, I_NOP, 0, NULL), ret);
            tmp_instr2->alt_instruction = tmp_instr;
            *instr = tmp_instr2->alt_instruction;
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_REPEAT) {
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(nt_cmd_list(input, locals, globals, functions, instr, vars, false), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_UNTIL), ret);
            CHECK_VALUE(parse_expr(input, locals, globals, functions, vars,
                                   instr, &type, false), ret);
            if (type != TYPE_BOOL) {
                return INCOMPATIBLE_TYPE;
            }
            CHECK_VALUE(gen_instr(instr, I_JMP, 0, NULL), ret);
            (*instr)->alt_instruction = tmp_instr;
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_BEGIN) {
            unget_token(&token);
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            return SUCCESS;
        } else if (token.value.value_keyword == KEYWORD_FOR) {
            CHECK_VALUE(get_token(&token, input), ret);
            if (token.type != TOKEN_ID) {
                return SYNTAX_ERROR;
            }
            CHECK_VALUE(t_symbol(input, ASSIGNMENT), ret);
            CHECK_VALUE(parse_expr(input, locals, globals, functions,
                                   vars, instr, &type, false), ret);
            CHECK_VALUE(search_trees(cstr_create_str(token.value.value_name),
                                     locals, globals, &unique_id, &type2), ret);
            if (type != type2) {
                return INCOMPATIBLE_TYPE;
            }
            CHECK_VALUE(gen_instr(instr, I_ASSIGN, unique_id, NULL), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_TO), ret);
            CHECK_VALUE(gen_instr(instr, I_NOP, 0, NULL), ret);
            tmp_instr = *instr;
            CHECK_VALUE(parse_expr(input, locals, globals,functions,
                                   vars, instr, &type, false), ret);
            CHECK_VALUE(t_keyword(input, KEYWORD_DO), ret);
            CHECK_VALUE(nt_comp_cmd(input, locals, globals, functions, instr, vars), ret);
            CHECK_VALUE(gen_instr(instr, I_INC, unique_id, NULL), ret);
            (*instr)->next_instruction = tmp_instr;
            CHECK_VALUE(gen_instr(&tmp_instr, I_NOP, 0, NULL), ret);
            (*instr)->alt_instruction = tmp_instr;
            *instr = (*instr)->alt_instruction;
            return SUCCESS;
        }
    } else if (token.type == TOKEN_ID) {
        //TODO: array
        CHECK_VALUE(t_symbol(input, ASSIGNMENT), ret);
        CHECK_VALUE(parse_expr(input, locals, globals, functions,
                               vars, instr, &type, false), ret);
        CHECK_VALUE(search_trees(cstr_create_str(token.value.value_name),
                                locals, globals, &unique_id, &type2), ret);
        
        if (type != type2) {
            return INCOMPATIBLE_TYPE;
        }
        CHECK_VALUE(gen_instr(instr, I_ASSIGN, unique_id, NULL), ret);
        return SUCCESS;
    }
    unget_token(&token);
    return RETURNING;
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

    CHECK_VALUE(parse_expr(input, locals, globals, functions, vars, instr, NULL, true), ret);
    CHECK_VALUE(gen_instr(instr, I_WRITE, 1337, NULL), ret);
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
        return SUCCESS;
    }
    unget_token(&token);
    return RETURNING;
}

static int search_trees(cstring *id, Tree *locals, Tree *globals, int *_ret, Type *_type)
{
    Tree_Node *node = NULL;

    if (locals != NULL && (node = tree_find_key(locals, id)) != NULL) {
        *_ret = -((int)((Var_record *)(node->data))->index + 1);
    } else if (globals == NULL) {
        return INTERNAL_ERROR;
    } else if ((node = tree_find_key(globals, id)) != NULL) {
        *_ret = (int)((Var_record *)(node->data))->index;
    }

    if (node != NULL) {
        if (_type != NULL)
            *_type = ((Var_record *)(node->data))->type;
        return SUCCESS;
    } else {
        return UNDEFINED_IDENTIFIER;
    }

    return (node != NULL) ? SUCCESS : UNDEFINED_IDENTIFIER;
}

static int insert_tree(Tree *insert, cstring *id, void *data, Tree *other)
{
    int ret;
    Tree_Node *node;
    if ((node = tree_find_key(other, id)) != NULL) {
        return UNDEFINED_IDENTIFIER;
    } else {
        CHECK_VALUE(tree_insert(insert, id, data), ret);
        return SUCCESS;
    }
}

static bool compare_var_record(Var_record *a, Var_record *b)
{
    if ((a == NULL && b != NULL) || (a != NULL && b == NULL))
        return false;
    return (a == NULL && b == NULL) ||
           (a->type == b->type && a->index == b->index);
}

static bool compare_var_records(Var_record **a, Var_record **b, int n)
{
    for (int i = 0; i < n; i++) {
        if (!compare_var_record(a[i], b[i]))
            return false;
    }
    return true;
}

static int compare_functions(Func_record *a, Func_record *b)
{
    if (a != NULL && b != NULL &&
        a->local_count == b->local_count &&
        compare_var_records(a->params, b->params, a->local_count)) {
        return SUCCESS;
    } else {
        return UNDEFINED_IDENTIFIER;
    }
}

static int update_tree(Tree *update, cstring *id, void *data, Tree *other)
{
    int ret;
    Tree_Node *node;
    if ((node = tree_find_key(other, id)) != NULL) {
        return UNDEFINED_IDENTIFIER;
    } else  if ((node = tree_find_key(update, id)) != NULL) {
        debug("%s\n", id->str);
        CHECK_VALUE(compare_functions((Func_record *)data, (Func_record *)(node->data)),
                    ret);
        node->data = data;
        return SUCCESS;
    } else {
        CHECK_VALUE(tree_insert(update, id, data), ret);
        return SUCCESS;
    }
}

static int check_functions_init(Tree *functions)
{
    if (functions != NULL &&
        tree_check_all(functions, &check_init)) {
        return SUCCESS;
    } else {
        return UNDEFINED_IDENTIFIER;
    }
}

bool check_init(Tree_Node *node) {
    Func_record *func = NULL;
    if (node != NULL && node->data != NULL) {
        func = (Func_record *)(node->data);
        return (func->first_instr != NULL &&
                func->first_instr->next_instruction != NULL);
    } else {
        return true;
    }
}
