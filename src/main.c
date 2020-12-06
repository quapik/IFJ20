#include <stdio.h>
#include "scanner.h"
#include "parser.h"
//#include "exprBottomUp.h"
#include "error.h"


int main() {
    // FILE *fp;
    //
    // if(argc<2){
    //     fprintf(stderr,"Nebyl zadan vstupni soubor\n");
    //     return 99;
    // }
    // if(argc>2){
    //     fprintf(stderr,"Nesmi byt zadany zadne dalsi parametry krome nazvu vstupniho souboru\n");
    //     return 99;
    // }
    // if((fp=fopen(argv[1],"r"))==NULL){
    //     fprintf(stderr,"Soubor se nepodarilo precist\n");
    //     return 99;
    // }

    tToken token1 = NULL;
    if (scannerLoadTokens(&token1) != 0)
    {
        fprintf(stderr, "Error: scanner skoncil s chybou\n");
        return ERR_LEX;
    }
    //debug
    //exprBUParse(&token1);

    int parserErr = StartParser(&token1);
    if (parserErr != 0)
    {
        fprintf(stderr, "Error: parser skoncil s chybou\n");
        return parserErr;
    }

    //debug
    //char *data = token1->data;

    // scannerPrint(token1);

    return ERR_OK;
}
