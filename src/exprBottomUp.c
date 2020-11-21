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

   int ret = -1;
   unsigned tmpRet;

    // Algoritmus precedencni analyzy
    while (ret > 0) {

        xOperator stackOp, newOp;

        //posun na spravnou polozku zasobniku
        int posun = stack->top;


        while(true) { // hledani na zasobniku
            if (stack->xs[posun]->type == XT_TERM) break;
        }
        if (posun < 0) stackOp = X_$;
        else stackOp = exprTokenTypeToOperator(stack->xs[posun]->data.token->type);  //konverze do precedencich prvku
        newOp = exprTokenTypeToOperator((*token)->type);

        // inicializace nove polozky
        txItem item;

        switch (exprBUGetPriority(stackOp, newOp)) {
            case X_OPEN: // shift <
                exprBUStackOpen(stack, posun + 1);
                item = malloc(sizeof(struct xItem));
                item->type = XT_TERM;
                item->data.token = (*token);
                exprBUStackPush(stack, item);
                (*token) = (*token)->nextToken;
                break;
            case X_CLOSE:
                tmpRet = exprBUStackClose(stack);
        }






        //gettingTokens = false;
    }

    return *token; //T_UNKNOWN prazdny kdyz error, data ERR_SEM_KOMP
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
    (*stack)->size = EXPR_STACK_ALLOC_SIZE;
    (*stack)->top = -1;
    (*stack)->xs = malloc(sizeof(struct xItem) * EXPR_STACK_ALLOC_SIZE);
    if ((*stack)->xs == NULL) {
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
        exit(99);
    }
}

void exprBUStackOpen (txStack stack, int posun)
{
    exprBUStackPush(stack, NULL);

    txItem item = malloc(sizeof(struct xItem));
    if (item == NULL)
    {
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
        exit(99);
    }
    //vzdy pridame < pred vlozenim noveho terminalu
    item->type = XT_OPEN;
    stack->xs[posun] = item;
}

void exprBUStackPush (txStack stack, txItem item)
{
    stack->top++;

    // kontrola preteceni
    if (stack->top >= stack->size)
    {
        stack->size += EXPR_STACK_ALLOC_SIZE;
        stack->xs = realloc(stack->xs, sizeof(txItem) * stack->size);
        if(stack->xs == NULL){
            fprintf(stderr, "[INTERNAL] Fatal error - nelze realokovat pamet pro stack\n");
            exit(99);
        }
    }
    // pushnutí
    stack->xs[stack->top] = item;
}

unsigned exprBUStackClose(txStack stack)
{
    txItem item = exprBUStackPop(stack);
    if (item == NULL)
    {
        return 99;
    }


    if (item->type == XT_TERM)
    {
        // resime pravidla, jednu stranu
        // E -> ( E )
        if (item->data.token->type == T_RDBR)
        {
            free(item);
            item = exprBUStackPop(stack);
            txItem ldbr = exprBUStackPop(stack);
            free(ldbr);
        }
        else
        {
            // E -> n
            xNTermType ntype = X_UNKNOWN;

            switch (item->data.token->type) {
                case T_INT:
                    //TODO generovani kodu
                    ntype = X_INT;
                    break;
                case T_EXP:
                    //TODO generovani kodu
                    ntype = X_FLOAT;
                    break;
                case T_STRING:
                    //TODO generovani kodu
                    ntype == X_STRING;
                    break;
                case T_ID:
                    //TODO sémantika
                    //TODO generovani kodu
                    break;
                default:
                    fprintf(stderr, "[SYNTAX ERROR] chyba ve vyrazu, ocekavan operand\n");
                    free(item);
                    return 2;
            }
            item->type = XT_NONTERM;
            item->data.ntype = ntype;

        }
        // vlozen redukovany neterminal na vrchol zasobniku
        free(exprBUStackPop(stack));
        exprBUStackPush(stack, item);
        return 0;
    }

    txItem rItem = item;
    item = exprBUStackPop(stack);
    txItem lItem = exprBUStackPop(stack);

    xNTermType lType = lItem->data.ntype;
    xNTermType rType = rItem->data.ntype;
    xNTermType type = rType;
    bool isSingle = (lItem->type == X_OPEN);
    bool unknownType = (rType == X_UNKNOWN);
    bool isSame = false; // stejny typ

    if(!(isSingle))
    {
        // identikujeme vyraz
        if (lType == X_UNKNOWN || rType == X_UNKNOWN)
        {
            unknownType = true;
        }

        if (lType == rType)
        {
            isSame = true;
        }
        else if (rType == X_UNKNOWN)
        {
            type = lType;
            isSame = true;
        }
        else if (lType == X_UNKNOWN)
        {
            isSame = true;
        }
        else if (lType == X_INT && rType == X_FLOAT)
        {
            //TODO generovani kodu (zadani str. 10)
            isSame = true;
        }
        else if (rType == X_INT && lType == X_FLOAT)
        {
            //TODO generovani kodu (zadani str. 10)
            type = X_FLOAT;
            isSame = true;
        }
    }

    switch (item->data.token->type) {
        //reseni operaci TODO
        case T_ADD:
        case T_SUB:
        case T_MUL:
        case T_DIV:
        case T_LESS:
        case T_GREAT:
        case T_LEQ:
        case T_GREQ:
        case T_EOL:
        case T_NEQ:
        default:
            break;


    }

    return 0;

}


txItem exprBUStackPop(txStack stack)
{
    if (stack->top < 0) return NULL;
    stack->top--;
    return stack->xs[stack->top + 1];
}