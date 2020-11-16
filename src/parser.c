
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
int StartParser(tToken *token)
{
    if((*token)->type==T_EOF) //konec souboru (je to to jedine, co soubor obsahujhe)
        {
        return ERR_OK;
        } 
    else if ((*token)->type==T_IF || (*token)->type==T_FUNC || (*token)->type==T_FOR || (*token)->type==T_RETURN || (*token)->type==T_ID || (*token)->type==T_EOL ||(*token)->type==T_ID) //TODO _, int float string?
        {
        return body(&token);
        }
    else
        {
        return ERR_SYNTAX;
        }
        
}
int body(tToken *token)
{
    if (token->type==T_IF)
    {
       (*token)=(*token)->nextToken;
       //TODO KONTROLA zda nasleduje vyraz a je vporadku
        
        if ((*token)->type==T_LCBR) // token je {
            {
                 (*token)=(*token)->nextToken;
                 if ((*token)->type==T_EOL)
                 {
                     (*token)=(*token)->nextToken;
                     return body(&token);
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

    else if (token->type==T_ID) //Máme identifikátor (s hodnotou)
    {
        (*token)=(*token)->nextToken;
        return id_next(&token);

    }

    else
    {
        return ERR_SYNTAX; 
    }


}
//Pravidla 31 <ID_NEXT> -> := vyraz  a 35 , <ID_NEXT> -> <ID_N>=<VICE_ID_VLEVO>
int id_next(tToken *token)
{
    if(token->type==T_ASSIGN) // pokud je :=
    {
        //TODO kontrolva vyrazu
    }
    else if (token->type==T_COMMA) //pokud je carka
    {
       return id_n(&token); //volat <ID_> (vice identifikatoru za sebou)
    }
    else
    {
        return ERR_SYNTAX;
    }
}
//pravidlo 15 a 16 <ID_N> -> , ID <ID_N> & <ID_N> -> EPS
int id_n(tToken *token)
{
    if(token->type==T_COMMA) //pri prvnim volani je to jiste, pak kontrola
    {
        (*token) = (*token)->nextToken;
        if (token->type == T_ID) //ted musi byt id jinak chyba
        {
            (*token) = (*token)->nextToken;
            return id_n(&token); //rekurzivni volani, kde se jiz kontroluje carka nebo =
        }
        else
        {
            return ERR_SYNTAX;
        }

    }
    else if (token->type==T_DEFINE) //=
    {
        return id_vice_nalevo(&token);
    }
    else //neco jinyho nez carka nebo = automaticky error

    {
        return ERR_SYNTAX;
    }

}

int id_vlevo_vice(tToken *token)
{

    //TODO
}












