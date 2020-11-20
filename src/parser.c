
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
tToken pomToken;
//pravidlo 1. <program>-><>package main<body> EOF
int StartParser(tToken *token)
{
    if(((*token)->type)==T_PACKAGE) //prvni musi byt package main
    {
        (*token)=(*token)->nextToken;
        if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0)) //id s main
        {
            (*token)=(*token)->nextToken;

             if ((*token)->type == T_IF || (*token)->type == T_FUNC || (*token)->type == T_FOR ||
                     (*token)->type == T_RETURN || (*token)->type == T_ID || (*token)->type == T_EOL || (*token)->type == T_EOF)//TODO _, int float string?
            {
                pomToken=body(token);
                if((*pomToken).type==T_UNKNOWN) //vzdy to v tomto ifu musi skoncit (bud errorem nebo po EOFU s OK)
                {
                if(strcmp((*pomToken).data,"ERR_OK")==0) return ERR_OK;
                if(strcmp((*pomToken).data,"ERR_SYNTAX")==0) return ERR_SYNTAX;
                if(strcmp((*pomToken).data,"ERR_SEM_PROG")==0) return ERR_SEM_PROG;
                if(strcmp((*pomToken).data,"ERR_SEM_TYPE")==0) return ERR_SEM_TYPE;
                if(strcmp((*pomToken).data,"ERR_SEM_KOMP")==0) return ERR_SEM_KOMP;
                if(strcmp((*pomToken).data,"ERR_SEM_POCET")==0) return ERR_SEM_POCET;
                if(strcmp((*pomToken).data,"ERR_SEM_OSTATNI")==0) return ERR_SEM_OSTATNI;
                if(strcmp((*pomToken).data,"ERR_SEM_DELENI")==0) return ERR_SEM_DELENI;
                if(strcmp((*pomToken).data,"ERR_INTERNI")==0) return ERR_INTERNI;
                }

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


tToken body(tToken *token)
{
    //PRAVIDLO <FUNC> -> func ID (<PARAMS>)(NAVRATTYPE_N){ EOL <BODY>}
     if ((*token)->type==T_FUNC)
    {
        (*token)=(*token)->nextToken;
        (*token)=func_rule(token);

        if ((*token)->type==T_UNKNOWN) //chyba v ifu
        {
            return *token;
        }
        else
        {
            (*token)=body(token);
            if ((*token)->type==T_UNKNOWN) //chyba v body nebo EOF (ten urcite jednou musi prijit)
            {
                return *token;
            }
        }


    }
    //PRAVIDLO <IF> if vyraz { EOL <BODY>} else { EOL <BODY> }
    else if ((*token)->type==T_IF)
    {
        (*token)=(*token)->nextToken;
        (*token)=if_rule(token);
        if ((*token)->type==T_UNKNOWN) //chyba v ifu
        {
            return *token;
        }
        else
        {
            (*token)=body(token);
            if ((*token)->type==T_UNKNOWN) //chyba v body nebo EOF (ten urcite jednou musi prijit)
            {
                return *token;
            }
        }


    }
         //pravidlo <FOR> -> for <FORDEF>; <vyraz>; <prikaz_prirazeni> { EOL <BODY> }
     else if ((*token)->type==T_FOR)
     {
         (*token)=(*token)->nextToken;
         (*token)=for_rule(token);
         if ((*token)->type==T_UNKNOWN) //chyba v foru
         {
             return *token;
         }
         else
         {
             (*token)=body(token);
             if ((*token)->type==T_UNKNOWN) //chyba v body nebo EOF (ten urcite jednou musi prijit)
             {
                 return *token;
             }
         }

     }
         //PRAVIDLO <prirazeni> -> _ = vyraz
     else if(((*token)->type==T_ID)&&(strcmp((*token)->data,"_")==0)) // _
     {
         (*token)=(*token)->nextToken;
         if ((*token)->type==T_ASSIGN) // =
         {
             (*token)=(*token)->nextToken;
             //TED MUSI BYT VYRAZ
             //(*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
             if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
             {
                 return *token;
             }
             (*token)=(*token)->nextToken;
             (*token)=body(token);
             return *token; //TODO melo by to asi stacit takto

         }
     }
     else if ((*token)->type==T_ID) //Máme identifikátor (s hodnotou)
     {
         (*token)=(*token)->nextToken;
         *(token)=id_next(token);
         if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_NEXT
         {
             printf("chyba v ID_NEXT\n");  return *token;
         }
         (*token)=(*token)->nextToken;
         (*token)=body(token);
         return *token; //TODO melo by to asi stacit takto
     }

         //PRAVIDLO <BODY> -> EOL
     else if ((*token)->type==T_EOL)
     {
         (*token)=(*token)->nextToken;
         (*token)=body(token); //kontrola za kdyz zavolame body tak probehlo vporadku
         if((*token)->type==T_UNKNOWN)
         {
             return *token;
         }
     }
    //PRAVIDLO <BODY> -> EOF
     else if ((*token)->type==T_EOF)
    {
        (*token)->type=T_UNKNOWN;
        if (BylMain==false)
        {
            (*token)->data="ERR_SEM_PROG"; printf("Chybi fce main\n");
        }
        else
        {
            (*token)->data="ERR_OK";
        }
        return *token;
    }
    //ZADNE FIRST z BODY nesedi
    else
    {
        (*token)->type=T_UNKNOWN;
        (*token)->data="ERR_SYNTAX";  printf("Chybne FIRST v body\n");
        return *token;
    }


}

tToken if_rule(tToken *token) //TODO KONTROLA zda nakonci IFU }
{
    //TED MUSI BYT VYRAZ
    //(*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
    if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
    {
        return *token;
    }

    (*token)=(*token)->nextToken;

    if ((*token)->type==T_LCBR) // token je {
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_EOL)
        {
            (*token)=(*token)->nextToken;

            (*token)=body(token); //TODO je return kdyz neni chyba?
            if((*token)->type==T_UNKNOWN) //pokud v tele ife nastala chyba
            {
                return *token;
            }

            (*token)=(*token)->nextToken;

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
                                (*token)=body(token);
                                if((*token)->type==T_UNKNOWN) //pokud v tele ELSE nastala chyba
                                {
                                    return *token;
                                }
                                (*token)=(*token)->nextToken;
                                {
                                    if ((*token)->type==T_RCBR) // } konec else
                                    {
                                        return *token; //cele pravidlo if je ok takze vracime aktualni
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    (*token)->type=T_UNKNOWN;
    (*token)->data="ERR_SYNTAX";  printf("Chybne v ifu (konec)\n");
    return *token;
}

tToken func_rule(tToken *token) //TODO KONTROLA zda nakonci FUNC }
{
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

                    (*token)=body(token);
                    if((*token)->type==T_UNKNOWN)
                    {
                        return *token;
                    }
                }
                else //pokud to neni main()
                {   printf("chyba main()");
                    (*token)->type=T_UNKNOWN;
                    (*token)->data="ERR_SEM_POCET";
                    return *token;
                }
            }

        } //pokud jiz byl main a je znovu main
        else
        {
            (*token)->type=T_UNKNOWN;
            (*token)->data="ERR_SYNTAX"; printf("main again");
            return *token;
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

tToken for_rule(tToken *token) //TODO KONTROLA zda nakonci FORU }
{
    //PRAVIDLO <FORDEF> -> <DATATYPE> ID  a <FORDEF>EPS
    if (((*token)->type==T_STRING) || ((*token)->type==T_INT) || ((*token)->type==T_EXP)) //TODO FLOAT?
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ID)
        {
            (*token)=(*token)->nextToken; //mame <DATAYPE> ID a ted musi byt strednik
        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for\n");
            return *token;
        }

    }
    if ((*token)->type==T_SEMICOLON) //
    {   //uspesne bylo datype ID a nebo bylo prazdne (pouze strednik)
        //TODO ted musi byt vyraz

    }
    else
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for\n");
        return *token;
    }

}

