//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Lexikalni Analyzator
*/

#include "scanner.h"

int scannerLoadTokens(tToken *firstToken)
{
    // pocitame s platnym souborem

    tToken prevToken = NULL;
    int dkaError;
    //cyklus, ktery vytvori linked seznam
    while (prevToken == NULL || prevToken->type != T_EOF) {

        tToken newToken;


        // filling token:
        dkaError = scannerGetValidToken(&newToken);

        if (prevToken == NULL)
            // Prvni token
        {
            *firstToken = newToken;
        }
        else
            // Napojeni minuleho tokenu na soucasny
        {
            prevToken->nextToken = newToken;
        }

        // Napojeni souasneho tokenu na predchozi
        newToken->prevToken = prevToken;

        // Nove vytvoreny je predchozi pro dalsi cyklus
        prevToken = newToken;

        //osetreni erroru
        if(dkaError)
        {
            scannerTokenListDealloc(&prevToken);
            *firstToken = NULL;
            return dkaError;
        }
    }

    return 0;

}

int scannerGetValidToken (tToken *newToken)
{
    //pocitame s platnym souborem
    if (newToken == NULL) return 99;

    *newToken = malloc(sizeof(struct Token));
    if(newToken == NULL){
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro token\n");
        exit(99);
    }
    (*newToken)->data = NULL;
    (*newToken)->prevToken = NULL;
    (*newToken)->nextToken = NULL;

    bool dkaError = false;
    //bool debugPrint = true;

    unsigned int suspiciousCounter = 0;
    // cyklus naplneni jednoho tokenu
    do {
        free((*newToken)->data);
        (*newToken)->data = NULL;
        if (scannerDKA(*newToken))
        {
            suspiciousCounter++;
            dkaError = true;
        }
        if (suspiciousCounter==20)
        {
            fprintf(stderr, "Error: Prilis mnoho chyb, konec lexikalni analyzy a programu.\n");
            return 1;
        }

        //debug
        //printf("dkaError je %d\n", dkaError);

        //vypis prvni chyby
//        if (dkaError == true && debugPrint == true)
//        {
//            scannerPrintDebug((*newToken), 1);
//            debugPrint = false;
//        }




    } while ((dkaError && (*newToken)->type != T_EOF) ||
             (*newToken)->type == T_UNKNOWN);

    //keyword check
    (*newToken)->type = scannerKeywordCheck((*newToken));

    return dkaError ? 1 : 0;
}

