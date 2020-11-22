//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


/* uzel stromu */
typedef struct tBSTNode {
	char* Key;			                                         /* klíč --jmeno identifikatoru nebo fce (token->data)*/
	int BSTNodeCont;                                            /* užitečný obsah uzlu */
	int PromennaORFunkce;                                       /* 1=promenna fce */ //Todo predelat na vlasni dat typ?
	struct tBSTNode * LPtr;                                    /* levý podstrom */
	struct tBSTNode * RPtr;                                   /* pravý podstrom */
} *tBSTNodePtr;

typedef enum {
    TypUlozenychHodnotPromenna,     /* nodeDataTypeVariable */
    TypUlozenychHodnotFunkce    /* nodeDataTypeFunction */
} tTypUlozenychHodnot;

/* prototypy funkcí BVS*/

void BSTInit   (tBSTNodePtr *RootPtr);
tBSTNodePtr BSTSearch  (tBSTNodePtr RootPtr, char* K);
void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr);
void BSTInsert (tBSTNodePtr* RootPtr, char* K, tTypUlozenychHodnot PromennaOrFunkce);
void BSTDelete (tBSTNodePtr *RootPtr, char* K);
void BSTDispose (tBSTNodePtr *RootPtr);

typedef struct symtable {
    tBSTNodePtr RootPtr;
} tSymtable;

typedef struct funkce {
bool definovana;
int parametry_datovytyp[10]; //arr pro ulozeni typu parametru a navratovych hodnot int=1, flaot64=2, string=3
int navratovehodnoty_datovytyp[10];
}tFunkce;

typedef struct promenna{ //is it neccesery?
int promenna_Datovytyp; ///int=1, flaot64=2, string=3 
}tPromennaDatovytyp;

void SymtableInit(tSymtable*);
tBSTNodePtr SymtableSearch(tSymtable* Symtable, char* K); //podle jmena promenne/fce hledani v tabulce
void SymtableInsertGlobal(tSymtable* Symtable, char* K);//vlozeni polozky do symtable globalni (pro funkce)
void SymtableInsertLocal(tSymtable* Symtable, char* K); //vlozeni polozky do symtable globalni (pro promenne)
void SymtableDelete(tSymtable* Symtable,char* K);
void SyntableDispose(tSymtable* Symtable);
