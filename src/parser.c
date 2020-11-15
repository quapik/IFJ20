
//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/

#include "parser.h"


//pravidlo 1. <program>-><body> EOF
int program(Token *token) //TODO určitě blbě
{
    if(token->type==T_EOF) 
        {
        return ERR_OK;
        } 
    else if (token->type==T_IF || token->type==T_FUNC || token->type==T_FOR || token->type==T_RETURN || token->type==T_ID || token->type==T_EOL || token->type==T_ID) //TODO _, int float string?
        {
        return body(token);
        }
    else
    {
        return ERR_SYNTAX; 
    }
        
}
int body(Token *token)
{
    if (token->type==T_IF)
    {
        token=token->nextToken;
       //TODO KONTROLA zda nasleduje vyraz a je vporadku
        
        if (token->type==T_LCBR) // token je {
            {
                 token=token->nextToken;
                 if (token->type==T_EOL)
                 {
                     token=token->nextToken;
                     return body(token);
                 }
               
                //TODO ELSE
            }

    }
    else if (token->type==T_FOR)
    {


    }

     else if (token->type==T_FUNC)
    {

        
    }
    else
    {
        return ERR_SYNTAX; 
    }
    }
    




}












