/*****************************************************
 * Chybove hlasky
 * Autori: Tomas Paulus (xpaulu01)
 *****************************************************/

enum msgs
{
    SUCCESS,                /** uspesny preklad programu */
    LEXICAL_ERROR,          /** Chyba v programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu). */
    SYNTAX_ERROR,           /** Chyba v programu v ramci syntakticke analyzy (chybna syntaxe struktury programu.). */   
    UNDEFINED_IDENTIFIER,   /** semanticka chyba v programu – nedefinovana funkce/promenna, pokus o redefinici funkce/promenne, atd. */     
    INCOMPATIBLE_TYPE,      /** semanticka chyba typove kompatibility v aritmetickych, retezovych a relacnich vyrazech. */
    SEMANTIC_ERROR,         /** ostatni sematicke chyby */
    RUNTIME_NUMERIC_IN,     /** behova chyba pri nacitani ciselni hodnoty ze vstupu */
    RUNTIME_UNINITIALIZED,  /** behova chyba pri praci s neinicializovanou promennou */ 
    RUNTIME_DIV_BY_ZERO,    /** behova chyba pri deleni nulou */
    RUNTIME_OTHER,          /** ostatni behove chyby */
    INTERNAL_ERROR = 99,    /** interni chyba interpretu */
};

const char *table[]=
{
    /** Stavove hlasky */
    [LEXICAL_ERROR]=
    "\nChyba v programu v ramci lexikalni analyzy.\n"
    "Chybna struktura aktualniho lexemu.",
    [SYNTAX_ERROR]=
    "\nChyba v programu v ramci syntakticke analyzy.\n"
    "Chybna syntaxe struktury programu.",
    [UNDEFINED_IDENTIFIER]=
    "\nSemanticka chyba v programu.\n"
    "Nedefinovana funkce nebo promenna,"
    "pokus o redefinici funkce nebo promenne.",
    [INCOMPATIBLE_TYPE]=
    "\nSemanticka chyba typove kompatibility\n"
    "v aritmetickych, retezovych nebo relacnich vyrazech."
    "Spatny pocet ci typ parametru u volani funkce.",
    
    [SEMANTIC_ERROR]=           "\nOstatni sematicke chyby.",
    [RUNTIME_NUMERIC_IN]=       "\nBehova chyba pri nacitani ciselne hodnoty ze vstupu.",
    [RUNTIME_UNINITIALIZED]=    "\nBehova chyba pri praci s neinicializovanou promennou.",
    [RUNTIME_DIV_BY_ZERO]=      "\nBehova chyba pri deleni nulou.",
    [RUNTIME_OTHER]=            "\nOstatni behove chyby.",
    [INTERNAL_ERROR]=           "\nInterni chyba interpretu.",
};