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

	// next TODO keywords

int scannerLoadTokens(tToken *firstToken, FILE *file)
{
    // pocitame s platnym souborem

    tToken prevToken = NULL;

    //cyklus, ktery vytvori linked seznam
    while (prevToken == NULL || prevToken->type != T_EOF) {

        tToken newToken;

        // filling token:
        newToken = malloc(sizeof(struct Token));
        if(newToken == NULL){
            fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro token\n");
            exit(99);
        }
        newToken->data = NULL;
        newToken->prevToken = NULL;
        newToken->nextToken = NULL;

        bool dkaError = false;


        // cyklus naplneni jednoho tokenu
        do {
            free(newToken->data);
            newToken->data = NULL;
            dkaError = scannerDKA(newToken, file);

            //debug
            printf("dkaError je %d\n", dkaError);


        } while ((dkaError && newToken->type != T_EOF) ||
                    newToken->type == T_UNKNOWN);

        //TODO keyword check



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
            //TODO free list
            *firstToken = NULL;
            return dkaError;
        }
    }

    return 0;

}

int scannerDKA(tToken token, FILE *file)
{

	// pocitame s platnym souborem

	sState state = STATE_START;
	sState nextState;

	int currChar;

	bool gettingLex = true;

	token->type = T_UNKNOWN;

	//TODO Implementovat pamatovani stringu (pole nebo ungetc)

	while(gettingLex) {

        if (currChar != EOF) {
            currChar = getc(file);
        }

        printf("scanning... %c\n", currChar);


        switch(state) {
			case STATE_START:
					if (currChar == '\n') nextState = STATE_EOL;
					else if (currChar == EOF) nextState = STATE_EOF;
					else if (currChar == '/') nextState = STATE_CMNT0;
					else if (currChar == '0') nextState = STATE_INT0;
					else if (currChar == '"') nextState = STATE_STR0;
					else if (currChar == '!') nextState = STATE_EXC;
					else if (currChar == '=') nextState = STATE_ASSIGN;
					else if (currChar == '>') nextState = STATE_GREAT;
					else if (currChar == '<') nextState = STATE_LESS;
					else if (currChar == '/') nextState = STATE_DIV;
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
                    else if (isspace(currChar)) nextState = STATE_SPACE;
					else nextState = STATE_ERROR;
					break;
			
			//ID
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
					else token->type = T_DOUBLE;
					break;
			case STATE_FLOAT64:
					if (isdigit(currChar)) nextState = STATE_FLOAT64;
					else token->type = T_EXP;
					break;
			
			//STRING
			case STATE_STR0:
					if (currChar == '\\') nextState = STATE_STR1;
					else if (currChar == '"') nextState = STATE_STRING;
					else if (currChar > 32) nextState = STATE_STR0;
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
					else if (currChar == '/') nextState = STATE_CMNT3;
					else nextState = STATE_ERROR;
					break;
			case STATE_CMNT1:
					if (currChar == '*') nextState = STATE_CMNT2;
					else if (currChar != EOF) nextState = STATE_CMNT1;
					else nextState = STATE_ERROR;
					break;
			case STATE_CMNT2:
					if (currChar == '/') nextState = STATE_START;
					else nextState = STATE_ERROR;
					break;
			case STATE_CMNT3:
					if (currChar != '\n' || currChar != EOF) nextState = STATE_CMNT3;
					else if (currChar == '\n') nextState = STATE_START;
					else nextState = STATE_ERROR;
					break;

			//MISC 
			case STATE_SPACE:
					if (isspace(currChar)) nextState = STATE_SPACE;
					break;
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
					token->type = T_DIV;
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
		if (token->type == EOF) {
		    break;
		}

		if (token->type != T_UNKNOWN) {
		    ungetc(currChar, file);
		    printf("ungeted \n");
		    break;
		}
		else {
            printf("mame unknown tokens \n");
		    if (nextState == STATE_ERROR) {
		        printf("error \n");
		        if (state != STATE_START) {
		            break;
		        }
		        gettingLex = false;
		    }
		    //break;
		}

		state = nextState;
	}
    return nextState == STATE_ERROR ? 1 : 0;
}