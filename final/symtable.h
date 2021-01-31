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

/**
 * Inicalizace tabulky
 *
 * @param Tab Ukazatel na strukturu
 */
void STableInit(tSymbolTablePtr *Tab);
void STableInitLocal(tSymbolTablePtrPromenna *Tab);
/**
 * Vyhledávání v tabluce a vrácení nalezené položky
 *
 * @param Tab Ukazatel na tabulku
 * @param Symbol Jméno proměnné/funkce
 * @return Nalezená položka, NULL pokud nenalezeno
 */
tSymbolTablePtr STableSearch(tSymbolTablePtr Tab, char *Symbol);
tSymbolTablePtrPromenna STableSearchLocal(tSymbolTablePtrPromenna Tab, char *Symbol);
/**
 * Vloženi položky do tabulky funkcí
 *
 * @param Tab    Ukazatel na tabulku
 * @param Symbol Jméno funkce
 * @param defined Informace že byla funkce definována
 */
void STableInsert(tSymbolTablePtr *Tab, char *Symbol,bool defined);
/**
 * Vložení položky do tabulky funkcí
 *
 * @param Tab Ukazatel na tabulku
 * @param Symbol Jméno proměnné
 * @param data Jaký datový typ proměnná má
 * @param hloubkazanoreni V jaké hloubce zanoření byla definována
 */
void STableInsertLocal(tSymbolTablePtrPromenna *Tab, char *Symbol,char data, unsigned int hloubkazanoreni);
/**
 * Pomocná funkce pro nahrazení nejpravějšího prvku
 *
 * @param PtrReplaced Pomocny prvek pro nahrazení
 * @param Tab Ukazatel na tabulku
 */
void ReplaceByRightmost(tSymbolTablePtr PtrReplaced, tSymbolTablePtr *Tab);
/**
 * Funkce pro mazání jednotlivých položek v tabulce
 *
 * @param Tab  Ukazatel na tabulku
 * @param Symbol Jméno proměnné/funkce
 */
void STableDelete(tSymbolTablePtr *Tab, char *Symbol);
/**
 * Funkce pro konečné smazání a uvolnění tabulky funkcí/proměnných
 *
 * @param Tab  Ukazatel na tabulku
 */
void STableDispose(tSymbolTablePtr Tab);
void STableDisposeLocal(tSymbolTablePtrPromenna Tab);
/**
 * Funkce pro částečné smazání tabulky proměnných na základě hloubky zanoření
 *
 * @param Tab   Ukazatel na tabulku
 * @param hloubkazanoreni Aktuální hloubka zanoření kterou budeme mazat
 */
void STableDisposeZanorene(tSymbolTablePtrPromenna *Tab, unsigned int hloubkazanoreni);
/**
 * Funkce která vrací datový typ proměnné z tabulky proměnných
 *
 * @param Tab   Ukazatel na tabulku
 * @param Symbol Jméno proměnnéé
 * @return Char o hodnotě 'i','s','f' nebo '_'
 */
char STableSearchLocalReturnType(tSymbolTablePtrPromenna Tab, char *Symbol);