tToken id_next(tToken *token)
{
    if((*token)->type==T_DEFINE) // pokud je :=
    {
        (*token)=(*token)->nextToken;
        //TED MUSI BYT VYRAZ TODO
        //(*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }
        (*token)=(*token)->nextToken;
        (*token)=body(token);
        return *token; //TODO MELO BY TO STACIT?
    }
    else if ((*token)->type==T_COMMA) //pokud je carka
    {
        (*token)=id_n(token); //volat <ID_N> (vice identifikatoru za sebou)

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_N
        {
            return *token;
        }
        (*token)=(*token)->nextToken;
        (*token)=body(token);
        return *token; //TODO MELO BY TO STACIT?
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_NEXT\n"); return *token;
    }
}
//pravidlo 15 a 16 <ID_N> -> , ID <ID_N> & <ID_N> -> EPS
tToken id_n(tToken *token)
{
    if((*token)->type==T_COMMA) //pri prvnim volani je to jiste, pak kontrola
    {
        (*token) = (*token)->nextToken;
        if ((*token)->type == T_ID) //ted musi byt id jinak chyba
        {
            (*token) = (*token)->nextToken;
            (*token) = id_n(token); //rekurzivni volani, kde se jiz kontroluje carka nebo =

            if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_N
            {
                return *token;
            }
        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_N\n"); return *token;
        }

    }
    else if ((*token)->type==T_DEFINE) //=
    {
        (*token)=vice_id_vlevo(token);
        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_vice_vlevo
        {
         printf ("chyba vice_id_vlevo\n");   return *token;
        }
    }
    else //neco jinyho nez carka nebo = automaticky error
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_N\n"); return *token;
    }

}
//PRAVIDLA <VICE_ID_VLEVO> -> vyraz <vyraz_n> NEBO <VICE_ID_VLEVO> -> ID (<PARAMSCALL>
tToken vice_id_vlevo(tToken *token)
{
    if (((*token)->type==T_ID) && ((*token)->nextToken->type==T_LDBR))
    {
        (*token) = (*token)->nextToken;  (*token) = (*token)->nextToken;
        (*token) = paramscall(token);
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri paramscall
        {
           printf("chyba v paramscall\n"); return *token;
        }
        (*token) = (*token)->nextToken;
        (*token) = body(token);
        return *token;
        //TODO SEMANTIKA IDčka
    }
    else
    {
        //TED MUSI BYT VYRAZ
        //(*token)=exprBUParse(&token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }
        (*token)=(*token)->nextToken;
        //TODO CALL VYRAZ_N

    }

}

//PRAVIDLO <paramscall> -> ID <PARAMSCALL_N>
tToken paramscall(tToken *token)
{
    if ((*token)->type==T_ID)
    {    //TODO SEMANTIKA ID
        (*token)=(*token)->nextToken;
        (*token)=paramscall_n(token);
        if ((*token)->type==T_UNKNOWN) //nastala chyba v paramscall
        {
            printf("paramscall chyba\n"); return *token;
        }
        (*token)=(*token)->nextToken;
        (*token)=body(token);
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba paramscall\n"); return *token;
    }

}

//PRAVIDLO <paramscall_n> -> , ID <PARAMSCALL_N> nebo <paramscall_n> -> EPS
tToken paramscall_n(tToken *token)
{
    if ((*token)->type==T_COMMA)
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ID)
        {
            (*token)=(*token)->nextToken;
            (*token)=paramscall_n(token);
            if ((*token)->type==T_UNKNOWN) //nastala chyba v paramscall_n
            {
                printf("paramscall_n chyba\n"); return *token;
            }
        }

    }
    else if ((*token)->type==T_RDBR) //)
    {
        return *token;
    }


}

