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

    //CodeGenDefVar("dfsdf");

    int ret = -1;
    unsigned tmpRet;
    //bool first = true;


    // Algoritmus precedencni analyzy
    while (ret < 0) {

        //debug
        //printf("%s\n", (*token)->data);
        //if ((*token)->type == T_RDBR)
        //{
        //    printf(")\n");
        //}
        xOperator stackOp, newOp;

        //posun na spravnou polozku zasobniku
        int posun = stack->top;


        while(posun >= 0) { // hledani na zasobniku, preskocime neterminal
            if (stack->xs[posun]->type == XT_TERM)
            {
                break;
            }
            posun--;
        }
        if (posun < 0) stackOp = X_$;
        else stackOp = exprTokenTypeToOperator(stack->xs[posun]->data.token->type);  //konverze do precedencich prvku
        newOp = exprTokenTypeToOperator((*token)->type);



        // inicializace nove polozky
        txItem item;

        switch (exprBUGetPriority(stackOp, newOp)) {
            case X_OPEN: // shift <
                exprBUStackOpen(stack, posun + 1);
                item = malloc(sizeof(struct xItem)); //TODO CHYBA ALOKACE
                item->type = XT_TERM;
                item->data.token = *token;
                exprBUStackPush(stack, item);
                (*token) = (*token)->nextToken;
                break;
            case X_CLOSE: // >
                tmpRet = exprBUStackClose(stack);
                if (tmpRet > 0) {
                    ret = (int)tmpRet;
                }
                break;
            case X_EQUAL: // =
                item = malloc(sizeof(struct xItem));
                if (item == NULL)
                {
                    fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
                    exit(99);
                }
                item->type = XT_TERM; // na vrcholu nyni token - terminal
                item->data.token = (*token);
                exprBUStackPush(stack, item);
                (*token) = (*token)->nextToken;
                break;
            case X_EMPTY: // prazdne pole tabulky
                if (stackOp == newOp && newOp == X_$) //posledni stav prec. analyzy
                {
                    if (stack->top < 0)
                    {
                        fprintf(stderr, "[SYNTAX ERROR] chyba ve vyrazu, neuplny vyraz\n");
                        ret = 2;
                    }
                    else
                    {
                        //TODO generovani kodu
                        ret = 0;
                    }
                }
                else
                {
                    fprintf(stderr, "[SYNTAX ERROR] chyba ve vyrazu\n");
                    ret = 2;
                }

        }



        //gettingTokens = false;
    }

    exprBUStackDispose(&stack);

    //error handling
    if (ret != 0)
    {
        (*token)->type = T_UNKNOWN;
        if (ret == 2)
        {
            (*token)->data = "ERR_SYNTAX";
        }
        else if (ret == 3)
        {
            (*token)->data = "ERR_SEM_PROG";
        }
        else if (ret == 4)
        {
            (*token)->data = "ERR_SEM_TYPE";
        }
        else if (ret == 5)
        {
            (*token)->data = "ERR_SEM_KOMP";
        }
    }
    //debug
    switch (exprTyp) {
        case X_STRING:
            printf("vraceny typ = string\n");
            break;
        case X_FLOAT:
            printf("vraceny typ = float\n");
            break;
        case X_INT:
            printf("vraceny typ = int\n");
            break;
        case X_UNKNOWN:
            printf("vraceny typ = unknown (id)\n");
            break;
        default:
            break;
    }
    //debug
