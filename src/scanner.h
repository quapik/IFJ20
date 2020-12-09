//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně

Na souboru pracoval Radek Čábela
Lexikalni Analyzator
*/

#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>


/* Stavy FSM Lexikalniho analyzatoru
 * Nazvy odpovidaji schematu */

typedef enum{
    STATE_NULL,
    STATE_ERROR,
    STATE_START,
    STATE_EOF,
    STATE_EOL,
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
    STATE_DEFINE,
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
    STATE_EQL,
    STATE_EXC,
    STATE_NEQ,
} sState;

// Typy tokenu
typedef enum
{
    //KEYWORDS

    T_UNKNOWN,
    T_ELSE,
    T_KEYFLOAT64, //mozna problem
    T_FOR,
    T_FUNC,
    T_IF,
    T_KEYINT,
    T_PACKAGE,
    T_RETURN,
    T_KEYSTRING,

    //TERMINALY

    T_EOF, 		 	//Konec souboru
    T_EOL,			//Konec radku
    T_ID, 		 	//Identifiktor [obsahuje data]
    T_INT, 		 	//Cele cislo [obsahuje data]
    T_DOUBLE, 	 	//Desetine cislo [obsahuje data]
    T_EXP, 		 	//Float 64 (s exponentem) [obsahuje data]
    T_STRING, 	 	//Retezec " ... " [obsahuje data]
    T_LDBR, 	 	// Left default bracket ' ( '
    T_RDBR, 	 	// Right default bracket ' ) '
    T_ASSIGN,	 	// Prirazeni ' = '
    T_DEFINE,		// Definice :=
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
* obousmerny seznam */

typedef struct Token {

    tType type; 				// typ tokenu
    char *data;					// hodnota tokenu
    struct Token *prevToken;	// ukazatel na predchozi token
    struct Token *nextToken;	// ukazatel na dalsi token

} *tToken;

// funkce
/**
 * Vytvoří vázaný seznam tokenů ze standardního vstupu
 *
 * @param firstToken Ukazatel na první token deklarovaný ve funkci main
 * @return int Hodnota 0 při úspěchu, hodnota 1 při chybě
 */
int scannerLoadTokens(tToken *firstToken, FILE *file);
/**
 * Funkce deterministického konečného stavového automatu. Navržena podle schématu.
 * Dle posloupnosti znaků ze standardního vstupu se zjišťuje typ tokenu a lexikální správnost.
 * Pomocné proměnné dataString a ignoreChar slouží ke snadnějšímu naplnění datové složky tokenu v případech
 * ID nebo string.
 *
 * @param token Token odeslaný do funkce, očekává se, že bude určen typ a naplněn datovým obsahem
 * @return int Hodnota 0 při úspěchu, hodnota 1 při chybě
 */
int scannerDKA(tToken token, FILE *file);
/**
 * Naplní jeden token správnými ukazateli (ukazatel na další a předchozí token) a zavolá
 * scannerDKA pro získ informací o datech a typu
 *
 * @param newToken Ukazatel na nový token
 * @return int Hodnota 0 při úspěchu, hodnota 1 při chybě
 */
int scannerGetValidToken (tToken *newToken, FILE *file);
/**
 * Kontrola, zda je token typu ID klíčovým slovem. V případě, že
 * je klíčové slovo bude tokenu změnen na token s typem klíčového slova
 *
 * @param token Kontrolovaný token
 * @return tType Původní token v případě nenalezeného klíčového slova, token T_UNKNOWN, T_ELSE, T_KEYFLOAT64, T_FOR,
 * T_FUNC, T_IF, T_KEYINT, T_PACKAGE, T_RETURN, nebo T_KEYSTRING v případě nalezeného klíčového slova
 */
tType scannerKeywordCheck (tToken token);
/**
 * Uvolní z paměti seznam tokenů.
 *
 * @param firstToken Ukazatel na první token seznamu tokenů
 */
void scannerTokenListDealloc (tToken *firstToken);
/**
 * Slouží pouze k debugu, v odevzdané verzi nevyužitá.
 *
 * @param token
 * @param opt 1 -> chybovy vypis , opt = 0 -> vypis obsahu tokenu (pro syntax, pozdejsi chyby)
 */
void scannerPrintDebug (tToken token, unsigned opt);

