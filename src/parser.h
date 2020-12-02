//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/

#ifndef _PARSER_H
#define _PARSER_H
#include "scanner.h"
#include "error.h"
#include "exprBottomUp.c"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int StartParser(tToken *token);
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
bool Porovnavani=false;


#endif /* parser.h */
