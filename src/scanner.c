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

int scannerDKA(FILE *file, tToken token) {

	// pocitame s platnym souborem

	sState state = STATE_START;
	sState nextState;

	static unsigned int currChar;

	bool gettingLex = true;

	token->type = T_UNKNOWN;

	//TODO Implementovat pamatovani stringu (pole nebo ungetc)

	while(gettingLex) {

		switch(state) {

			if (currChar != EOF) {
				currChar = getc(file);
			}


			case STATE_START:
					if (currChar == '\n') nextState = STATE_EOL;
					else if (currChar == EOF) nextState = STATE_EOF;
					else if (isspace(currChar)) nextState = STATE_SPACE;
					else if (currChar == '/') nextState = STATE_CMNT0;
					else if (isalpha(currChar) || currChar == '_') nextState = STATE_ID;
					else if (currChar == '0') nextState = STATE_INT0;
					else if (isdigit(currChar)) nextState = STATE_INT;
					else if (currChar == '"') nextState = STATE_STR0;
					else if (currChar == '!') nextState = STATE_EXC;
					else if (currChar == '=') nextState = STATE_SEQ;
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
					else nextState == STATE_ERROR;
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
					else if (currChar != '"' || currChar != '\\' || currChar != EOF) nextState = STATE_STR0;
					else nextState = STATE_ERROR;
					break;
			case STATE_STR1:
					if (currChar == 'n' ||
						currChar == 't' ||
						currChar == '\\' ||
						currChar == '"') nextState = STATE_STR0;
					else if (currChar == 'x') nextState = STATE_STRHEX;
					else nextState == STATE_ERROR;
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

			//EXPRESSIONS 
			default:
					break;
		}
	}

}