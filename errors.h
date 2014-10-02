/*
 * Nazev:   errors.h
 * Popis:   Chybove hlasky
 * Autori:  Tomas Paulus (xpaulu01), Vojtech Mašek (xmasek15)
 *****************************************************/

#ifndef _ERRORS_H
#define _ERRORS_H

#include <stdio.h>

extern const char *err_table[];

enum msgs
{
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


/**Vypise chybovu/stavovu hlasku z err_table[] na stderr.
 * Vrati navratovy kod prisluchajuci danej hlaske/kodu.
 * 
 * @param ret_msg_id - ID navratoveho kodu podla ktoreho vypise hlasku
 * @return ret_msg_id - samotny navratovy kod
 */
extern unsigned int print_error(unsigned int ret_msg_id);

#endif