int scannerDKA(tToken token)
{

    // pocitame s platnym souborem

    sState state = STATE_START;
    sState nextState;

    int currChar = 0;
    //char *tokenData;


    bool ignoreChar = false; //indikace nepridani znaku do obsahu tokenu
    bool gettingLex = true;

    token->type = T_UNKNOWN;

    //dynamicky string
    unsigned int c = 0; //data string counter
    unsigned int dsMultiplier = 16; // data string multiplier - k alokaci a realokaci //TODO fix to var
    char *dataString = malloc(sizeof(char)*dsMultiplier);
    if(dataString == NULL){
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro token\n");
        exit(99);
    }

    while(gettingLex) {

        //doplneni do pole charů
        if (currChar != 0) //prvni pruchod (pokud currchar neni nulovy znak)
        {
            //znak se nevejde do alokovaneho pole
            if (c >= dsMultiplier)
            {
                dsMultiplier += 16; //TODO fix to var
                dataString = realloc(dataString, sizeof(char)*dsMultiplier);
                if(dataString == NULL){
                    fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro token\n");
                    exit(99);
                }

            }
            if (!ignoreChar)
//            if (currChar != ' ' &&
//                nextState != STATE_START
//                && nextState != STATE_DIV
//                && nextState != STATE_CMNT3) //osetreni komentaru
            {
                dataString[c] = currChar;
                c++;
            }
        }
        ignoreChar = false;

        //sken dalsiho znaku
        if (currChar != EOF)
        {
            currChar = getc(stdin);
        }

        //debug
        //printf("scanning... %c\n", currChar);

        nextState = STATE_NULL;


        switch(state) {
            case STATE_START:
                if (currChar == '\n') nextState = STATE_EOL;
                else if (currChar == EOF) nextState = STATE_EOF;
                else if (currChar == '/')
                {
                    ignoreChar = true;
                    nextState = STATE_DIV;
                }
                else if (currChar == '0') nextState = STATE_INT0;
                else if (currChar == '"')
                {
                    ignoreChar = true;
                    nextState = STATE_STR0;
                }
                else if (currChar == '!') nextState = STATE_EXC;
                else if (currChar == '=') nextState = STATE_ASSIGN;
                else if (currChar == '>') nextState = STATE_GREAT;
                else if (currChar == '<') nextState = STATE_LESS;
                else if (currChar == '*') nextState = STATE_MUL;
                else if (currChar == '-') nextState = STATE_SUB;
                else if (currChar == '+') nextState = STATE_ADD;
                else if (currChar == ';') nextState = STATE_SEMICOL;
                else if (currChar == ',') nextState = STATE_COMMA;
                else if (currChar == '{') nextState = STATE_LCBR;
                else if (currChar == '}') nextState = STATE_RCBR;
                else if (currChar == ':') nextState = STATE_COLON;
                else if (currChar == '(') nextState = STATE_LDBR;
                else if (currChar == ')') nextState = STATE_RDBR;
                else if (isdigit(currChar)) nextState = STATE_INT;
                else if (isalpha(currChar) || currChar == '_') nextState = STATE_ID;
                else if (isspace(currChar))
                {
                    nextState = STATE_START;
                    ignoreChar = true;
                }
                else nextState = STATE_ERROR;
                break;

                //ID
                //case STATE_KEYWORD:
            case STATE_ID:
                if (isalpha(currChar) || isdigit(currChar) || currChar == '_') nextState = STATE_ID;
                else token->type = T_ID;
                break;

                //NUMBER
            case STATE_INT0:
                if (currChar == '.') nextState = STATE_DBL0;
                else if (currChar == 'E' || currChar == 'e') nextState = STATE_EXP0;
                else token->type = T_INT;
                break;
            case STATE_INT:
                if (isdigit(currChar)) nextState = STATE_INT;
                else if (currChar == '.') nextState = STATE_DBL0;
                else if (currChar == 'E' || currChar == 'e') nextState = STATE_EXP0;
                else token->type = T_INT;
                break;
            case STATE_EXP0:
                if (currChar == '+' || currChar == '-') nextState = STATE_EXP1;
                else if (isdigit(currChar)) nextState = STATE_FLOAT64;
                else nextState = STATE_ERROR;
                break;
            case STATE_DBL0:
                if (isdigit(currChar)) nextState = STATE_DOUBLE;
                else nextState = STATE_ERROR;
                break;
            case STATE_EXP1:
                if (isdigit(currChar)) nextState = STATE_FLOAT64;
                else nextState = STATE_ERROR;
                break;
            case STATE_DOUBLE:
                if (isdigit(currChar)) nextState = STATE_DOUBLE;
                else if (currChar == 'E' || currChar == 'e') nextState = STATE_EXP0;
                else token->type = T_DOUBLE;
                break;
            case STATE_FLOAT64:
                if (isdigit(currChar)) nextState = STATE_FLOAT64;
                else token->type = T_EXP;
                break;

                //STRING
            case STATE_STR0:
                if (currChar == '\\') nextState = STATE_STR1;
                else if (currChar == '"')
                {
                    ignoreChar = true;
                    nextState = STATE_STRING;
                }
                else if (currChar >= 32) nextState = STATE_STR0;
                else nextState = STATE_ERROR;
                break;
            case STATE_STR1:
                if (currChar == 'n' ||
                    currChar == 't' ||
                    currChar == '\\' ||
                    currChar == '"') nextState = STATE_STR0;
                else if (currChar == 'x') nextState = STATE_STRHEX;
                else nextState = STATE_ERROR;
                break;
            case STATE_STRHEX:
                if ((currChar >= 65 && currChar <= 70) ||
                    (currChar >= 97 && currChar <= 102) ||
                    (isdigit(currChar))) nextState = STATE_STRHEX2;
                else nextState = STATE_ERROR;
                break;
            case STATE_STRHEX2:
                if ((currChar >= 65 && currChar <= 70) ||
                    (currChar >= 97 && currChar <= 102) ||
                    (isdigit(currChar))) nextState = STATE_STR0;
                else nextState = STATE_ERROR;
                break;
            case STATE_STRING:
                token->type = T_STRING;
                break;

                //COMMENTS
            case STATE_CMNT0:
                if (currChar == '*') nextState = STATE_CMNT1;
                else if (currChar == '/')
                {
                    ignoreChar = true;
                    nextState = STATE_CMNT3;
                }
                else nextState = STATE_ERROR;
                break;
            case STATE_CMNT1:
                if (currChar == '*') nextState = STATE_CMNT2;
                else if (currChar != EOF) nextState = STATE_CMNT1;
                else nextState = STATE_ERROR;
                break;
            case STATE_CMNT2:
                if (currChar == '/')
                {
                    ignoreChar = true;
                    nextState = STATE_START;
                }
                else nextState = STATE_ERROR;
                break;
            case STATE_CMNT3:
                if (currChar != '\n' && currChar != EOF)
                {
                    ignoreChar = true;
                    nextState = STATE_CMNT3;
                }
                else if (currChar == '\n')
                {
                    ignoreChar = true;
                    nextState = STATE_START;
                }
                else nextState = STATE_ERROR;
                break;

                //MISC
                //case STATE_SPACE:
                //		printf("Ve STATE_SPACE currchar k porovnani je: %c\n", currChar);
                //        if (isspace(currChar) && currChar != '\n') nextState = STATE_SPACE;
                //        nextState = STATE_START;
                //		break;
            case STATE_EOL:
                token->type = T_EOL;
                break;
            case STATE_EOF:
                token->type = T_EOF;
                break;
            case STATE_LDBR:
                token->type = T_LDBR;
                break;
            case STATE_RDBR:
                token->type = T_RDBR;
                break;
            case STATE_SEMICOL:
                token->type = T_SEMICOLON;
                break;
            case STATE_LCBR:
                token->type = T_LCBR;
                break;
            case STATE_RCBR:
                token->type = T_RCBR;
                break;
            case STATE_COLON:
                if (currChar == '=') nextState = STATE_DEFINE;
                else nextState = STATE_ERROR;
                break;
            case STATE_DEFINE:
                token->type = T_DEFINE;
                break;
            case STATE_COMMA:
                token->type = T_COMMA;
                break;

                //EXPRESSIONS
            case STATE_ADD:
                token->type = T_ADD;
                break;
            case STATE_SUB:
                token->type = T_SUB;
                break;
            case STATE_MUL:
                token->type = T_MUL;
                break;
            case STATE_DIV:
                if (currChar == '*') nextState = STATE_CMNT1;
                else if (currChar == '/')
                {
                    ignoreChar = true;
                    nextState = STATE_CMNT3;
                }
                else token->type = T_DIV;
                break;
            case STATE_LESS:
                if (currChar == '=') nextState = STATE_LEQ;
                else token->type = T_LESS;
                break;
            case STATE_LEQ:
                token->type = T_LEQ;
                break;
            case STATE_GREAT:
                if (currChar == '=') nextState = STATE_GREQ;
                else token->type = T_GREAT;
                break;
            case STATE_GREQ:
                token->type = T_GREQ;
                break;
            case STATE_ASSIGN:
                if (currChar == '=') nextState = STATE_EQL;
                else token->type = T_ASSIGN;
                break;
            case STATE_EQL:
                token->type = T_EQL;
                break;
            case STATE_EXC:
                if (currChar == '=') nextState = STATE_NEQ;
                else (nextState = STATE_ERROR);
                break;
            case STATE_NEQ:
                token->type = T_NEQ;
                break;
            default:
                break;
        }
        // ignorujeme znaky:
        //if (nextState == STATE_START) {
        //    printf("freed string: %s\n", dataString);
        //    dataString[0] = '\0';
        //    free(dataString);
        //}

        //konec souboru:
        if (token->type == EOF)
        {
            break;
        }
//		if (nextState == STATE_START)
//        {
//		    //debug
//		    //printf("mazu");
//		    if (strlen(dataString) != 0)
//            {
//                free(dataString);
//            }
//        }

        // priprava obsahu tokenu:
        if (token->type != T_UNKNOWN) {
            ungetc(currChar, stdin);
            dataString[c] = '\0';
            //debug
            //printf("ungeted \n");
            break;
        }
        else {
            //debug
            //printf("mame unknown tokens \n");
            if (nextState == STATE_ERROR) {
                //printf("error \n");


                fprintf(stderr, "Error: Neocekavany znak ");
                if (currChar == '\n') fprintf(stderr, "EOL\n");
                else if (currChar == EOF) fprintf(stderr, "EOF\n");
                else fprintf(stderr, "%c \n", currChar);
                if (state != STATE_START) {
                    break;
                }
                gettingLex = false;
            }
            //break;
        }

        state = nextState;
    }

    //debug
    //printf("obsah tokenu: %s\n", dataString);

    //zaplneni datoveho obsahu tokenu
    switch (token->type)
    {
        case T_ID:
        case T_INT:
        case T_DOUBLE:
        case T_EXP:
        case T_STRING:
            token->data = dataString;
            break;
        default:
            free(dataString);
            break;
    }



    return nextState == STATE_ERROR ? 1 : 0;
}

