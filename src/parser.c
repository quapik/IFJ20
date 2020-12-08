//Implementace pĹ™ekladaÄŤe imperativnĂ­ho jazyka IFJ20
//TĂ˝m ÄŤĂ­slo 041, varianta I
/*AutoĹ™i projektu:
Ĺ Ă­ma VojtÄ›ch 	xsimav01@stud.fit.vutbr.cz
FabiĂˇn Michal   xfabia13@stud.fit.vutbr.cz
ÄŚĂˇbela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informaÄŤnĂ­ch technologiĂ­ VUT v BrnÄ›
*/

#include "parser.h"

bool BylMain=false; //pomocna pro to jestli byla fce main
tToken pomToken;
bool PossibleEof=false; //aby nenastal eof v tele ifu apod
bool ViceVyrazu=false; bool Only1ID=false;
unsigned int PocetKoncovychZavorek=0; //promenna pro kontrolu zda je stejny pocet { & }
int IDCounter=0; unsigned  int IDCounterOpacny=0; unsigned FORCounter=0; unsigned FOR2Counter=0;
unsigned  int IFCounter=0;   unsigned int ELSECounter=0; //countery pro LABELY pro CODEGEN
char* UchovaniID[20];
bool VeFunkci=false; bool VMainu=false; // bool JdemeZReturnu = false;
char* JmenoPromenne;
unsigned int AktualniHloubkaZanoreni=0; unsigned int paramscounter=0;

tSymbolTablePtrPromenna LocalTable;
tSymbolTablePtr GlobalTable;

