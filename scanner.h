//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně

Lexikalni Analyzator
*/

#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>


/* Stavy FSM Lexikalniho analyzatoru
 * Nazvy odpovidaji grafu */

 typedef enum{
 	STATE_NULL,
 	STATE_ERROR,
 	STATE_START,
 	STATE_EOF,
 	STATE_ID,
 	STATE_INT,
 	STATE_EXP0,
 	STATE_EXP1,
 	STATE_FLOAT64,
 	STATE_DBL0,
 	STATE_DOUBLE,
 	STATE_STR0,	//ALT
 	STATE_STR1, //ALT
 	STATE_STRHEX, //ALT
 	STATE_STRING, //ALT
 	STATE_CMNT0,
 	STATE_CMNT1,
 	STATE_CMNT2,
 	STATE_CMNT3,
 	STATE_LDBR, //ALT
 	STATE_RDBR, //ALT
 	STATE_COLON, //ALT
 	STATE_ASSIGN,
 	STATE_LCBR,
 	STATE_RCBR,
 	STATE_COMMA,
 	STATE_SEMICOL, //ALT
 	STATE_ADD, //ALT
 	STATE_SUB,
 	STATE_MUL,
 	STATE_DIV,
 	STATE_LESS,
 	STATE_LEQ,
 	STATE_GREAT,
 	STATE_GREQ,
 	STATE_SEQ,
 	STATE_EQL,
 	STATE_EXC,
 	STATE_NEQ
 } lState;