tType scannerKeywordCheck (tToken token)
{
    if (token->type != T_ID) return token->type;
    char *dataString;
    dataString = token->data;

    //debug
    //printf("kontroluji keywords... \n");

    if(strcmp(dataString, "else") == 0) return T_ELSE;
    else if (strcmp(dataString, "float64") == 0) return T_KEYFLOAT64;
    else if (strcmp(dataString, "for") == 0) return T_FOR;
    else if (strcmp(dataString, "func") == 0) return T_FUNC;
    else if (strcmp(dataString, "if") == 0) return T_IF;
    else if (strcmp(dataString, "int") == 0) return T_KEYINT;
    else if (strcmp(dataString, "package") == 0) return T_PACKAGE;
    else if (strcmp(dataString, "return") == 0) return T_RETURN;
    else if (strcmp(dataString, "string") == 0) return T_KEYSTRING;
    else
    {
        //debug
        //printf("nenalezen keyword.\n");
        return T_ID;
    }
}

void scannerTokenListDealloc (tToken *firstToken)
{
    if(firstToken == NULL || *firstToken == NULL) return;

    while((*firstToken)->nextToken != NULL){
        *firstToken = (*firstToken)->nextToken;
    }

    while(*firstToken != NULL){
        tToken prevToken = (*firstToken)->prevToken;

        if ((*firstToken)->data != NULL) {
            free((*firstToken)->data);
        }
        free(*firstToken);
        *firstToken = NULL;

        *firstToken = prevToken;
    }
}

void scannerPrintDebug (tToken token, unsigned opt) {
    // pokud opt = 1, vypiseme error, pokud opt = 0, vypiseme obsah tokenu
    char *dataString;
    dataString = token->data;

    if (opt == 1)
    {
        printf("Debug: Chyba tokenu\n");
    }

    if (token == NULL)
    {
        printf("Debug: Token je NULL\n");
        return;
    }
    if (dataString == NULL)
    {
        printf("Debug: Zadna data v tokenu\n");
        return;
    }
    else
    {
        if (opt == 1)
        {
            printf("Debug: Vyskytla se chyba pobliz %s\n", dataString);
            return;
        }
        if (opt == 0)
        {
            printf("Debug: Obsah tokenu = %s\n", dataString);
            return;
        }
    }
}