//    if (Porovnavani == true)
//    {
//        printf("Porovnáváme\n");
//    }
//    else
//    {
//        printf("Precedencni: (debug) Vse v poradku.\n");
//    }
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
        case X_EQL:
            if (nextOperator == X_RDBR || nextOperator == X_$)
                return X_CLOSE;
            else if (nextOperator == X_EQL) return X_EMPTY;
            return X_OPEN;
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

    //pro prohození prvků
    for (int i = stack->top - 1; i>= posun; i--)
    {
        stack->xs[i + 1] = stack->xs[i];
    }

    txItem item = malloc(sizeof(struct xItem));
    if (item == NULL)
    {
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro stack\n");
        exit(99);
    }
    //vzdy pridame na vrchol nebo pred neterminal <
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
        exit(99);
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
                    printf("PUSHS int@%s\n", item->data.token->data);
                    ntype = X_INT;
                    //exprTyp = 'i';
                    break;
                case T_EXP:
                    printf("PUSHS float@%s\n", item->data.token->data);
                    ntype = X_FLOAT;
                    //exprTyp = 'i';
                    break;
                case T_STRING:
                    //debug
                    //printf("%s\n", item->data.token->data);
                    printf("PUSHS ");

                    gen_string(item->data.token->data);
                    //debug
                    //printf("mame string\n");
                    ntype = X_STRING;
                    //exprTyp = 's';
                    break;
                case T_ID:
                    //TODO sémantika
                    printf("PUSHS LF@%s\n", item->data.token->data);
                    //exprTyp = 'u';
                    break;
                default:
                    fprintf(stderr, "[SYNTAX ERROR] chyba ve vyrazu, ocekavan operand\n");
                    free(item);
                    return 2;
            }
            item->type = XT_NONTERM;
            item->data.ntype = ntype;
            exprTyp = ntype;

        }
        // vlozen redukovany neterminal na vrchol zasobniku
        free(exprBUStackPop(stack));
        exprBUStackPush(stack, item);
        return 0;
    }


    //debug
    //printf("Navrat z nedokoncene casti \n");
    //return 0;

    txItem rItem = item;
    item = exprBUStackPop(stack);
    txItem lItem = exprBUStackPop(stack);


    xNTermType lType = lItem->data.ntype;

    xNTermType rType = rItem->data.ntype;
    xNTermType type = rType;
    bool isSingle = (lItem->type == XT_OPEN);
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
        else if (rType == X_UNKNOWN) //nejsou stejne typy a vpravo je prom
        {
            type = lType;
            isSame = true;

            //pro parser
            switch (lType) {
                case X_UNKNOWN:
                    exprTyp = 'u';
                    break;
                case X_FLOAT:
                    exprTyp = 'f';
                    break;
                case X_INT:
                    exprTyp = 'i';
                    break;
                case X_STRING:
                    exprTyp = 's';
                    break;
                default:
                    break;
            }
        }
        else if (lType == X_UNKNOWN) //nejsou stejne typy a vlevo je prom
        {
            isSame = true;
        }
        // Tolerance int a float u sebe