//pravidlo 1. <program>-><>package main<body> EOF
int StartParser(tToken *token)
{   STableInitLocal(&LocalTable); STableInsertLocal(&LocalTable,"_",'_',0); //vlozeni _ abychom s ni mohli pracovat
    STableInit(&GlobalTable);

    if(((*token)->type)==T_PACKAGE) //prvni musi byt package main
    {
        (*token)=(*token)->nextToken;
        if(((*token)->type==T_ID)&&(strcmp((*token)->data,"main")==0)) //id s main
        {   CodeGenStart();
            (*token)=(*token)->nextToken;

            if ((*token)->type == T_EOL || (*token)->type == T_FUNC) //po package main muĹľe byt jen eol nebo func
            {
                pomToken=body(token);

                STableDisposeLocal(LocalTable);
                STableDispose(GlobalTable);
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

        if ((*token)->type==T_UNKNOWN) //chyba ve func rule
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
    else if(((*token)->type==T_ID)&&(strcmp((*token)->data,"_")==0)&&(*token)->nextToken->type==T_ASSIGN) // _
    {
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ASSIGN) // =
        {
            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token,LocalTable);
            if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
            {
                return *token;
            }
            if(Porovnavani==true) //Vyskytlo se porovnavani na miste kde nemĂˇ bĂ˝t
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba v _ \n");  return *token;
            }
            printf("CLEARS\n"); //vycisteni stacku (zapomenuti hodnoty)
            (*token)=body(token);
            return *token;

        }

        else{  (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
            printf("Chybny oef\n");
        }

    }
    else if ((*token)->type==T_ID) //MĂˇme identifikĂˇtor (s hodnotou)
    {

        (*token)=(*token)->nextToken;
        if ((*token)->type==T_LDBR) //pokud mame volani funkce ale nikam neprirazujeme
        {
            if(STableSearch(GlobalTable,(*token)->prevToken->data)==NULL) //funkce nenalezena
            {
                STableInsert(&GlobalTable,(*token)->prevToken->data,false);

            }
            else //funkce nalezena
            {
                if(strlen(GlobalTable->datastringnavratovehodnoty)!=0) //pokud volana funkce ma nÄ›jake navratove hodnoty
                {   printf("test");
                    //printf ("%s\n",strlen(GlobalTable->datastringnavratovehodnoty));
                    (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET"; return *token;
                }
                (*token)=(*token)->nextToken;
                if(((*token)->type==T_RDBR)&&strlen(GlobalTable->datastringparametry)!=0)
                {
                    printf ("tu2\n"); (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET"; return *token;
                }
                paramscounter=0;
                while((*token)->type!=T_RDBR)
                {
                    if ((*token)->type==T_ID)
                    {
                        if(STableSearchLocal(LocalTable,(*token)->data)==NULL) {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;}
                        if(STableSearchLocalReturnType(LocalTable,(*token)->data)!=GlobalTable->datastringparametry[paramscounter])
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                        }


                    }
                    else if ((*token)->type==T_INT) {
                        if((GlobalTable->datastringparametry[paramscounter])!='i') {(*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;}
                    }
                    else if ((*token)->type==T_STRING) {
                        if((GlobalTable->datastringparametry[paramscounter])!='s') {(*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;}
                    }
                    else if (((*token)->type==T_EXP)||((*token)->type==T_DOUBLE)) {
                        if((GlobalTable->datastringparametry[paramscounter])!='f') { (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;}
                    }
                    else
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
                    }

                    (*token)=(*token)->nextToken;
                    if ((*token)->type==T_COMMA)
                    {
                        (*token)=(*token)->nextToken;
                    }
                    paramscounter++;
                }

                (*token)=(*token)->nextToken;

                (*token)=body(token);
                return *token;

            }

        }
        else
        {

            (*token)=id_next(token);
        }


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
        if(PocetKoncovychZavorek==0)
        {    //MAYBE TODO
            printf("moc zavorek }\n");  (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX";   return *token;
        }
        (*token)=(*token)->prevToken;
        return *token;
    }
        //pravidlo return -> return
    else if ((*token)->type==T_RETURN)
    {   if (VeFunkci==true)
        {   //SEMANTICKA KONTROLA
            if(strlen(GlobalTable->datastringnavratovehodnoty)==0) //prazdne navratove hodnoty a my chceme return
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET";  return *token;
            }
            (*token)=(*token)->nextToken;
            //TODO KONTROLA DATOVYCH TYPU
            ((*token)=exprBUParse(token,LocalTable)); //do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
            if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
            {
                return *token;
            }
            if(Porovnavani==true) //pokud ve vyrazu vyskytlo porovnavani
            {
                (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba v returnu \n");  return *token;
            }
            if (exprTyp==X_INT) { if(GlobalTable->datastringnavratovehodnoty[0]!='i') printf("chyba navrat"); }
            if (exprTyp==X_STRING) { if(GlobalTable->datastringnavratovehodnoty[0]!='s') printf("chyba navrat"); }
            if (exprTyp==X_FLOAT) { if(GlobalTable->datastringnavratovehodnoty[0]!='f') printf("chyba navrat"); }
            // JdemeZReturnu=true; //TODO SMAZAT
            // (*token)=vyraz_n(token);
            //JdemeZReturnu=false;
            int pom=1;
            while(pom<strlen(GlobalTable->datastringnavratovehodnoty)) //prochazeni v priapde ze je vice navratovych hodnot
            {
                if((*token)->type==T_COMMA)
                {
                    (*token)=(*token)->nextToken;
                    (*token)=exprBUParse(token,LocalTable); //do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
                    if ((*token)->type==T_UNKNOWN) //pokud nastala chyba pri vyrazu
                    {
                        return *token;
                    }
                    if(Porovnavani==true) //pokud ve vyrazu vyskytlo porovnavani
                    {
                        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba v returnu \n");  return *token;
                    }

                    if (exprTyp==X_INT) { if(GlobalTable->datastringnavratovehodnoty[pom]!='i') {(*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET";  return *token;}}
                    if (exprTyp==X_STRING) { if(GlobalTable->datastringnavratovehodnoty[pom]!='s'){ (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET";  return *token; }}
                    if (exprTyp==X_FLOAT) { if(GlobalTable->datastringnavratovehodnoty[pom]!='f'){ (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_POCET";  return *token; }}
                    printf("hodnota pom %d a delka co porovanvame %d\n",pom,strlen(GlobalTable->datastringnavratovehodnoty));
                    pom++;

                }
                else
                {
                    (*token)->type=T_UNKNOWN;   (*token)->data="ERR_SEM_POCET";  return *token;
                }

            }

            if ((*token)->type==T_COMMA) //Pokud bylo moc parametru
            {
                (*token)->type=T_UNKNOWN;   (*token)->data="ERR_SEM_POCET";  return *token;
            }
            //KONEC SEMANTIKY

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
    return *token;
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
{   PossibleEof=false; AktualniHloubkaZanoreni++;

    //TED MUSI BYT VYRAZ
    (*token)=exprBUParse(token,LocalTable); //do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
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
        printf("#---VETVENI IF---\n");
        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        printf("JUMPIFNEQ $elselabel%d TF@$return bool@true\n",IFCounter);
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
            {   printf("JUMP $endiflabel%d\n",ELSECounter);
                printf("#---VETVENI ELSE---\n");
                printf("LABEL $elselabel%d\n",ELSECounter);
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
                                    PocetKoncovychZavorek--; ELSECounter--; AktualniHloubkaZanoreni--;
                                    STableDisposeZanorene(&LocalTable,AktualniHloubkaZanoreni);
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
    { printf("LABEL $$main\n"); AktualniHloubkaZanoreni=0;
        printf("CREATEFRAME\nPUSHFRAME\n");
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
    {   AktualniHloubkaZanoreni++;
        //Abychom nemohli definovat vestavene funkce
        if((strcmp((*token)->data,"inputi")==0)||(strcmp((*token)->data,"inputf")==0)||(strcmp((*token)->data,"inputs")==0)||
                (strcmp((*token)->data,"len")==0)||(strcmp((*token)->data,"substr")==0)||(strcmp((*token)->data,"ord")==0)||
                (strcmp((*token)->data,"chr")==0)){(*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return* token;}

        if(STableSearch(GlobalTable,(*token)->data)!=NULL)
        {
            printf("Fce jiz byla jednou definovana\n");(*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return* token;
        }
        STableInsert(&GlobalTable,(*token)->data,true);

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

            GlobalTable->datastringnavratovehodnoty = malloc(sizeof(char)*16);
            if(GlobalTable->datastringparametry == NULL){
                fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro dynamicstring\n");
                exit(99);
            }

            GlobalTable->datastringnavratovehodnoty[0]='\0';
            if ((*token)->type==T_LDBR) // (
            {
                (*token)=(*token)->nextToken;
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
                        {   PocetKoncovychZavorek--; VeFunkci=false; AktualniHloubkaZanoreni--;
                            STableDisposeZanorene(&LocalTable,AktualniHloubkaZanoreni);
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
                VeFunkci=true;
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
                    {   PocetKoncovychZavorek--; AktualniHloubkaZanoreni--; VeFunkci=false;
                        STableDisposeZanorene(&LocalTable,AktualniHloubkaZanoreni);
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
        if((*token)->type==T_KEYINT) GlobalTable->datastringnavratovehodnoty[paramscounter]='i';
        else if((*token)->type==T_KEYFLOAT64) GlobalTable->datastringnavratovehodnoty[paramscounter]='f';
        else if((*token)->type==T_KEYSTRING) GlobalTable->datastringnavratovehodnoty[paramscounter]='s';
        GlobalTable->datastringnavratovehodnoty[paramscounter+1]='\0';
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
            printf("Pocet navratovych hodnot %d a jsou %s\n",strlen(GlobalTable->datastringnavratovehodnoty), GlobalTable->datastringnavratovehodnoty);
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
    {   JmenoPromenne=(*token)->data;
        //vlozeni vstupnich parametru funkce (idcke) do tabulky promennych
        if(STableSearchLocal(LocalTable,JmenoPromenne)!=NULL)
        {
            (*token)->type = T_UNKNOWN;  (*token)->data = "ERR_SEM_PROG"; return *token;
        }


        (*token)=(*token)->nextToken;
        if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
        {
            if((*token)->type==T_KEYINT){
                GlobalTable->datastringparametry[paramscounter]='i';
                STableInsertLocal(&LocalTable,JmenoPromenne, 'i', AktualniHloubkaZanoreni);
            }
            else if((*token)->type==T_KEYFLOAT64) {
                GlobalTable->datastringparametry[paramscounter]='f';
                STableInsertLocal(&LocalTable,JmenoPromenne, 'f', AktualniHloubkaZanoreni);
            }
            else if((*token)->type==T_KEYSTRING){
                GlobalTable->datastringparametry[paramscounter]='s';
                STableInsertLocal(&LocalTable,JmenoPromenne, 's', AktualniHloubkaZanoreni);
            }
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
        GlobalTable->datastringparametry[0]='\0';
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
{
    paramscounter++;
    if ((*token)->type==T_COMMA)
    { JmenoPromenne=((*token)->prevToken->data);
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_ID)
        {   (*token)=(*token)->nextToken;
            if (((*token)->type==T_KEYINT) || ((*token)->type==T_KEYFLOAT64) || ((*token)->type==T_KEYSTRING))
            {
                if((*token)->type==T_KEYINT){
                    GlobalTable->datastringparametry[paramscounter]='i';
                    STableInsertLocal(&LocalTable,JmenoPromenne, 'i', AktualniHloubkaZanoreni);
                }
                else if((*token)->type==T_KEYFLOAT64) {
                    GlobalTable->datastringparametry[paramscounter]='f';
                    STableInsertLocal(&LocalTable,JmenoPromenne, 'f', AktualniHloubkaZanoreni);
                }
                else if((*token)->type==T_KEYSTRING){
                    GlobalTable->datastringparametry[paramscounter]='s';
                    STableInsertLocal(&LocalTable,JmenoPromenne, 's', AktualniHloubkaZanoreni);
                }
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

tToken for_rule(tToken *token)
{ FORCounter++; AktualniHloubkaZanoreni++;
    PossibleEof=false;
    if ((*token)->type==T_ID)
    {  JmenoPromenne=(*token)->data;
        (*token)=(*token)->nextToken;
        if ((*token)->type==T_DEFINE)
        {
            if(STableSearchLocal(LocalTable,JmenoPromenne)!=NULL)
            {
                (*token)->type = T_UNKNOWN;
                (*token)->data = "ERR_SEM_PROG";
                return *token;
            }

            CodeGenDefVar(JmenoPromenne); //GENEROVANI PROMENNE a nasledne prirazeni hodnoty
            (*token)=(*token)->nextToken;
            (*token)=exprBUParse(token,LocalTable);
            //SEMANTICKA CAST - vlozeni jmena promenne, datoveho typu a hloubky zanoreni do LocalTable
            if (exprTyp==X_INT)  {STableInsertLocal(&LocalTable,JmenoPromenne,'i',AktualniHloubkaZanoreni);}
            else if (exprTyp==X_STRING) { STableInsertLocal(&LocalTable,JmenoPromenne,'s',AktualniHloubkaZanoreni); }
            else if (exprTyp==X_FLOAT) { STableInsertLocal(&LocalTable,JmenoPromenne,'f',AktualniHloubkaZanoreni); }

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
            printf("MOVE LF@%s TF@$return\n", JmenoPromenne);

        }
        else
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SYNTAX";
            printf("chyba for po ID\n");
            return *token;
        }
    }
    //GENEROVANI KODU FOR label (zacatek foru na ktery se jumpuje)
    printf("#---FOR---\n");
    printf("LABEL $for%d\n",FORCounter);

    if ((*token)->type==T_SEMICOLON) //je strednik? bud je prvni nebo uspesne bylo ID=vyraz
    {
        (*token)=(*token)->nextToken; //ted povinny vyraz
        (*token)=exprBUParse(token,LocalTable);
        if(Porovnavani==false) //pokud by se ve vyrazu nevyskytlo porovnani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("chyba for vyraz\n"); return *token;
        }
        //GENEROVANI KODU podminka FOR
        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        printf("JUMPIFNEQ $forendlabel%d TF@$return bool@true\n",FORCounter);
        printf("JUMP $forbody%d\n",FORCounter);
        printf("LABEL $forprirazeni%d\n",FORCounter);


        if ((*token)->type == T_UNKNOWN)
        {
            return *token;
        }

        if ((*token)->type==T_SEMICOLON) //povinny ;
        {
            (*token)=(*token)->nextToken; //bud id=vyraz nebo rovnou {
            if ((*token)->type==T_ID)
            {
                //SEMANTICKA CAST - kontrola promenna byla definovana
                if(STableSearchLocal(LocalTable,JmenoPromenne)==NULL)
                {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_PROG";
                    return *token;
                }

                (*token)=(*token)->nextToken;
                if ((*token)->type==T_ASSIGN)
                {
                    JmenoPromenne=(*token)->prevToken->data;
                    (*token)=(*token)->nextToken;
                    (*token)=exprBUParse(token,LocalTable);
                    if ((*token)->type == T_UNKNOWN)
                    {
                        return *token;
                    }
                    if(Porovnavani==true) //pokud by se ve vyrazu vyskytlo porovnani
                    {
                        (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("chyba for prikaz_prirazeni\n");
                        return *token;
                    }
                    printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                    printf("MOVE LF@%s TF@$return\n", JmenoPromenne);

                }
            }

            printf("JUMP $for%d\n",FORCounter);

            if ((*token)->type==T_LCBR)
            {    FOR2Counter=FORCounter;
                printf("LABEL $forbody%d\n",FORCounter);
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
                        printf("JUMP $forprirazeni%d\n",FOR2Counter);
                        printf("LABEL $forendlabel%d\n",FOR2Counter);
                        FOR2Counter--;
                        PocetKoncovychZavorek--; AktualniHloubkaZanoreni--;
                        STableDisposeZanorene(&LocalTable,AktualniHloubkaZanoreni);
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
            (*token)->type = T_UNKNOWN;
            (*token)->data = "ERR_SEM_PROG";
            return *token;
        }
        CodeGenDefVar(JmenoPromenne); //GENEROVANI NOVE PROMENNE

        (*token)=(*token)->nextToken;
        (*token)=exprBUParse(token,LocalTable);
        //SEMANTICKA CAST - vlozeni jmena promenne, datoveho typu a hloubky zanoreni do LocalTable
        if (exprTyp==X_INT)  {STableInsertLocal(&LocalTable,JmenoPromenne,'i',AktualniHloubkaZanoreni);}
        else if (exprTyp==X_STRING) { STableInsertLocal(&LocalTable,JmenoPromenne,'s',AktualniHloubkaZanoreni); }
        else if (exprTyp==X_FLOAT) { STableInsertLocal(&LocalTable,JmenoPromenne,'f',AktualniHloubkaZanoreni); }

        if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chybas (porovnavani tam kde nema byt \n");
            return *token;
        }

        //GENEROVANI KODU TODO DO CODEGEN?
        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        printf("MOVE LF@%s TF@$return\n", JmenoPromenne);

        return *token;
    }
    else if ((*token)->type==T_COMMA) //pokud mame za ID carku
    {   if(STableSearchLocal(LocalTable,JmenoPromenne)==NULL)
        {
            (*token)->type = T_UNKNOWN;  (*token)->data = "ERR_SEM_PROG";    return *token;
        }
        UchovaniID[IDCounter+1]=JmenoPromenne;


        (*token)=id_n(token); //volat <ID_N> (vice identifikatoru za sebou)

        if ((*token)->type==T_UNKNOWN) //nastala chyba v ID_N
        {
            return *token;
        }
        if ((*token)->type==T_ASSIGN)  //po vice identifikatorech prislo =
        {
            (*token)=(*token)->nextToken; IDCounterOpacny=0;

            (*token)=vice_id_vlevo(token);
            //SEMANTICKA CAST -> ulozeni hodnot z vyrazu do promenynych
            if(ViceVyrazu==true)
            {
                IDCounterOpacny--;
                while(IDCounterOpacny>0)
                {   if(strcmp(UchovaniID[IDCounterOpacny],"_")!=0)
                    { printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                        printf("MOVE LF@%s TF@$return\n", UchovaniID[IDCounterOpacny]);
                    }
                    else //potrebujeme promennou zapomenou - poze vytvoreni TF a popnuti, nasledne se premaze
                    {
                        printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
                    }

                    IDCounterOpacny--;
                }

                ViceVyrazu=false;
            }

            //KONEC SEMANTICKE CASTI
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
    {
        if(STableSearchLocal(LocalTable,(*token)->prevToken->data)==NULL) //pokud nebyla promenena nalezna v LocalTable
        {
            printf("Promenna nebyla definovana1\n");  (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;
        }
        IDCounter=1; Only1ID=true; //nastaaveni napevno aby nekolidovalo s vice id
        UchovaniID[IDCounter]=(*token)->prevToken->data;

        (*token)=(*token)->nextToken; IDCounterOpacny=0;
        (*token)=vice_id_vlevo(token); //do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
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
        (*token)=(*token)->nextToken;
        if ((*token)->type == T_ID) //ted musi byt id jinak chyba
        {
            if(STableSearchLocal(LocalTable,(*token)->data)==NULL) //pokud nebyla promenena nalezna v LocalTable
            {
                printf("Promenna nebyla definovana1\n");  (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG"; return *token;
            }

            UchovaniID[IDCounter+1]=(*token)->data;




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
tToken vice_id_vlevo(tToken *token) {
    IDCounterOpacny++;
    //VESTAVENA FUNKCE INPUTS
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "inputs") == 0)) {
        (*token)= CodeGenInputs(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }

    //VESTAVENA FUNKCE INPUTI
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "inputi") == 0)) {
        (*token)= CodeGenInputi(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }
    //VESTAVENA FUNKCE INPUTF
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "inputf") == 0))
    {
        (*token)= CodeGenInputf(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }

    //VESTAVENA FUNKCE len
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "len") == 0))
    {
        (*token)= CodeGenLen(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0;
        return *token;
    }
    //VESTAVENA FUNKCE substr

    if (((*token)->type == T_ID) && (strcmp((*token)->data, "substr") == 0))
    {
        (*token)= CodeGenSubstr(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }
    //VESTAVENA FUNKCE ord
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "ord") == 0))
    {
        (*token)= CodeGenOrd(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }
    //VESTAVENA FUNKCE chr
    if (((*token)->type == T_ID) && (strcmp((*token)->data, "chr") == 0))
    {
        (*token)= CodeGenChr(token,LocalTable,IDCounter,UchovaniID);
        IDCounter=0; return *token;
    }


    if (((*token)->type == T_ID) && ((*token)->nextToken->type == T_LDBR)) //volani fce
    {
        if (STableSearch(GlobalTable, (*token)->data) == NULL) {
            printf("Volame fci co nebyla definovana"); //TODO insert
        }

        (*token) = (*token)->nextToken;
        (*token) = (*token)->nextToken;
        paramscounter = 0;
        (*token) = paramscall(token);
        paramscounter = 0; //vraceno ) nebo chyba z paramscall
        return *token;
        //TODO SEMANTIKA IDÄŤka
    } else {
        ViceVyrazu = true;
        //pokud nevolame funkci, musi nasledovat vyraz

        (*token) = exprBUParse(token,LocalTable); //do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)
        if ((*token)->type == T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }
        //SEMANTICKA CAST
        if (Porovnavani == true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type = T_UNKNOWN;
            (*token)->data = "ERR_SEM_OSTATNI";
            printf("Chyba (porovnavani tam kde nema byt \n");
            return *token;
        }
        if(Only1ID==(true))
        {printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
            printf("MOVE LF@%s TF@$return\n", UchovaniID[IDCounterOpacny]);
            Only1ID=false;}
        //Kontrola zda sedi datove typy, ktere ted prirazujeme a ktere byly definovany
        if (strcmp(UchovaniID[IDCounterOpacny], "_") != 0) { //pokud _ tak netreba hlidat dat typy
            if (exprTyp == X_INT) {
                if (STableSearchLocalReturnType(LocalTable, UchovaniID[IDCounterOpacny]) != 'i') {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_KOMP";
                    return *token;
                }
            }
            if (exprTyp == X_STRING) {
                if (STableSearchLocalReturnType(LocalTable, UchovaniID[IDCounterOpacny]) != 's') {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_KOMP";
                    return *token;
                }
            }
            if (exprTyp == X_FLOAT) {
                if (STableSearchLocalReturnType(LocalTable, UchovaniID[IDCounterOpacny]) != 'f') {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_KOMP";
                    return *token;
                }
            }
        }
        //KONEC SEMANTICKE CASTI
        IDCounterOpacny++;
        (*token) = vyraz_n(token);
        return *token;
    }

}

//PRAVIDLO <paramscall> -> ID <PARAMSCALL_N>
tToken paramscall(tToken *token)
{
    if ((*token)->type==T_ID||((*token)->type==T_INT)||((*token)->type==T_STRING)||((*token)->type==T_EXP)||((*token)->type==T_DOUBLE))
    {
        //SEMANTIKA TODO ID
        if((*token)->type==T_ID)
        {
            if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
            {
                printf("promenna nebyla definovana"); (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
            printf("datovy typ definovany promenny %c\n",LocalTable->DatovyTyp);
            printf("paramscounter %d, datovy typ promenny %c, datovy typ ve fci %c\n", paramscounter,LocalTable->DatovyTyp,GlobalTable->datastringparametry[paramscounter] );
            if(GlobalTable->datastringparametry[paramscounter]!=LocalTable->DatovyTyp)
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }

        }
        if((*token)->type==T_INT)
        {
            if(GlobalTable->datastringparametry[paramscounter]!='i') //pokud je ÄŤislo, musĂ­ se na danem mistÄ› ve funkci nachazet int
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET";  return *token;
            }

        }

        if((*token)->type==T_STRING)
        {
            if(GlobalTable->datastringparametry[paramscounter]!='s')
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
        }

        if(((*token)->type==T_EXP)||((*token)->type==T_DOUBLE))
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
    else  if ((*token)->type==T_RDBR) //je tam jen () - nÄ›jakej input()
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
        if (((*token)->type==T_ID)||((*token)->type==T_INT)||((*token)->type==T_STRING)||((*token)->type==T_EXP)||((*token)->type==T_DOUBLE))
        {
            //SEMANTIKA TODO ID
            if((*token)->type==T_INT)
            {
                if(GlobalTable->datastringparametry[paramscounter]!='i') //pokud je ÄŤislo, musĂ­ se na danem mistÄ› ve funkci nachazet int
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

            if(((*token)->type==T_EXP)||(*token)->type==T_DOUBLE)
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
    {   printf("%d paramscouter, delkaass stringu %s\n",paramscounter,GlobalTable->datastringparametry );
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
        (*token)=exprBUParse(token,LocalTable);//do token ulozeni buÄŹ posledni token vyrazu (vse ok) nebo v token type T_UNKNOWN (pri chybe)

        if ((*token)->type==T_UNKNOWN) //nastala chyba pri vyrazu
        {
            return *token;
        }
        //SEMANTICKA CAST
        if(Porovnavani==true) //kontrola zda nebylo ve vyrazu porovnavani
        {
            (*token)->type=T_UNKNOWN;  (*token)->data="ERR_SEM_OSTATNI"; printf("Chyba (porovnavani tam kde nema byt vyraz_n \n");  return *token;
        }

        //Kontrola zda sedi datove typy, ktere ted prirazujeme a ktere byly definovany
        if(strcmp(UchovaniID[IDCounterOpacny],"_")!=0){ //pokud _ tak netreba hlidat dat typy
            if (exprTyp==X_INT) {if(STableSearchLocalReturnType(LocalTable,UchovaniID[IDCounterOpacny])!='i') {  (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_KOMP"; return *token;}}
            if (exprTyp==X_STRING) {if(STableSearchLocalReturnType(LocalTable,UchovaniID[IDCounterOpacny])!='s') { (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_KOMP"; return *token;}}
            if (exprTyp==X_FLOAT) {if(STableSearchLocalReturnType(LocalTable,UchovaniID[IDCounterOpacny])!='f') { (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_KOMP"; return *token;}}
        }
        //KONEC SEMANTICKE CASTI


        //GENEROVANI KODU
        // printf("CREATEFRAME\nDEFVAR TF@$return\nPOPS TF@$return\n");
        // printf("MOVE LF@%s TF@$return\n", UchovaniID[IDCounterOpacny]);
        IDCounterOpacny++;
        (*token)=vyraz_n(token); //rekurzivni volani teto funkce
        return *token;
    }
    else
    { //neni uz zadny dalsi vyraz a nasleduje neco z body (returny az uplne na zacatek kde bude body)
        /*if (JdemeZReturnu==false)
        {
            if(IDCounter!=0) //nebyl stejny pocet identifikatoru vlevo a vyrazu vpravo
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; printf("chyba-> ruzny pocet id a vyrazu  <vyraz_n>\n");
                return *token;
            }
        }
        else
        {
            return *token;
        }
        */
        //TODO SMAZAT
        if(IDCounter>0)
        {(*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_PROG";return *token;  }
        return *token;
    }
}
