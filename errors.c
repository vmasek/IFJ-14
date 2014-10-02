/*
 * Nazev:   errors.c
 * Popis:   Chybove hlasky
 * Autori:  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *****************************************************/

#include "errors.h"

const char *err_table[] =
{
    /** Stavove hlasky */
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

/**Vypise chybovu/stavovu hlasku z err_table[] na stderr.
 * Ako stavove kody pouzivat tie z enum msgs.
 * Vrati navratovy kod prisluchajuci danej hlaske/kodu.
 * 
 * @param   ret_msg_id  - ID nav. kodu podla ktoreho vypise hlasku
 * @return ret_msg_id   - samotny navratovy kod
 */
inline unsigned int print_error(unsigned int ret_msg_id) 
{
    fprintf(stderr, "%s", err_table[ret_msg_id]);
    return ret_msg_id;
}
