//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Parser, na souboru pracoval Šíma Vojtěch
*/

#ifndef _PARSER_H
#define _PARSER_H
#include "scanner.h"
#include "error.h"
#include "codegen.h"
#include "exprBottomUp.h"
#include "symtable.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
bool Porovnavani;

/**
 * Startovací funkce celého parseru - volaná z funkce main
 * 
 * @param token Ukazatel na aktuální token se kterým se bude dále ve funkci pracovat
 * @return Vrácenta hodnota typu int která informuje, zda je vše vpořádku (0) nebo chyba
 */
int StartParser(tToken *token);
/**
 * Všechny funkce si berou a vrací token, pomocí rekurzivního sestupu se jeho hodnota mění
 * 
 * @param Ukazatel na aktuální token se kterým se bude dále ve funkci pracovat
 * @return Ukazatel na poslední token se kterým se pracovalo
 */
tToken body(tToken *token);
tToken id_next(tToken *token);
tToken  id_n(tToken *token);
tToken  vice_id_vlevo(tToken *token);
tToken  paramscall(tToken *token);
tToken  paramscall_n(tToken *token);
tToken if_rule(tToken *token);
tToken func_rule(tToken *token);
tToken for_rule(tToken *token);
tToken params(tToken *token);
tToken params_n(tToken *token);
tToken narvrattype_n(tToken *token);
tToken vyraz_n(tToken *token);
tToken print_rule(tToken *token);


#endif /* parser.h */
