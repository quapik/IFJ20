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
#include scanner.h
#include <string.h>
#include <stdio.h>

int StartParser(tToken *token);
int body(tToken *token);
int id_next(tToken *token);
int id_n(tToken *token);
int id_vlevo_vice(tToken *token);

#endif /* prarser.h */