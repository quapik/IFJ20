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
 	STATE_START,
 	STATE_EOF,
 	STATE_EOL,
 	STATE_SPACE,
 	STATE_ID,
 	STATE_INT,
 	STATE_INT0,
 	STATE_EXP0,
 	STATE_EXP1,
 	STATE_FLOAT64,
 	STATE_DBL0,
 	STATE_DOUBLE,
 	STATE_STR0,	
 	STATE_STR1, 
 	STATE_STRHEX, 
 	STATE_STRHEX2,
 	STATE_STRING,
 	STATE_CMNT0,
 	STATE_CMNT1,
 	STATE_CMNT2,
 	STATE_CMNT3,
 	STATE_LDBR, 
 	STATE_RDBR, 
 	STATE_COLON, 
 	STATE_ASSIGN,
 	STATE_LCBR,
 	STATE_RCBR,
 	STATE_COMMA,
 	STATE_SEMICOL,
 	STATE_ADD,
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
 } sState;

 // Typy tokenu
 typedef enum
 {
 	//KEYWORDS

 	T_UNKNOWN,
 	T_ELSE,
 	T_FLOAT64, //mozna problem
 	T_FOR,
 	T_FUNC,
 	T_IF,
 	T_INT,
 	T_PACKAGE,
 	T_RETURN,
 	T_STRING,

 	//TERMINALY

 	T_EOF, 		 	//Konec souboru
 	T_EOL,			//Konec radku
 	T_SPACE,		//Mezera, tabulator apod.
 	T_ID, 		 	//Identifiktor
 	//T_INT0, 	 	//0 useless asi
 	T_INT, 		 	//Cele cislo
 	T_DOUBLE, 	 	//Desetine cislo
 	T_EXP, 		 	//Float 64 (s exponentem)
 	T_STRING, 	 	//Retezec " ... "
 	T_LDBR, 	 	// Left default bracket ' ( '
 	T_RDBR, 	 	// Right default bracket ' ) '
 	T_ASSIGN,	 	// Prirazeni :=
 	T_LCBR, 	 	// Left curly bracket ' { '
 	T_RCBR, 	 	// Right curly bracket ' } '
 	T_COMMA, 	 	// Carka
 	T_SEMICOLON, 	// Strednik
 	T_ADD, 		 	// + Scitani nebo konkatenace retezce
 	T_SUB, 		 	// - Odcitani
 	T_MUL, 		 	// * Nasobeni
 	T_DIV, 		 	// / Deleni
 	T_LESS, 	 	// < Je mensi
 	T_LEQ, 		 	// <= Mensi nebo rovno
 	T_GREAT, 	 	// > Je vetsi
 	T_GREQ, 	 	// >= Vetsi nebo rovno
 	T_EQL, 	 	 	// == Je rovno
 	T_NEQ 		 	// != Neni rovno

 	//DODELAT NETERMINALY CASEM
 } tType;


 /* Struktura tokenu
 * obousmernu seznam */

 typedef struct Token {

 	tType type; 				// typ tokenu
 	char *data;					// hodnota tokenu
 	struct Token *prevToken;	// ukazatel na predchozi token
 	struct Token *nextToken;	// ukazatel na dalsi token

 } *tToken;

