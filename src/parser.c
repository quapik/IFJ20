
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
bool BylMain=false; //pomocna pro to jestli byla fce main
tToken pomToken;
bool PossibleEof=false; //aby nenastal eof v tele ifu apod
int PocetKoncovychZavorek=0; //promenna pro kontrolu zda je stejny pocet { & }
int IDCounter=0;

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
            (*token)=(*token)->nextToken;
            (*token)=body(token);
            return *token;

    }
    else if (((*token)->type==T_ID)&&(strcmp((*token)->data,"print")==0))
    {
        (*token)=(*token)->nextToken;
        if((*token)->type==T_LDBR)
        {   (*token)=(*token)->nextToken;
            while((*token)->type==T_STRING||(*token)->type==T_ID||(*token)->type==T_COMMA)
            {
                (*token)=(*token)->nextToken;
            }
            if((*token)->type==T_RDBR)
            {
                (*token)=(*token)->nextToken;
                (*token)=body(token);
                return *token;
            }
        }
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";  printf("CHYBA FIRST\n");
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
        {    (*token)=(*token)->nextToken;
            (*token)=body(token);
                return *token;
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
            (*token)=(*token)->nextToken;
            (*token)=body(token);
            return *token;
        }

    }
        //PRAVIDLO <prirazeni> -> _ = vyraz
    else if(((*token)->type==T_ID)&&(strcmp((*token)->data,"_")==0)) // _
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ASSIGN) // =
        {
            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token);
            if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
            {
                return *token;
            }

            (*token)=body(token);
            return *token;

        }
    }
    else if ((*token)->type==T_ID) //Máme identifikátor (s hodnotou)
    {
        (*token)=(*token)->nextToken;
        (*token)=id_next(token);

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_NEXT
        {
            printf("chyba v ID_NEXT_body\n");  return *token;
        }
        (*token)=(*token)->nextToken;
        (*token)=body(token);
        return *token;
    }

        //PRAVIDLO <BODY> -> EOL
    else if ((*token)->type==T_EOL)
    {
        (*token)=(*token)->nextToken;
        (*token)=body(token); //kontrola za kdyz zavolame body tak probehlo vporadku

        return *token;

    }
        //PRAVIDLO <BODY> -> EOF
    else if ((*token)->type==T_EOF)
    {   printf("Pocet koncovych zavorek %d\n",PocetKoncovychZavorek);
        if ((PossibleEof==true)&&(PocetKoncovychZavorek==0))
        { (*token)->type=T_UNKNOWN;
            if (BylMain==false) //TODO nejak ti to nevypisuje main chybu kdyz chybi
            {
                (*token)->data="ERR_SEM_PROG"; printf("Chybi fce main\n");
            }
            else
            {
                (*token)->data="ERR_OK";
            }
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN;
            (*token)->data="ERR_SYNTAX"; printf("Chybny oef\n");
            return *token;
        }

    }
    else if ((*token)->type==T_RCBR)
    {
        (*token)=(*token)->prevToken;
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

tToken if_rule(tToken *token)
{   PossibleEof=false;
    //TED MUSI BYT VYRAZ

    (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
    if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
    {
        return *token;
    }


    if ((*token)->type==T_LCBR) // token je {
    {   PocetKoncovychZavorek++;
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_EOL)
        {
            (*token)=(*token)->nextToken;
            (*token)=body(token); //telo IF
            if((*token)->type==T_UNKNOWN) //pokud v tele ife nastala chyba
            {
                printf ("chybe v IFbody"); return *token;
            }

            (*token)=(*token)->nextToken;

                if ((*token)->type==T_RCBR) // } ukonocovaci zavorka ifu
                {   PocetKoncovychZavorek--;
                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_ELSE)
                    {
                        (*token)=(*token)->nextToken;
                        if ((*token)->type==T_LCBR) // token je { otviraci zavorku elsu
                        {   PocetKoncovychZavorek++;
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
                                    {   PocetKoncovychZavorek--;
                                        PossibleEof=true; return *token; //cele pravidlo if je ok takze vracime aktualni
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

tToken func_rule(tToken *token)
{   PossibleEof=false;
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
                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_LCBR) // {
                    {   PocetKoncovychZavorek++;
                        BylMain=true;
                        (*token)=(*token)->nextToken;

                        (*token)=body(token);
                        if((*token)->type==T_UNKNOWN)
                        {
                            return *token;
                        }
                        else
                        {
                            (*token)=(*token)->nextToken;
                            if ((*token)->type==T_RCBR) // }
                            {   PossibleEof=true; PocetKoncovychZavorek--;
                                (*token)=(*token)->nextToken; //ted se ceka eof
                                (*token)=body(token);
                                return *token;
                            }
                        }

                    }
                    else {(*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; return *token; }

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
            (*token)=(*token)->nextToken;
            (*token)=params(token);

            if ((*token)->type==T_UNKNOWN) //nastala chyba v params
                {
                    return *token;
                }
            (*token)=(*token)->nextToken; //kdyz ok tak by mel nasledovat ( NEBO { kdyz je seznam navartovych prazdny
            if ((*token)->type==T_LDBR) // (
            {
                (*token)=(*token)->nextToken;
                (*token)=narvrattype_n(token);

                if ((*token)->type==T_UNKNOWN) //nastala chyba v navrattype_n a nebo je vraceno )
                {
                    return *token;
                }
                (*token)=(*token)->nextToken;
                if ((*token)->type==T_LCBR) // {
                {   PocetKoncovychZavorek++;
                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_EOL)
                    {
                        (*token)=(*token)->nextToken;
                        (*token)=body(token);
                        if ((*token)->type==T_UNKNOWN) //v body neco spatne
                        {
                            return *token;
                        }
                        (*token)=(*token)->nextToken;
                        if ((*token)->type==T_RCBR) // }a tim ukonceni tela funkce
                        {   PocetKoncovychZavorek--;
                            (*token)=(*token)->nextToken;
                            (*token)=body(token);
                            return *token;
                        }
                        else
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole1\n"); return *token;
                        }

                    }
                    else
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole2\n"); return *token;
                    }

                }
                else
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole3\n"); return *token;
                }

            }
            else if ((*token)->type==T_LCBR) // je rovnou { ->  nejsou zadne navratove datove typy
            {   PocetKoncovychZavorek++;
                (*token)=(*token)->nextToken;
                if ((*token)->type==T_EOL)
                {
                    (*token)=body(token);
                    if ((*token)->type==T_UNKNOWN) //v body neco spatne
                    {
                        return *token;
                    }
                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_RCBR) // }a tim ukonceni tela funkce
                    {   PocetKoncovychZavorek--;
                        (*token)=(*token)->nextToken;
                        (*token)=body(token);
                        return *token;
                    }
                    else
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole4\n"); return *token;
                    }
                }
                else
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole5\n"); return *token;
                }


            }
            else
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole6\n"); return *token;
            }



        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba func_dole7\n"); return *token;
        }
    }


}
//PRAVIDLO <navrattype_n> -> <DATATYPE> , <NAVRATTYPE_N>  nebo <navrattype_n> -> EPS
tToken narvrattype_n(tToken *token)
{
    if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_COMMA)
        {
            (*token)=(*token)->nextToken;
            (*token)=narvrattype_n(token);
            return *token;
        }
        else if ((*token)->type==T_RDBR)
        {
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba navrattype_n1\n"); return *token;
        }
    }
    else if ((*token)->type==T_RDBR)
    {
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba navrattype_n2\n"); return *token;
    }

}
//pravidla <PARAMS> -> ID <DATATYPE> <PARAMS_N> nebo <PARAMS> ->  EPS
tToken params(tToken *token)
{

    if ((*token)->type==T_ID)
    {
        (*token)=(*token)->nextToken;
        if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
        {
            (*token)=(*token)->nextToken;
            (*token)=params_n(token);
            if ((*token)->type==T_UNKNOWN) //nastala chyba v params
            {
                return *token;
            }
            return *token; //return do func


        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba params1\n"); return *token;
        }
    }
    else if ((*token)->type==T_RDBR)
    {
        return *token; //vraci ) do func
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba params2\n"); return *token;
    }

}
//Pravidlo <params_n> -> , ID  <DATATYPE> <PARAMS_N> nebo  <params_n> -> EPS
tToken params_n(tToken *token)
{
    if ((*token)->type==T_COMMA)
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ID)
        {   (*token)=(*token)->nextToken;
            if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
            {
                (*token)=(*token)->nextToken;
                (*token)=params_n(token);
                return *token;
            }
            else
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba params_n1\n"); return *token;
            }

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba params_n2\n"); return *token;
        }

    }
    else  if ((*token)->type==T_RDBR) //)
    {
        return *token; //ukonceni a vraceni )
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba params_n3\n"); return *token;
    }


}

