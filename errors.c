/*****************************************************
 * Chybove hlasky
 * Autori: Tomas Paulus (xpaulu01)
 *****************************************************/

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

const char *table[] =
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

    [SEMANTIC_ERROR] =           "Ostatni sematicke chyby.",
    [RUNTIME_NUMERIC_IN] =       "Behova chyba pri nacitani ciselne hodnoty ze vstupu.",
    [RUNTIME_UNINITIALIZED] =    "Behova chyba pri praci s neinicializovanou promennou.",
    [RUNTIME_DIV_BY_ZERO] =      "Behova chyba pri deleni nulou.",
    [RUNTIME_OTHER] =            "Ostatni behove chyby.",
    [INTERNAL_ERROR] =           "Interni chyba interpretu.",
};
