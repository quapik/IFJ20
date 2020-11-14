/*
# Implementace p�eklada�e imperativn�ho jazyka IFJ20
# T�m ��slo 041, varianta I
# Auto�i projektu:
#	��ma Vojt�ch 	xsimav01@stud.fit.vutbr.cz
#	Fabi�n Michal   xfabia13@stud.fit.vutbr.cz
#	��bela Radek    xcabel04@stud.fit.vutbr.cz
#	Poposki Vasil   xpopos00@stud.fit.vutbr.cz
#	Prosinec 2020, Fakulta informa�n�ch technologi� VUT v Brn�
# Soubor main.c
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