tToken for_rule(tToken *token) //TODO KONTROLA zda nakonci FORU }
{
    PossibleEof=false;
    if ((*token)->type==T_ID)
    {
            (*token)=(*token)->nextToken;
        if ((*token)->type==T_DEFINE)
        {
            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token);
            if ((*token)->type==T_UNKNOWN)
            {
                return *token;
            }

        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for po ID\n");    return *token;
        }
    }

    if ((*token)->type==T_SEMICOLON) //je strednik? bud je prvni nebo uspesne bylo ID=vyraz
    {
        (*token)=(*token)->nextToken; //ted povinny vyraz
        (*token)=exprBUParse(token);
        if ((*token)->type == T_UNKNOWN)
        {
            return *token;
        }

        if ((*token)->type==T_SEMICOLON) //povinny ;
            {
                (*token)=(*token)->nextToken; //bud id=vyraz nebo rovnou {
                if ((*token)->type==T_ID)
                {   (*token)=(*token)->nextToken;
                    if ((*token)->type==T_ASSIGN)
                    {
                        (*token)=(*token)->nextToken;
                        (*token) = exprBUParse(token);
                        if ((*token)->type == T_UNKNOWN)
                        {
                            return *token;
                        }
                    }
                }

              if ((*token)->type==T_LCBR)
              {
                  PocetKoncovychZavorek++;
                  (*token)=(*token)->nextToken;
                  if ((*token)->type==T_EOL) //povinny eol
                  {   (*token)=(*token)->nextToken;
                      (*token)=body(token); //telo foru
                      if ((*token)->type == T_UNKNOWN)
                      {
                          return *token; //chyba v tele foru
                      }
                      (*token)=(*token)->nextToken;
                      if ((*token)->type==T_RCBR) //} koncova zavorka tela foru
                      {
                          PocetKoncovychZavorek--;
                          return *token;
                      }
                      else
                      {
                          (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for1\n");   return *token;
                      }
                  }

              }
              else
              {
                  (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for2\n");   return *token;
              }
             }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for3\n");   return *token;
        }

    }
    else
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for4\n");   return *token;
    }

}
//pravidlo <ID_NEXT> -> := vyraz nebo <ID_NEXT> -> <ID_N>=<vice_id_vlevo>
tToken id_next(tToken *token)
{
    if((*token)->type==T_DEFINE) // pokud je :=
    {
        //printf("DEFVAR @GF %s",((*token)->prevToken->data));
        (*token)=(*token)->nextToken;
        (*token)=exprBUParse(token); //do tokenu bud T_UNKNOWN nebo nasledujici znak
        return *token;
    }
    else if ((*token)->type==T_COMMA) //pokud je carka
    {
        (*token)=id_n(token); //volat <ID_N> (vice identifikatoru za sebou)

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_N
        {
            return *token;
        }
        if ((*token)->type==T_ASSIGN) //=
        {
            (*token)=(*token)->nextToken;
            (*token)=vice_id_vlevo(token);
            return *token; //return do body kde se vola
        }

        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_NEXT1\n"); return *token;
        }
    }
    else if ((*token)->type==T_ASSIGN) // JEN ID=
    {   IDCounter=1;
        (*token)=(*token)->nextToken;
        (*token)=vice_id_vlevo(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_NEXT2\n"); return *token;
    }
}
//pravidlo 15 a 16 <ID_N> -> , ID <ID_N> & <ID_N> -> EPS
tToken id_n(tToken *token)
{   IDCounter++;
    if((*token)->type==T_COMMA) //pri prvnim volani je to jiste, pak kontrola
    {
        (*token) = (*token)->nextToken;
        if ((*token)->type == T_ID) //ted musi byt id jinak chyba
        {
            (*token) = (*token)->nextToken;
            (*token) = id_n(token); //rekurzivni volani, kde se jiz kontroluje carka nebo =
            return *token;

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_N1\n"); return *token;
        }

    }
    else if ((*token)->type==T_ASSIGN) //=
    {
      return *token;
    }
    else //neco jinyho nez carka nebo = automaticky error
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_N2\n"); return *token;
    }

}
//PRAVIDLA <VICE_ID_VLEVO> -> vyraz <vyraz_n> NEBO <VICE_ID_VLEVO> -> ID (<PARAMSCALL>
tToken vice_id_vlevo(tToken *token)
{
    if (((*token)->type==T_ID) && ((*token)->nextToken->type==T_LDBR))
    {
        (*token)=(*token)->nextToken;  (*token)=(*token)->nextToken;
        (*token)=paramscall(token); //vraceno ) nebo chyba z paramscall
        return *token;
        //TODO SEMANTIKA IDčka
    }
    else
    {

        (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {

            return *token;
        }
        (*token)=vyraz_n(token);

    }

}

//PRAVIDLO <paramscall> -> ID <PARAMSCALL_N>
tToken paramscall(tToken *token)
{
    if ((*token)->type==T_ID)
    {
        //TODO SEMANTIKA ID
        (*token)=(*token)->nextToken;
        (*token)=paramscall_n(token); //je vracenej bud error nebo ) a toto vracime do vice_id_vlevo
        return *token;
    }
    else  if ((*token)->type==T_RDBR) //je tam jen () - nějakej input()
    {
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
        if (((*token)->type==T_ID)||((*token)->type==T_INT)||((*token)->type==T_STRING)||((*token)->type==T_EXP))
        {
            (*token)=(*token)->nextToken;
            (*token)=paramscall_n(token);
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba paramscall_n1\n"); return *token;
        }

    }
    else if ((*token)->type==T_RDBR) //)
    {
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba paramscall_n2\n"); return *token;
    }

}
//PRAVIDLO <vyraz_n> -> , vyraz <vyraz n>
tToken vyraz_n(tToken *token){
    IDCounter--; //TODO nehodi to errory kdyz je tam id1,i2=vyraz, NIC
    if ((*token)->type==T_COMMA) //nastala chyba pri vyrazu
    {
        (*token)=(*token)->nextToken;
        (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }
        (*token)=vyraz_n(token);
    }
    else
        { //neni uz zadny dalsi vyraz a nasleduje neco z body (returny az uplne na zacatek kde bude body)
        if(IDCounter!=0) //nebyl stejny pocet identifikatoru vlevo a vyrazu vpravo
        {   printf("IDCOUNTER %d\n",IDCounter);
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("chyba-> ruzny pocet id a vyrazu  <vyraz_n>\n"); return *token;
        }
        return *token;
        }
}


