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

int DynamicStringInit(DynamicString *Dstr){
Dstr->delka=0;
Dstr->str= (char *)malloc(sizeof(char)*16);
//todo kdyz selze
Dstr->str[0]='\0; //pridani konce stringu  
}
