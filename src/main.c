/*
# Implementace p�eklada�e imperativn�ho jazyka IFJ20
# T�m 041, varianta I
# Auto�i:	xfabia13, Michal Fabi�n
#
#
*/

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

int main(int argc, char *argv[]){
	FILE *fp;
	if(argc<2){
		fprintf(stderr,"Nebyl zadan vstupni soubor\n");
		return ERR_INTERNI;
	}
	if(argc>2){
		fprintf(stderr,"Nesmi byt zadany zadne dalsi parametry krome nazvu vstupniho souboru\n");
		return ERR_INTERNI;
	}
	if((fp=fopen(argv[1],"r"))==NULL){
		fprintf(stderr,"Soubor se nepodarilo precist\n");
		return ERR_INTERNI;
	}
	
	/*
		Doplnit podle implementace:		Inicializaci tab. symbolu, atd.
										Predani rizeni a zpracovani
										Nakonec uvolneni pameti
	*/
	
	fclose(fp);
	return ERR_OK;
}
