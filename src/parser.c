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
int IDCounter=0; int IDCounterOpacny=0; int FORCounter=0;
int IFCounter=0;   int ELSECounter=0; //countery pro LABELY pro CODEGEN
char* UchovaniID[10];
bool VeFunkci=false; bool VMainu=false; bool JdemeZReturnu = false;
char* JmenoPromenne;
unsigned int AktualniHloubkaZanoreni=0; unsigned int paramscounter=0;

tSymbolTablePtrPromenna LocalTable;
tSymbolTablePtr GlobalTable;

//pravidlo 1. <program>-><>package main<body> EOF
int StartParser(tToken *token)
{   STableInitLocal(&LocalTable);
    STableInit(&GlobalTable);


    if(((*token)->type)==T_PACKAGE) //prvni musi byt package main
    {
        (*token)=(*token)->nextToken;
        if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0)) //id s main
        {   CodeGenStart();
            (*token)=(*token)->nextToken;

            if ((*token)->type == T_EOL || (*token)->type == T_FUNC) //po package main muže byt jen eol nebo func
            {
                pomToken=body(token);

                if(strcmp((*pomToken).data,"ERR_OK")==0) return ERR_OK;
                if(strcmp((*pomToken).data,"ERR_SYNTAX")==0) return ERR_SYNTAX;
                if(strcmp((*pomToken).data,"ERR_SEM_PROG")==0) return ERR_SEM_PROG;
                if(strcmp((*pomToken).data,"ERR_SEM_TYPE")==0) return ERR_SEM_TYPE;
                if(strcmp((*pomToken).data,"ERR_SEM_KOMP")==0) return ERR_SEM_KOMP;
                if(strcmp((*pomToken).data,"ERR_SEM_POCET")==0) return ERR_SEM_POCET;
                if(strcmp((*pomToken).data,"ERR_SEM_OSTATNI")==0) return ERR_SEM_OSTATNI;
                if(strcmp((*pomToken).data,"ERR_SEM_DELENI")==0) return ERR_SEM_DELENI;
                else return ERR_INTERNI;

            }
            else
            {   // printf("chyba po package main");
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
    {   if((VeFunkci==true)||BylMain==true) {(*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";   return *token;}
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
        //PRAVIDLO <BODY> -> EOL
    else if ((*token)->type==T_EOL)
    {
        (*token)=(*token)->nextToken;
        (*token)=body(token); //kontrola za kdyz zavolame body tak probehlo vporadku
        return *token;
    }

    else if ((BylMain==false&&VeFunkci==false&&VMainu==false)) //kontrola aby pred mainem byly jen funkce
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";   return *token;
    }
    //print TODO
    else if (((*token)->type==T_ID)&&(strcmp((*token)->data,"print")==0))
    {
        (*token)=(*token)->nextToken;
        if((*token)->type==T_LDBR)
        {
            (*token)=(*token)->nextToken;
            (*token)=print_rule(token);
            if ((*token)->type==T_UNKNOWN)
            {
                return *token;
            }

            if((*token)->type==T_RDBR)
            {
                (*token)=(*token)->nextToken;
                (*token)=body(token);
                return *token;
            }
        }
        printf("CHYBA FIRST\n");
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";   return *token;
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
            (*token)=exprBUParse(token); //TODO zahodit to co jsme dostali
            if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
            {
                return *token;
            }
            if(Porovnavani==true) //Vyskytlo se porovnavani na miste kde nemá být
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba v _ \n");  return *token;
            }

            (*token)=body(token);
            return *token;

        }
        else{  (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
            printf("Chybny oef\n");
        }

    }
    else if ((*token)->type==T_ID) //Máme identifikátor (s hodnotou)
    {
        (*token)=(*token)->nextToken;
        (*token)=id_next(token);

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_NEXT
        {
            // printf("chyba v ID_NEXT_body\n");
            return *token;
        }
        (*token)=(*token)->nextToken;
        (*token)=body(token);
        return *token;
    }

        //PRAVIDLO <BODY> -> EOF
    else if ((*token)->type==T_EOF)
    {  // printf("Pocet koncovych zavorek %d\n",PocetKoncovychZavorek);

        STableDisposeLocal(&LocalTable);
        if ((PossibleEof==true)&&(PocetKoncovychZavorek==0))
        { (*token)->type=T_UNKNOWN;
            if (BylMain==false) //TODO nejak ti to nevypisuje main chybu kdyz chybi
            {
                (*token)->data="ERR_SEM_PROG";
                printf("Chybi fce main\n");
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
            (*token)->data="ERR_SYNTAX";
            printf("Chybny oef\n");
            return *token;
        }

    }
    else if ((*token)->type==T_RCBR)
    {
        (*token)=(*token)->prevToken;
        return *token;
    }
        //pravidlo return -> return
    else if ((*token)->type==T_RETURN)
    {   if (VeFunkci==true)
        {
            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
            if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
            {
                return *token;
            }
                if(Porovnavani==true) //pokud ve vyrazu vyskytlo porovnavani
                {
                    (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba v returnu \n");  return *token;
                }
            JdemeZReturnu=true;
            (*token)=vyraz_n(token);
            JdemeZReturnu=false;
            if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
            {
                return *token;
            }
            (*token)=body(token);
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN;
            (*token)->data="ERR_SYNTAX";
            printf("Chybny return\n");
            return *token;
        }

    }
        //ZADNE FIRST z BODY nesedi
    else
    {
        (*token)->type=T_UNKNOWN;
        (*token)->data="ERR_SYNTAX";
        printf("Chybne FIRST v body\n");
        return *token;
    }
    return *token; //TODO divny
}
tToken print_rule(tToken *token)
{
    if((*token)->type==T_DOUBLE||(*token)->type==T_STRING||(*token)->type==T_ID||(*token)->type==T_EXP||(*token)->type==T_INT)
    {
        CodeGenPrint(token);
        (*token)=(*token)->nextToken;
        if((*token)->type==T_COMMA)
        {   (*token)=(*token)->nextToken;
            (*token)=print_rule(token);
            return *token;
        }
        else if((*token)->type==T_RDBR){
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("Chyba v printu \n");  return *token;
        }
    }
    else
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("Chyba v printu \n");  return *token;
    }

}
tToken if_rule(tToken *token)
{   PossibleEof=false;
    //TED MUSI BYT VYRAZ

    (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
    if(Porovnavani==false) //pokud ve vyrazu nebylo porovnavani
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba ifu-nebylo porovnavani \n");  return *token;
    }
    if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
    {
        return *token;
    }



    if ((*token)->type==T_LCBR) // token je {
    {   PocetKoncovychZavorek++; IFCounter++; ELSECounter=IFCounter;
        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        printf("JUMPIFNEQ $elsedlabel%d TF@$return bool@true\n",IFCounter);
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_EOL)
        {
            (*token)=(*token)->nextToken;
            (*token)=body(token); //telo IF
            if((*token)->type==T_UNKNOWN) //pokud v tele ife nastala chyba
            {
                printf ("chybe v IFbody");
                return *token;
            }

            (*token)=(*token)->nextToken;

            if ((*token)->type==T_RCBR) // } ukoncovaci zavorka ifu
            {   printf("JUMP $endiflabel%d\n",IFCounter);
                printf("LABEL $elselabel%d\n",IFCounter);
                PocetKoncovychZavorek--;
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
                                {
                                    printf("LABEL $endiflabel%d\n",ELSECounter);
                                    PocetKoncovychZavorek--; ELSECounter--;
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
    (*token)->data="ERR_SYNTAX";
    printf("Chybne v ifu (konec)\n");
    return *token;
}

tToken func_rule(tToken *token)
{   PossibleEof=false;
    if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0))
    { printf("LABEL $$main\n");
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
                    {   PocetKoncovychZavorek++; VMainu=true;
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
                            {   PossibleEof=true; PocetKoncovychZavorek--; VMainu=false;
                                (*token)=(*token)->nextToken; //ted se ceka eof
                                (*token)=body(token);
                                return *token;
                            }
                            else {(*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; return *token; }
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
            else {(*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; return *token; }

        } //pokud jiz byl main a je znovu main
        else
        {
            (*token)->type=T_UNKNOWN;
            (*token)->data="ERR_SYNTAX";
            printf("main again");
            return *token;
        }

    }
    else if ((*token)->type==T_ID) //Dalsi typy funkci
    {   if(STableSearch(GlobalTable,(*token)->data)!=NULL)
        {
        printf("Fce jiz byla jednou definovana\n");
        }
        STableInsert(&GlobalTable,(*token)->data);

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
                GlobalTable->datastringnavratovehodnoty = malloc(sizeof(char)*16);
                if(GlobalTable->datastringparametry == NULL){
                    fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro dynamicstring\n");
                    exit(99);
                }

                paramscounter=0; (*token)=narvrattype_n(token);

                if ((*token)->type==T_UNKNOWN) //nastala chyba v navrattype_n a nebo je vraceno )
                {
                    return *token;
                }
                (*token)=(*token)->nextToken;
                if ((*token)->type==T_LCBR) // {
                {   PocetKoncovychZavorek++; VeFunkci=true;
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
                        {   PocetKoncovychZavorek--; VeFunkci=false;
                            (*token)=(*token)->nextToken;
                            (*token)=body(token);
                            return *token;
                        }
                        else
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                            printf("Chyba func_dole1\n");
                            return *token;
                        }

                    }
                    else
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                        printf("Chyba func_dole2\n");
                        return *token;
                    }

                }
                else
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                    printf("Chyba func_dole3\n");
                    return *token;
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
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                        printf("Chyba func_dole4\n");
                        return *token;
                    }
                }
                else
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                    printf("Chyba func_dole5\n");
                    return *token;
                }

            }
            else
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                //("Chyba func_dole6\n");
                return *token;
            }

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba func_dole7\n");
            return *token;
        }
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba func_dole8\n");
        return *token;
    }

}
//PRAVIDLO <navrattype_n> -> <DATATYPE> , <NAVRATTYPE_N>  nebo <navrattype_n> -> EPS
tToken narvrattype_n(tToken *token)
{
    if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
    {
        if((*token)->type==T_KEYINT) GlobalTable->datastringparametry[paramscounter]='i';
        else if((*token)->type==T_KEYFLOAT64) GlobalTable->datastringparametry[paramscounter]='f';
        else if((*token)->type==T_KEYSTRING) GlobalTable->datastringparametry[paramscounter]='s';
        GlobalTable->datastringparametry[paramscounter+1]='\0';
        paramscounter++;

        (*token)=(*token)->nextToken;
        if ((*token)->type==T_COMMA)
        {
            (*token)=(*token)->nextToken;
            (*token)=narvrattype_n(token);
            return *token;
        }
        else if ((*token)->type==T_RDBR)
        {
            printf("Pocet navratovych hodnot %d a jsou %s\n",paramscounter, GlobalTable->datastringparametry);
            return *token;

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba navrattype_n1\n");
            return *token;
        }
    }
    else if ((*token)->type==T_RDBR)
    {
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba navrattype_n2\n");
        return *token;
    }

}
//pravidla <PARAMS> -> ID <DATATYPE> <PARAMS_N> nebo <PARAMS> ->  EPS

