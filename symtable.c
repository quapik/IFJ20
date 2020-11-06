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

/* prototypy funkcí */

void BSTInit   (tBSTNodePtr *);
int BSTSearch  (tBSTNodePtr, char, int *);
void BSTInsert (tBSTNodePtr *, char, int);
void BSTDelete (tBSTNodePtr *, char);
void BSTDispose(tBSTNodePtr *);
