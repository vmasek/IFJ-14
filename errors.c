/**
 * @file    errors.c
 * @author  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 * @brief   Error messages and error return codes for IFJ2014
 */

#include "errors.h"

const char *err_table[] = {
    /** Error messages */
    [LEXICAL_ERROR] =
    "Chyba v programu v ramci lexikalni analyzy.\n"
    "Chybna struktura aktualniho lexemu.\n",
    [SYNTAX_ERROR] =
    "Chyba v programu v ramci syntakticke analyzy.\n"
    "Chybna syntaxe struktury programu.\n",
    [UNDEFINED_IDENTIFIER] =
    "Semanticka chyba v programu.\n"
    "Nedefinovana funkce nebo promenna,\n"
    "pokus o redefinici funkce nebo promenne.\n",
    [INCOMPATIBLE_TYPE] =
    "Semanticka chyba typove kompatibility\n"
    "v aritmetickych, retezovych nebo relacnich vyrazech.\n"
    "Spatny pocet ci typ parametru u volani funkce.\n",

    [SUCCESS] =                 "",
    [SEMANTIC_ERROR] =          "Ostatni sematicke chyby.\n",
    [RUNTIME_NUMERIC_IN] =      "Behova chyba pri nacitani ciselne hodnoty ze vstupu.\n",
    [RUNTIME_UNINITIALIZED] =   "Behova chyba pri praci s neinicializovanou promennou.\n",
    [RUNTIME_DIV_BY_ZERO] =     "Behova chyba pri deleni nulou.\n",
    [RUNTIME_OTHER] =           "Ostatni behove chyby.\n",
    [INTERNAL_ERROR] =          "Interni chyba interpretu.\n",
};

/**
 * @brief   Printing error messages and returning their error codes.
 *
 * @param   ret_msg_id  ID of returning code.
 * @return  ret_msg_id  Return code.
 *
 * Prints error/state message from err_table[] to stderr.
 * Use enum msgs codes for state codes.
 * Returning code for returning state.
 */
inline unsigned int print_error(unsigned int ret_msg_id)
{
    fprintf(stderr, "%s", err_table[ret_msg_id]);
    return ret_msg_id;
}