tToken params(tToken *token)
{
    GlobalTable->datastringparametry = malloc(sizeof(char)*16);
    if(GlobalTable->datastringparametry == NULL){
        fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro dynamicstring\n");
        exit(99);
    }
    if ((*token)->type==T_ID)
    {
        (*token)=(*token)->nextToken;
        if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
        {
            if((*token)->type==T_KEYINT) GlobalTable->datastringparametry[paramscounter]='i';
            else if((*token)->type==T_KEYFLOAT64) GlobalTable->datastringparametry[paramscounter]='f';
            else if((*token)->type==T_KEYSTRING) GlobalTable->datastringparametry[paramscounter]='s';
            GlobalTable->datastringparametry[paramscounter+1]='\0';

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
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba params1\n");
            return *token;
        }
    }
    else if ((*token)->type==T_RDBR)
    {
        return *token; //vraci ) do func
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba params2\n");
        return *token;
    }

}
//Pravidlo <params_n> -> , ID  <DATATYPE> <PARAMS_N> nebo  <params_n> -> EPS
tToken params_n(tToken *token)
{ paramscounter++;
    if ((*token)->type==T_COMMA)
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ID)
        {   (*token)=(*token)->nextToken;
            if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
            {
                if((*token)->type==T_KEYINT) GlobalTable->datastringparametry[paramscounter]='i';
                else if((*token)->type==T_KEYFLOAT64) GlobalTable->datastringparametry[paramscounter]='f';
                else if((*token)->type==T_KEYSTRING) GlobalTable->datastringparametry[paramscounter]='s';
                GlobalTable->datastringparametry[paramscounter+1]='\0';

                (*token)=(*token)->nextToken;
                (*token)=params_n(token);
                return *token;
            }
            else
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
                printf("Chyba params_n1\n");
                return *token;
            }

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba params_n2\n");
            return *token;
        }

    }
    else  if ((*token)->type==T_RDBR) //)
    {   printf("ten string je %s a jeho delka je %d\n",GlobalTable->datastringparametry,strlen(GlobalTable->datastringparametry)); paramscounter=0;
        return *token; //ukonceni a vraceni )
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba params_n3\n");
        return *token;
    }


}

