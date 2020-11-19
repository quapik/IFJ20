#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "scanner.c"
#include "parser.h"
#include "parser.c"


int main() {
    FILE *source1 = fopen("str.txt", "r");
    tToken token1 = NULL;
    if (scannerLoadTokens(&token1, source1) != 0) printf("nastala chyba v ramci lexikalni analyzy\n");
    if (StartParser(&token1)!= 0)) printf("nastala chyba v ramci parseru\n");
    return 0;
}

