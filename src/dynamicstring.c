//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/

#include "dynamicstring.h"
unsigned int dsMultiplier = 16; // data string multiplier - k alokaci a realokaci
void DynamicStringInit(DynamicString *Dstr){
Dstr->delka=0;
Dstr->str=(char *)malloc(sizeof(char)*dsMultiplier);
//todo kdyz selze
Dstr->str[0]='\0'; //pridani konce stringu
}

void DynamicStringAdd(DynamicString *Dstr, char c) //pro pridani znaku (ve fci
{
Dstr->str[Dstr->delka]=c; //ulozeni charu na pozici ve stringu
Dstr->str[Dstr->delka]='\0'; //posunuti konce stringu
Dstr->delka++;
}

void DynamicStringFree(DynamicString *Dstr) {
        free(Dstr->str);
        Dstr->delka = 0;
}
