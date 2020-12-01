//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Generování kódu
*/

#include "codegen.h"

void CodeGenStart()
{
    printf(".IFJcode20\n");
    printf("JUMP $$main\n");
}
void CodeGenDefVar(char* id)
{
    printf("DEFVAR @LF %s\n",id);
}

void CodeGenPrint(tToken *token)
{
    if((*token)->type==T_INT)
    {
        printf("WRITE INT");
        //print cislo
    }
    else if((*token)->type==T_STRING)
    {
        //print string
        printf("WRITE string");
    }
    else if((*token)->type==T_EXP)
    {
        printf("WRITE exp");
        //print flaot? TODO co double?
    }
    else if((*token)->type==T_ID)
    {
        printf("WRITE LF@%s\n",(*token)->data);//print variable
    }

}

