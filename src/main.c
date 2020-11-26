#include "main.h"
#include "scanner.c"
#include "parser.c"
#include "exprBottomUp.c"


int main(int argc, char *argv[]) {
    FILE *fp = fopen("str.txt", "r");

    if(argc<2){
        fprintf(stderr,"Nebyl zadan vstupni soubor\n");
        return 1;
    }
    if(argc>2){
        fprintf(stderr,"Nesmi byt zadany zadne dalsi parametry krome nazvu vstupniho souboru\n");
        return 1;
    }
    if((fp=fopen(argv[1],"r"))==NULL){
        fprintf(stderr,"Soubor se nepodarilo precist\n");
        return 1;
    }


    tToken token1 = NULL;
    if (scannerLoadTokens(&token1, fp) != 0) printf("nastala chyba v ramci lexikalni analyzy\n");
    //char *data = token1->data;
    exprBUParse(&token1);
    // scannerPrint(token1);

    return 0;
}

