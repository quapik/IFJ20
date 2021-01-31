//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně

Chybove hodnoty
*/



#pragma once

//Chyby
#define ERR_LEX 1 //chybna struktura aktualniho lexemu
#define ERR_SYNTAX 2 //chybna syntaxe programu, neocekavane odradkovani
#define ERR_SEM_PROG 3 //nedefinovaná fce/promenna, pokus o redefinici
#define ERR_SEM_TYPE 4 //chyba pri odvozovani dat typu nove definovane promenne
#define ERR_SEM_KOMP 5 //chyba typove kompatibality ve vyrazech
#define ERR_SEM_POCET 6 //spatny pocet/typ parametru ci navratovyho hodnot fce
#define ERR_SEM_OSTATNI 7 //ostatni semanticke chyby
#define ERR_SEM_DELENI 9 //semanticka chyba deleni
#define ERR_INTERNI 99 //interni chyba prekladace

//Vse ok
#define ERR_OK 0 //vse probehlo v poradku

