//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně

Precedencni analyza zdola nahoru pro vyrazy
*/

#pragma once
#include "scanner.h"
#include "parser.h"
#include "codegen.h"



#define EXPR_STACK_ALLOC_SIZE 64

// typy neterminalu

typedef enum {
    X_INT,
    X_FLOAT,
    X_STRING,
    X_UNKNOWN
    //mozna pridat nil
} xNTermType;


// operátory z tabulky pro precedenční analýzu

typedef enum{
    X_MULDIV,	    // Násobení / dělení
    X_ADDSUB,	    // Sčítaní / odčítání
    X_LEQGEQ,		// Porovnání větší / menší / rovná se
    X_EQL,		    // Porovnání (ne)rovná se
    X_LDBR,		    // Levá (default) závorka
    X_RDBR,		    // Pravá (default) závorka
    X_N,		    // Proměnná
    X_$			    // Jiné
} xOperator;



// položky precedenční tabulky

typedef enum{
    X_OPEN,		// (<)
    X_CLOSE,	// (>)
    X_EQUAL,	// (=)
    X_EMPTY		// ( )
} xPriority;


// typ prvku na zasobniku

typedef enum{
    XT_OPEN,	// shift (<)
    XT_TERM,	// terminál
    XT_NONTERM,	// neterminál
} xItemType;

// unie dat prvku na zasobniku

typedef union{
    tToken token; // token / terminal
    xNTermType ntype; // typ neterminalu
} xItemData;


// prvek na zasobniku

typedef struct xItem{
    xItemType type;
    xItemData data;
} *txItem;


// zásobník pro precedenční analýzu

typedef struct xStack{
    int size;	// velikost zásobníku
    int top;	// index nejvyššího prvku
    txItem *xs;	// pole prvků
} *txStack;

static xNTermType exprTyp = X_UNKNOWN;

// funkce
tToken exprBUParse (tToken *token);
void exprBUStackInit (txStack *stack);
xPriority exprBUGetPriority (xOperator currOperator, xOperator nextOperator);
xOperator exprTokenTypeToOperator(tType tokenType);
void exprBUStackPush (txStack stack, txItem item);
txItem exprBUStackPop(txStack stack);
void exprBUStackOpen (txStack stack, int posun); // < shift
unsigned exprBUStackClose(txStack stack); // > parse
void exprBUStackDispose(txStack *stack);
void exprBUDivZeroCheck(char *zero); // kontrola deleni nulou, prebira typ 0 (float/int)







