/**
 * @file    errors.h
 * @name    Chybove hlasky
 * @authors Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 * @brief   Error messages and error return codes for IFJ2014
 */

#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdio.h>

extern const char *err_table[];

enum msgs {
    SUCCESS,                /** Uspesny preklad programu */
    LEXICAL_ERROR,          /** Chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu). */
    SYNTAX_ERROR,           /** Chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury programu.). */
    UNDEFINED_IDENTIFIER,   /** Semanticka chyba v programu – nedefinovana funkce/promenna, pokus o redefinici funkce/promenne, atd. */
    INCOMPATIBLE_TYPE,      /** Semanticka chyba typove kompatibility v aritmetickych, retezovych a relacnich vyrazech. */
    SEMANTIC_ERROR,         /** Ostatni sematicke chyby */
    RUNTIME_NUMERIC_IN,     /** Behova chyba pri nacitani ciselne hodnoty ze vstupu */
    RUNTIME_UNINITIALIZED,  /** Behova chyba pri praci s neinicializovanou promennou */
    RUNTIME_DIV_BY_ZERO,    /** Behova chyba pri deleni nulou */
    RUNTIME_OTHER,          /** Ostatni behove chyby */
    INTERNAL_ERROR = 99,    /** Interni chyba interpretu */
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
unsigned int print_error(unsigned int ret_msg_id);

#endif //__ERRORS_H__