tToken for_rule(tToken *token) //TODO spravne cislovani for labelu (jako u ifu)
{ FORCounter++;
    PossibleEof=false;
    if ((*token)->type==T_ID)
    {  JmenoPromenne=(*token)->data;
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_DEFINE)
        {   CodeGenDefVar(JmenoPromenne);

            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token);

            if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba foru1\n");  return *token;
            }

            if ((*token)->type==T_UNKNOWN)
            {
                return *token;
            }
            //GENEROVNAI KODU prirazeni do promenne
            printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
            printf("MOVE %s TF@$return\n", JmenoPromenne);

        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
            printf("chyba for po ID\n");
            return *token;
        }
    }
    //GENEROVANI KODU FOR label (zacatek foru na ktery se jumpuje)
    printf("LABEL for%d\n",FORCounter);

    if ((*token)->type==T_SEMICOLON) //je strednik? bud je prvni nebo uspesne bylo ID=vyraz
    {
        (*token)=(*token)->nextToken; //ted povinny vyraz
        (*token)=exprBUParse(token);
        if(Porovnavani==false) //pokud by se ve vyrazu nevyskytlo porovnani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("chyba for vyraz\n"); return *token;
        }
        //GENEROVANI KODU podminka FOR
        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        printf("JUMPIFNEQ $forendlabel%d TF@$return bool@true\n",FORCounter);

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
                    if(Porovnavani==true) //pokud by se ve vyrazu vyskytlo porovnani //TODO testovat
                    {
                        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("chyba for prikaz_prirazeni\n");
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
                        //GENEROVANI KODU FOR KONEC
                        printf("JUMP for%d\n",FORCounter);
                        printf("LABEL $forendlabel%d\n",FORCounter);

                        PocetKoncovychZavorek--;
                        return *token;
                    }
                    else
                    {
                        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for1\n");
                        return *token;
                    }
                }
                else
                {
                    (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
                    printf("chyba for2_EOL\n");
                    return *token;
                }

            }
            else
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
                //("chyba for2\n");
                return *token;
            }
        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX"; printf("chyba for3\n");
            return *token;
        }

    }
    else
    {
        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
        printf("chyba for4\n");
        return *token;
    }

}
//pravidlo <ID_NEXT> -> := vyraz nebo <ID_NEXT> -> <ID_N>=<vice_id_vlevo>
tToken id_next(tToken *token)
{
    JmenoPromenne = (*token)->prevToken->data;
    if((*token)->type==T_DEFINE) // pokud je :=
    {
        if(STableSearchLocal(LocalTable,JmenoPromenne)!=NULL)
        {
            printf("definice podruhe!!!!!!!");
             //TODO ERRORIK
        }


        CodeGenDefVar(JmenoPromenne);
        (*token)=(*token)->nextToken;


        (*token)=exprBUParse(token); //do tokenu bud T_UNKNOWN nebo nasledujici znak
        if (exprTyp==X_INT)  {STableInsertLocal(&LocalTable,JmenoPromenne,'i',AktualniHloubkaZanoreni); printf("1qn\n");}//todo predelat hloubku zanoreni
        else if (exprTyp==X_STRING) { STableInsertLocal(&LocalTable,JmenoPromenne,'s',AktualniHloubkaZanoreni); printf("2qn\n");}
        else if (exprTyp==X_FLOAT) { STableInsertLocal(&LocalTable,JmenoPromenne,'f',AktualniHloubkaZanoreni); printf("3qn\n");}

        if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba (porovnavani tam kde nema byt \n");  return *token;
        }


        //GENEROVANI KODU TODO DO CODEGEN?
                printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                printf("MOVE %s TF@$return\n", JmenoPromenne);

        return *token;
    }
    else if ((*token)->type==T_COMMA) //pokud je carka
    {   UchovaniID[IDCounter+1]=(*token)->prevToken->data;
        (*token)=id_n(token); //volat <ID_N> (vice identifikatoru za sebou)

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_N
        {
            return *token;
        }
        if ((*token)->type==T_ASSIGN) //=
        {
            (*token)=(*token)->nextToken; IDCounterOpacny=0;
            (*token)=vice_id_vlevo(token);
            return *token; //return do body kde se vola
        }

        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba ID_NEXT1\n");
            return *token;
        }
    }
    else if ((*token)->type==T_ASSIGN) // JEN ID=
    {   IDCounter=1;
        UchovaniID[IDCounter]=(*token)->prevToken->data;
        (*token)=(*token)->nextToken; IDCounterOpacny=0;
        (*token)=vice_id_vlevo(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba ID_NEXT2\n");
        return *token;
    }
}
//pravidlo 15 a 16 <ID_N> -> , ID <ID_N> & <ID_N> -> EPS
tToken id_n(tToken *token)
{   IDCounter++;
    if((*token)->type==T_COMMA) //pri prvnim volani je to jiste, pak kontrola
    {
        (*token) = (*token)->nextToken;
        if ((*token)->type == T_ID) //ted musi byt id jinak chyba
        {   UchovaniID[IDCounter+1]=(*token)->data;
            (*token) = (*token)->nextToken;
            (*token) = id_n(token); //rekurzivni volani, kde se jiz kontroluje carka nebo =
            return *token;

        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba ID_N1\n");
            return *token;
        }

    }
    else if ((*token)->type==T_ASSIGN) //=
    {
        return *token;
    }
    else //neco jinyho nez carka nebo = automaticky error
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba ID_N2\n");
        return *token;
    }

}
//PRAVIDLA <VICE_ID_VLEVO> -> vyraz <vyraz_n> NEBO <VICE_ID_VLEVO> -> ID (<PARAMSCALL>
tToken vice_id_vlevo(tToken *token)
{IDCounterOpacny++;
    if (((*token)->type==T_ID) && ((*token)->nextToken->type==T_LDBR)) //volani fce
    {   if(STableSearch(GlobalTable,(*token)->data)==NULL)
        {
        printf("Volame fci co nebyla definovana"); //TODO insert
        }

        (*token)=(*token)->nextToken;  (*token)=(*token)->nextToken;
      paramscounter=0;  (*token)=paramscall(token); paramscounter=0; //vraceno ) nebo chyba z paramscall
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
        if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba (porovnavani tam kde nema byt \n");  return *token;
        }

        //GENEROVANI
                printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                printf("MOVE %s TF@$return\n", UchovaniID[IDCounterOpacny]);

        (*token)=vyraz_n(token);
        return *token;
    }

}

