//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Generování kódu, na souboru pracovali Šíma Vojtěch, Poposki Vasil
*/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h."
#include "symtable.h."

void CodeGenStart();
/**
 *
 * @param id Jméno proměnné kterou budeme definovat
 */
void CodeGenDefVar(char* id);
/**
 *
 * @param input_float Hodnota floatu ze scanneru která se převede na správný tvar pro výsledný kód
 * @return Výsledná podoba floatu
 */
char *gen_float(char *input_float);
/**
 *
 * @param input_string  Hodnota stringu ze scanneru která se převede na správný tvar pro výsledný kód
 */
void gen_string(char *input_string);
/**
 *
 * @param token Ukazatel na aktuální token se kterým se bude dále ve funkci pracovat
 */
void CodeGenPrint(tToken *token);
/**
 * Funkce pro výpis vestavěné funkce substr
 */
void gen_substr();
/**
 * Funkce pro výpis vestavěné funkce inputi, inputs a inputf
 *
 */
void gen_input();
/**
 * Funkce pro výpis vestavěné string funkce
 */
void gen_string_functions();
/**
 *
 * @param token Ukazatel na aktuální token se kterým se bude dále ve funkci pracovat
 * @param LocalTable Ukazatel na tabulku symbolů pro proměnné
 * @param IDCounter Počítadlo pro informaci kolik bylo proměnných na levé straně
 * @param UchovaniID Pomocná proměnná s informací o jménech proměnných
 * @return Ukazatel na poslední token se kterým se pracovalo
 */
tToken CodeGenInputi(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenInputf(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenInputs(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenSubstr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenOrd(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenChr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenLen(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);

