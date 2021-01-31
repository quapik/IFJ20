//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/

#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "error.h"


int main() {

    tToken token1 = NULL;
    if (scannerLoadTokens(&token1) != 0)
    {
        fprintf(stderr, "Error: scanner skoncil s chybou\n");
        return ERR_LEX;
    }

    int parserErr = StartParser(&token1);
    if (parserErr != 0)
    {
        fprintf(stderr, "Error: parser skoncil s chybou\n");
        return parserErr;
    }

    return ERR_OK;
}
