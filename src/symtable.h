//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Symtable
*/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


typedef struct tSymbolTable{
    char *Symbol;							// nazev identifikatoru

    char *datastringparametry;
    char *datastringnavratovehodnoty;
    bool defined;   // data promenne/funkce predavane strukturou dat
    struct tSymbolTable *LPtr;				// prvek na globalni urovni s mensim klicem
    struct tSymbolTable *RPtr;				// prvek na globalni urovni s vetsim klicem
}*tSymbolTablePtr;

typedef struct tSymbolTablePromenna{
    char *Symbol;							// nazev identifikatoru
    char DatovyTyp;
    unsigned int HloubkaZanoreni;                            // data promenne/funkce predavane strukturou dat
    struct tSymbolTablePromenna *LPtr;				// prvek na globalni urovni s mensim klicem
    struct tSymbolTablePromenna *RPtr;				// prvek na globalni urovni s vetsim klicem
}*tSymbolTablePtrPromenna;


void STableInit(tSymbolTablePtr *Tab);
void STableInitLocal(tSymbolTablePtrPromenna *Tab);
tSymbolTablePtr STableSearch(tSymbolTablePtr Tab, char *Symbol);
tSymbolTablePtrPromenna STableSearchLocal(tSymbolTablePtrPromenna Tab, char *Symbol);
void STableInsert(tSymbolTablePtr *Tab, char *Symbol);
void STableInsertLocal(tSymbolTablePtrPromenna *Tab, char *Symbol,char data, unsigned int hloubkazanoreni);
void ReplaceByRightmost(tSymbolTablePtr PtrReplaced, tSymbolTablePtr *Tab);
void STableDelete(tSymbolTablePtr *Tab, char *Symbol);
void STableDispose(tSymbolTablePtr *Tab);
void STableDisposeLocal(tSymbolTablePtrPromenna *Tab);
void STableDisposeZanorene(tSymbolTablePtrPromenna *Tab, unsigned int hloubkazanoreni);
