/*
 * Nazev:   common.c
 * Popis:   Funkce vyuzitelne napric moduly
 * Autori:  Albert Uchytil (xuchyt03)
 ************************************************************/

/**
 * @brief Prevede znak na odpovidajici cislo
 * @param c znak, ktery se bude konvertovat
 * @returns ciselnou hodnotu odpovidajici znaku
 * 
 * Funkce provede konverzi znaku na cislo, ktere zadanemu znaku odpovida.
 * V pripade, ze nejde o cislici je navracena 0.
 */
inline int char_to_int(char c)
{
    return ('0' <= c && c <= '9') ? (int)(c - '0') : 0;
}
