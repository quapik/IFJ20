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

#include "exprBottomUp.h"


xOperator exprTokenTypeToOperator(tType tokenType) {
    switch(tokenType) {
        case T_MUL:
        case T_DIV:
            return X_MULDIV;
        case T_ADD:
        case T_SUB:
            return X_ADDSUB;
        case T_LESS:
        case T_GREAT:
        case T_LEQ:
        case T_GREQ:
            return X_LEQGEQ;
        case T_EQL:
        case T_NEQ:
            return X_EQL;
        case T_LDBR:
            return X_LDBR;
        case T_RDBR:
            return X_RDBR;
        case T_INT:
        case T_EXP:
        case T_STRING:
        case T_ID:
            return X_N;
        default:
            return X_$;
    }
}


tToken exprBUParse (tToken *token) {
    txStack stack;
    exprBUStackInit(&stack);

    bool gettingTokens = true;

    while (gettingTokens) {

        xOperator stackOp, newOp;

        //posun na spravnou polozku zasobniku
        int posun = stack->top;
        while(true) {
            if (stack->xs[posun]->type == XT_TERM) break;
        }
        if (posun < 0) stackOp = X_$;
        else stackOp = exprTokenTypeToOperator(stack->xs[posun]->xToken->type);  //konverze do precedencich prvku
        newOp = exprTokenTypeToOperator((*token)->type);

        txItem item;
        switch (exprBUGetPriority(stackOp, newOp)) {
            
        }






        gettingTokens = false;
    }

    return *token; //T_UNKNOWN prazdny kdyz error
}

xPriority exprBUGetPriority (xOperator currOperator, xOperator nextOperator) {
    switch (currOperator) {
        case X_$:
            if (nextOperator == X_$ || nextOperator == X_RDBR)
            {
                return X_EMPTY;
            }
            else return X_OPEN;
        case X_MULDIV:
            if (nextOperator == X_LDBR || nextOperator == X_N)
            {
                return X_OPEN;
            }
            else return X_CLOSE;
        case X_ADDSUB:
            if (nextOperator == X_LDBR || nextOperator == X_N || nextOperator == X_MULDIV)
            {
                return X_OPEN;
            }
            else return X_CLOSE;
        case X_LEQGEQ:
            if (nextOperator == X_LEQGEQ)
            {
                return X_EMPTY;
            }
            else if (nextOperator == X_$ || nextOperator == X_RDBR)
            {
                return X_CLOSE;
            }
            else return X_OPEN;
        case X_LDBR:
            if (nextOperator == X_$)
            {
                return X_EMPTY;
            }
            else if (nextOperator == X_RDBR)
            {
                return X_EQUAL;
            }
            else return X_OPEN;
        case X_RDBR:
        case X_N:
            if (nextOperator == X_LDBR || nextOperator == X_N)
            {
                return X_EMPTY;
            }
            else return X_CLOSE;
        default:
            break;
    }
    return X_EMPTY;
}

void exprBUStackInit (txStack *stack) {
    if (stack == NULL) return;
    *stack = malloc(sizeof(struct xStack));
    if (stack == NULL) {
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
        exit(99);
    }
    (*stack)->size = EXPR_STACK_ALOC_SIZE;
    (*stack)->top = -1;
    (*stack)->xs = malloc(sizeof(struct xItem) * EXPR_STACK_ALOC_SIZE);
    if ((*stack)->xs == NULL) {
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
        exit(99);
    }
}


