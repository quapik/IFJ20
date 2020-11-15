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



/* uzel stromu */
typedef struct tBSTNode {
	char Key;			                                                      /* klíč */
	int BSTNodeCont;                                            /* užitečný obsah uzlu */
	struct tBSTNode * LPtr;                                    /* levý podstrom */
	struct tBSTNode * RPtr;                                   /* pravý podstrom */
} *tBSTNodePtr;

/* prototypy funkcí BVS*/

void BSTInit   (tBSTNodePtr *);
int BSTSearch  (tBSTNodePtr, char, int *); //TODO nebude to int, nechcem vracet nic
void BSTInsert (tBSTNodePtr *, char, int);
void BSTDelete (tBSTNodePtr *, char);
void BSTDispose(tBSTNodePtr *);

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
};

void SymtableInit(tSymtable*);
void SymtableSearch(tSymtable, string); //podle jmena promenne/fce hledani v tabulce
void SymtableInsert(tSymtable*, string); //vlozeni polozky do symtable, promenna a fce zvlast?
void SymtableDelete(tSymtable*, string);
void SyntableDispose(tSymtable*);