//        else if (lType == X_INT && rType == X_FLOAT)
//        {
//            //TODO generovani kodu (zadani str. 10)
//            isSame = true;
//        }
//        else if (rType == X_INT && lType == X_FLOAT)
//        {
//            //TODO generovani kodu (zadani str. 10)
//            type = X_FLOAT;
//            isSame = true;
//        }
    }


    exprTyp = type;





    switch (item->data.token->type) {
        //reseni operaci TODO
        case T_ADD:
            Porovnavani = false;
            if (isSingle)
            {
                fprintf(stderr, "Error: unarni operator nepodporovan\n");
                free(item);
                free(rItem);
                return 2;
            }
            if (unknownType) printf("todo\n");/*TODO generovani kodu */
            if (!(isSingle) && !(isSame))
            {
                fprintf(stderr, "Error: chybna kombinace datovych typu ve vyrazu\n");
                free(item);
                free((rItem));
                free(lItem);
                return 5;
            }
            if (!unknownType)
            {
                if (type == X_STRING)
                {
                    //TODO Generovani kodu
                }
                else
                {
                    //TODO Generovani kodu
                }
            }
            break;

        case T_SUB:
            Porovnavani = false;
            if (isSingle)
            {
                fprintf(stderr, "Error: unarni operator nepodporovan\n");
                free(item);
                free(rItem);
                return 2;
            }
            if (unknownType) printf("todo\n")/*TODO generovani kodu*/;
            if ((!(isSingle) && !(isSame)) || (type == X_STRING))
            {
                fprintf(stderr, "Error: chybna kombinace datovych typu ve vyrazu\n");
                free(item);
                free(rItem);
                free(lItem);
                return 5;
            }
            //TODO Generovani kodu
            break;
        case T_MUL:
            Porovnavani = false;
            if (unknownType) printf("todo\n")/* TODO Generovani kodu */;
            if (!(isSame) || (type == X_STRING))
            {
                free(item);
                free(rItem);
                free(lItem);
                if (!isSingle)
                {
                    fprintf(stderr, "Error: chybna kombinace datovych typu ve vyrazu\n");
                    return 5;
                } else
                {
                    fprintf(stderr, "Error: chyba ve vyrazu\n");
                    return 2;
                }
            }
            //TODO Generovani kodu
            break;
        case T_DIV:
            Porovnavani = false;
            if (unknownType) printf("todo\n")/* TODO Generovani kodu */;
            if (!(isSame) || (type == X_STRING))
            {
                fprintf(stderr, "Error: chybna kombinace datovych typu ve vyrazu\n");
                free(item);
                free(rItem);
                free(lItem);
                if (!isSingle)
                {
                    fprintf(stderr, "Error: chybna kombinace datovych typu ve vyrazu\n");
                    return 5;
                } else
                {
                    fprintf(stderr, "Error: chyba ve výrazu\n");
                    return 2;
                }
            }
            //TODO Generovani kodu (DELENI NULOU)
            if (unknownType) printf("todo\n")/*TODO Generovani kodu*/;
            else if (type == X_FLOAT)
            {
                //TODO Generovani kodu
            }
            else if (type == X_INT)
            {
                //TODO Generovani kodu
            }
            else
            {
                free(item);
                free(rItem);
                free(lItem);
                return 5;
            }
            break;
        case T_LESS:
        case T_GREAT:
        case T_LEQ:
        case T_GREQ:
            Porovnavani = true;
            if (isSingle)
            {
                //TODO generovani kodu
            }
            if (unknownType) printf("todo\n")/*TODO generovani kodu*/;
            if ((!(isSingle) && !(isSame)) || (type == X_STRING))
            {
                fprintf(stderr, "Error: ve vyrazu: porovnavame operandy rozdilnych typu\n");
                free(item);
                free(rItem);
                free(lItem);
                return isSingle ? 2 : 5;
            }
            break;
        case T_EQL:
        case T_NEQ:
            Porovnavani = true;
                if (isSingle)
                {
                    fprintf(stderr, "Error: chyba ve vyrazu\n");
                    free(item);
                    free(rItem);
                    free(lItem);
                    return 2;
                }
                else if (unknownType)
                {
                    //TODO Generovani kodu
                }
                else if (!(isSame))
                {
                    //TODO Generovani kodu
                }
                break;

        default:
            fprintf(stderr, "[UNKNOWN ERROR] chyba ve vyrazu, neocekavany operand\n");
            free(item);
            free(rItem);
            free(lItem);
            return 99;
    }

    //reseni kodu

    switch (item->data.token->type) {
        case T_ADD:

            if (type == X_STRING)
            {
                printf("#---KONKATENACE---\nCREATEFRAME\nDEFVAR $tmp1\nDEFVAR $tmp2\nDEFVAR $tmpString\n");
                printf("POPS TF@$tmp2\nPOPS TF@$tmp1\nCONCAT TF@$tmpString TF@$tmp1 TF@$tmp2\nPUSHS TF@$tmpString\n");
                printf("#---KONEC KONKATENACE---\n");
            }
            printf("ADDS\n");

            break;
        case T_SUB:
            printf("SUBS\n");
            break;
        case T_MUL:
            printf("MULS\n");
            break;
        case T_DIV:
        printf("#---DELENI---\n");
            if (!unknownType)
            {
                if (type == X_INT)
                {
                    exprBUDivZeroCheck("int");
                    printf("IDIVS\n");
                }
                else if (type == X_FLOAT)
                {
                    exprBUDivZeroCheck("float");
                    printf("DIVS\n");
                }
            }
            printf("#---KONEC DELENI---\n");
            break;
        case T_GREAT:
            printf("GTS\n");
            break;
        case T_LESS:
            printf("LTS\n");
            break;
        case T_LEQ:
            printf("GTS\n");
            printf("NOTS\n");
            break;
        case T_GREQ:
            printf("LTS\n");
            printf("NOTS\n");
            break;
        case T_EQL:
            if (isSame) printf("EQS\n");
            break;
        case T_NEQ:
            if (isSame) printf("EQS\n");
            printf("NOTS\n");
            break;
        default:
            free(item);
            free(rItem);
            free(lItem);
            return 99;
    }
    free(rItem);
    free(lItem);
    item->type = XT_NONTERM;
    item->data.ntype = type;
    if (!(isSingle))
    {
        free(exprBUStackPop(stack));
    }
    exprBUStackPush(stack, item);
    return 0;
}


txItem exprBUStackPop(txStack stack)
{
    if (stack->top < 0) return NULL;
    stack->top--;
    return stack->xs[stack->top + 1];
}

void exprBUStackDispose(txStack *stack) {
    if (stack == NULL || (*stack) == NULL)
    {
        return;
    }
    int del = (*stack)->top;
    while (del >= 0)
    {
        free((*stack)->xs[del]);
        del--;
    }
    free((*stack)->xs);
    free((*stack));
    (*stack) = NULL;
}
void exprBUDivZeroCheck(char *zero)
{
    printf("JUMP $expression\n");

    //error print zatim tady:
    printf("LABEL $divZeroError\n\tDEFVAR TF@$errorPrint\n\tMOVE TF@$errorPrint str@error:\\032pokus\\032o\\032deleni\\032nulou\n");
    printf("\tDPRINT TF@$errorPrint\n");
    printf("\tEXIT int@9\n");

    printf("LABEL $expression\n");


    if (zero == NULL)
    {
        return;
    }
    if (strcmp(zero, "int") == 0)
    {
        printf("DEFVAR TF@$RNum\nPOPS TF@$RNum\nJUMPIFEQ $divZeroError TF@$RNum int@0\n");
    }
    else if (strcmp(zero, "float") == 0)
    {
        printf("DEFVAR TF@$RNum\nPOPS TF@$RNum\nJUMPIFEQ $divZeroError TF@$RNum float@0x0p+0\n");
    }
    printf("PUSHS TF@$RNum\n");
}