//PRAVIDLO <paramscall> -> ID <PARAMSCALL_N>
tToken paramscall(tToken *token)
{
    if ((*token)->type==T_ID||((*token)->type==T_INT)||((*token)->type==T_STRING)||((*token)->type==T_EXP))
    {
        //SEMANTIKA TODO ID
        if((*token)->type==T_ID)
        {
            if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
            {
                printf("promenna nebyla definovana"); (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
           printf("datovy typ definovany promenny %c\n",LocalTable->DatovyTyp);

        }
        if((*token)->type==T_INT)
        {
            if(GlobalTable->datastringparametry[paramscounter]!='i') //pokud je čislo, musí se na danem mistě ve funkci nachazet int
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
        }

        if((*token)->type==T_STRING)
        {
            if(GlobalTable->datastringparametry[paramscounter]!='s')
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
        }

        if((*token)->type==TYPE_FLOAT64)
        {
            if(GlobalTable->datastringparametry[paramscounter]!='f')
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
        }
        paramscounter++;
        //KONEC SEMANTIKY
        (*token)=(*token)->nextToken;
        (*token)=paramscall_n(token); //je vracenej bud error nebo ) a toto vracime do vice_id_vlevo
        return *token;
    }
    else  if ((*token)->type==T_RDBR) //je tam jen () - nějakej input()
    {
        if(strlen(GlobalTable->datastringparametry)!=paramscounter)
        {printf("nesedi pocet vstupnich parametru\n");
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;

        }
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
        printf("Chyba paramscall\n");
        return *token;
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
            //SEMANTIKA TODO ID
            if((*token)->type==T_INT)
            {
                if(GlobalTable->datastringparametry[paramscounter]!='i') //pokud je čislo, musí se na danem mistě ve funkci nachazet int
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                }
            }

            if((*token)->type==T_STRING)
            {
                if(GlobalTable->datastringparametry[paramscounter]!='s')
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                }
            }

            if((*token)->type==TYPE_FLOAT64)
            {
                if(GlobalTable->datastringparametry[paramscounter]!='f')
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                }
            }
            paramscounter++;
            //KONEC SEMANTIKY

            (*token)=(*token)->nextToken;
            (*token)=paramscall_n(token);
            return *token;
        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";
            printf("Chyba paramscall_n1\n");
            return *token;
        }

    }
    else if ((*token)->type==T_RDBR) //)
    {
        if(paramscounter!=strlen(GlobalTable->datastringparametry)) (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; //SEMANTIKA -> kontrola poctu
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("Chyba paramscall_n2\n");
        return *token;
    }

}
//PRAVIDLO <vyraz_n> -> , vyraz <vyraz n>
tToken vyraz_n(tToken *token){
    IDCounter--;
    if ((*token)->type==T_COMMA) //nastala chyba pri vyrazu
    {
        (*token)=(*token)->nextToken;
        (*token)=exprBUParse(token); //do token ulozeni buď posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)

        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }

        if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba (porovnavani tam kde nema byt vyraz_n \n");  return *token;
        }

        IDCounterOpacny++;
        //GENEROVANI KODU
                printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                printf("MOVE %s TF@$return\n", UchovaniID[IDCounterOpacny]);

                (*token)=vyraz_n(token);
         return *token;
    }
    else
    { //neni uz zadny dalsi vyraz a nasleduje neco z body (returny az uplne na zacatek kde bude body)
        if (JdemeZReturnu==false)
        {
            if(IDCounter!=0) //nebyl stejny pocet identifikatoru vlevo a vyrazu vpravo
            {  // printf("IDCOUNTER %d\n",IDCounter);
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("chyba-> ruzny pocet id a vyrazu  <vyraz_n>\n");
               return *token;
            }
        }
        else
        {
            return *token;
        }

        return *token;
    }
}
