

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

