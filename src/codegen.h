//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Generování kódu
*/
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "symtable.h"

void CodeGenStart();
void CodeGenDefVar(char* id);
char *gen_float(char *input_float);
void gen_string(char *input_string);
void CodeGenPrint(tToken *token);
void gen_substr();
void gen_input();
void gen_string_functions();
tToken CodeGenInputi(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenInputf(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenInputs(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenSubstr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenOrd(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenChr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);
tToken CodeGenLen(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]);

