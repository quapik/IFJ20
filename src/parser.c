
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
bool BylMain=false;

//pravidlo 1. <program>-><>package main<body> EOF
int StartParser(tToken *token)
{
    if(((*token)->type)==T_PACKAGE) //prvni musi byt package main
    {
        (*token)=(*token)->nextToken;
        if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0)) //id s main
        {
            (*token)=(*token)->nextToken;
            if ((*token)->type == T_EOF) //pouze package main EOF
            {
                return ERR_OK;
            }
            else if ((*token)->type == T_IF || (*token)->type == T_FUNC || (*token)->type == T_FOR ||
                     (*token)->type == T_RETURN || (*token)->type == T_ID || (*token)->type == T_EOL) //TODO _, int float string?
            {
                return body(token);
            }
            else
            {   printf("chyba po package main");
                return ERR_SYNTAX;
            }
        }
        else
        {
            return ERR_SYNTAX;
        }
    }
    else //zacina to necim jinym nez package main
    {
        return ERR_SYNTAX;
    }
}


int body(tToken *token)
{   //PRAVIDLO <IF> if vyraz { EOL <BODY>} else { EOL <BODY> }
    if ((*token)->type==T_IF)
    {
        (*token)=(*token)->nextToken;
       //TED MUSI BYT VYRAZ
        (*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return ERR_SEM_KOMP;
        }

        (*token)=(*token)->nextToken;

        if ((*token)->type==T_LCBR) // token je {
        {
            (*token)=(*token)->nextToken;
            if ((*token)->type==T_EOL)
            {
                (*token)=(*token)->nextToken;
                if (body(token)==ERR_OK) //TODO jakej token je vracenej?
                {
                    if ((*token)->type==T_RCBR) // }
                    {
                        (*token)=(*token)->nextToken;
                        if ((*token)->type==T_ELSE)
                        {
                            (*token)=(*token)->nextToken;
                            if ((*token)->type==T_LCBR) // token je {
                            {
                                (*token)=(*token)->nextToken;
                                if ((*token)->type==T_EOL)
                                {
                                    (*token)=(*token)->nextToken;
                                    if (body(token)==ERR_OK) //TODO jakej token je vracenej?
                                    {
                                        if ((*token)->type==T_RCBR) // } konec else
                                        {
                                            return ERR_OK; //cele pravidlo if je ok
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        printf("chyba v body");
        return ERR_SYNTAX; //pokud to dodje sem tak nekde byla chybe
    }
    //pravidlo <FOR> -> for <FORDEF>; <vyraz>; <prikaz_prirazeni> { EOL <BODY> }
    else if ((*token)->type==T_FOR)
    {
        (*token)=(*token)->nextToken;
        //PRAVIDLO <FORDEF> -> <DATATYPE> ID  a <FORDEF>EPS

        if (((*token)->type==T_STRING) || ((*token)->type==T_INT) || ((*token)->type==T_EXP)) //TODO FLOAT?
        {
            (*token)=(*token)->nextToken;
            if ((*token)->type==T_ID)
            {
                (*token)=(*token)->nextToken; //mame <DATAYPE> ID a ted musi byt strednik
            }
            else
            {   printf("chyba for");
                return ERR_SYNTAX;
            }

        }
        if ((*token)->type==T_SEMICOLON) //
        {   //uspesne bylo datype ID a nebo bylo prazdne (pouze strednik)
            //TODO ted musi byt vyraz
        }
        else
        {
            return ERR_SYNTAX;
        }

    }
    else if ((*token)->type==T_EOL)
    {
        (*token)=(*token)->nextToken;
        return body(token);
    }
    //PRAVIDLO <FUNC> -> func ID (<PARAMS>)(NAVRATTYPE_N){ EOL <BODY>}
    else if ((*token)->type==T_FUNC)
    {
        (*token)=(*token)->nextToken;
        if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0))
        {
            if (BylMain==false)
            {
                (*token)=(*token)->nextToken;
                if ((*token)->type==T_LDBR) // (
                {
                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_RDBR) // )
                    {
                        BylMain=true;
                        (*token)=(*token)->nextToken;
                        return body(token);
                    }
                    else //pokud to neni main()
                    {   printf("chyba main()");
                        return ERR_SEM_POCET;
                    }
                }

            } //pokud jiz byl main a je znovu main
            else
            { printf("main again");
               return ERR_SYNTAX; //TODO?
            }

        }
        else if ((*token)->type==T_ID) //Dalsi typy funkci
        {
            (*token)=(*token)->nextToken;
            if ((*token)->type==T_LDBR) // (
            {
                //TODO CALL <PARAMS>
            }
        }


    }

    else if ((*token)->type==T_ID) //Máme identifikátor (s hodnotou)
    {
        (*token)=(*token)->nextToken;
        return id_next(token);
    }
    //PRAVIDLO <prirazeni> -> _ = vyraz
    else if(((*token)->type==T_ID)&&(strcmp((*token)->data,"_")==0)) // _
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ASSIGN) //
        {
            //TODO VYRAZ
        }
    }
    else if ((*token)->type==T_EOF)
    {
        return ERR_OK;
    }
    else
    {   printf("konec body");
        return ERR_SYNTAX;
    }


}
//Pravidla 31 <ID_NEXT> -> := vyraz  a 35 , <ID_NEXT> -> <ID_N>=<VICE_ID_VLEVO>
int id_next(tToken *token)
{
    if((*token)->type==T_DEFINE) // pokud je :=
    {
        (*token)=(*token)->nextToken;
        //TED MUSI BYT VYRAZ
        (*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return ERR_SEM_KOMP;
        }
        (*token)=(*token)->nextToken;
        return body(token);
    }
    else if ((*token)->type==T_COMMA) //pokud je carka
    {
        return id_n(token); //volat <ID_> (vice identifikatoru za sebou)
    }
    else
    {
        return ERR_SYNTAX;
    }
}
//pravidlo 15 a 16 <ID_N> -> , ID <ID_N> & <ID_N> -> EPS
int id_n(tToken *token)
{
    if((*token)->type==T_COMMA) //pri prvnim volani je to jiste, pak kontrola
    {
        (*token) = (*token)->nextToken;
        if ((*token)->type == T_ID) //ted musi byt id jinak chyba
        {
            (*token) = (*token)->nextToken;
            return id_n(token); //rekurzivni volani, kde se jiz kontroluje carka nebo =
        }
        else
        {
            return ERR_SYNTAX;
        }

    }
    else if ((*token)->type==T_DEFINE) //=
    {
        return vice_id_vlevo(token);
    }
    else //neco jinyho nez carka nebo = automaticky error
    {
        return ERR_SYNTAX;
    }

}
//PRAVIDLA <VICE_ID_VLEVO> -> vyraz <vyraz_n> NEBO <VICE_ID_VLEVO> -> ID (<PARAMSCALL>
int vice_id_vlevo(tToken *token)
{
    if (((*token)->type==T_ID) && ((*token)->nextToken->type==T_LDBR))
    {
        (*token) = (*token)->nextToken;  (*token) = (*token)->nextToken;
        //TODO CALL PARAMSCALL
        //TODO SEMANTIKA IDčka
    }
    else
    {
        //TED MUSI BYT VYRAZ
        (*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return ERR_SEM_KOMP;
        }
        (*token)=(*token)->nextToken;
        return body(token);

    }

}












