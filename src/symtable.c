//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Symtable
*/

#include "symtable.h"

void STableInit(tSymbolTablePtr *Tab){
    *Tab=NULL;

}
void STableInitLocal(tSymbolTablePtrPromenna *Tab){
    *Tab=NULL;

}

tSymbolTablePtr STableSearch(tSymbolTablePtr Tab, char *Symbol){
    if(Tab==NULL){
        return NULL;
    }
    else{
        if((strcmp(Symbol,Tab->Symbol))==0){
            return Tab;
        }
        else{
            if((strcmp(Symbol,Tab->Symbol))<0){
                return (STableSearch(Tab->LPtr,Symbol));
            }
            else{
                return (STableSearch(Tab->RPtr,Symbol));
            }
        }
    }
}

tSymbolTablePtrPromenna STableSearchLocal(tSymbolTablePtrPromenna Tab, char *Symbol){
    if(Tab==NULL){
        return NULL;
    }
    else{
        if((strcmp(Symbol,Tab->Symbol))==0){
            return Tab;
        }
        else{
            if((strcmp(Symbol,Tab->Symbol))<0){
                return (STableSearchLocal(Tab->LPtr,Symbol));
            }
            else{
                return (STableSearchLocal(Tab->RPtr,Symbol));
            }
        }
    }
}

char STableSearchLocalReturnType(tSymbolTablePtrPromenna Tab, char *Symbol){
        if((strcmp(Symbol,Tab->Symbol))==0){
            return Tab->DatovyTyp;
        }
        else{
            if((strcmp(Symbol,Tab->Symbol))<0){
                Tab->DatovyTyp=STableSearchLocalReturnType(Tab->LPtr,Symbol);
                return Tab->DatovyTyp;
            }
            else{
                Tab->DatovyTyp=STableSearchLocalReturnType(Tab->RPtr,Symbol);
                return Tab->DatovyTyp;
            }
        }
}
void STableInsert(tSymbolTablePtr *Tab, char *Symbol, bool defined){
    tSymbolTablePtr polozka;
    if(*Tab==NULL){
        polozka=(tSymbolTablePtr) malloc(sizeof(struct tSymbolTable));
        if(polozka==NULL){
            fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro symtable\n");
            exit(99);
        }
        else{

            polozka->Symbol=Symbol;
            polozka->defined=defined;
            polozka->LPtr=NULL;
            polozka->RPtr=NULL;
            *(Tab)=polozka;
        }
    }
    else{
        if((strcmp(Symbol,(*Tab)->Symbol))<0){
            STableInsert(&((*Tab)->LPtr),Symbol,defined);
        }
        else if((strcmp(Symbol,(*Tab)->Symbol))>0){
            STableInsert(&((*Tab)->RPtr),Symbol,defined);
        }

    }
}

void STableInsertLocal(tSymbolTablePtrPromenna *Tab, char *Symbol,char data, unsigned int hloubkazanoreni){
    tSymbolTablePtrPromenna polozka;
    if(*Tab==NULL){
        polozka=(tSymbolTablePtrPromenna) malloc(sizeof(struct tSymbolTablePromenna));
        if(polozka==NULL){

                fprintf(stderr, "[INTERNAL] Fatal error - nelze alokovat pamet pro symtable\n");
                exit(99);

        }
        else{

            polozka->Symbol=Symbol;
            polozka->DatovyTyp=data;
            polozka->HloubkaZanoreni=hloubkazanoreni;
            polozka->LPtr=NULL;
            polozka->RPtr=NULL;
            *(Tab)=polozka;
        }
    }
    else{
        if((strcmp(Symbol,(*Tab)->Symbol))<0){
            STableInsertLocal(&((*Tab)->LPtr),Symbol,data,hloubkazanoreni);
        }
        else if((strcmp(Symbol,(*Tab)->Symbol))>0){
            STableInsertLocal(&((*Tab)->RPtr),Symbol,data,hloubkazanoreni);
        }

    }
}

void ReplaceByRightmost(tSymbolTablePtr PtrReplaced, tSymbolTablePtr *Tab){
    if(*Tab==NULL){
        return;
    }
    else{
        if((*Tab)->RPtr==NULL){
            PtrReplaced->Symbol=(*Tab)->Symbol;
            //PtrReplaced->Data=(*Tab)->Data;
            STableDelete(Tab,(*Tab)->Symbol);
        }
        else{
            ReplaceByRightmost(PtrReplaced,&((*Tab)->RPtr));
        }
    }
}

void STableDelete(tSymbolTablePtr *Tab, char *Symbol){
    if(*Tab==NULL){
        return;
    }
    else{
        if((*Tab)->Symbol>Symbol){
            STableDelete(&((*Tab)->LPtr),Symbol);
        }
        else{
            if((*Tab)->Symbol<Symbol){
                STableDelete(&((*Tab)->RPtr),Symbol);
            }
            else{
                if((*Tab)->LPtr==NULL && (*Tab)->RPtr==NULL){
                    free(*Tab);
                    *Tab=NULL;
                }
                else{
                    if((*Tab)->LPtr!=NULL && (*Tab)->RPtr!=NULL){
                        ReplaceByRightmost(*Tab, &((*Tab)->LPtr));
                    }
                    else{
                        if((*Tab)->LPtr!=NULL && (*Tab)->RPtr==NULL){
                            tSymbolTablePtr pomLEFT=((*Tab)->LPtr);
                            free(*Tab);
                            *Tab=pomLEFT;
                        }
                        else{
                            tSymbolTablePtr pomRIGHT=((*Tab)->RPtr);
                            free(*Tab);
                            *Tab=pomRIGHT;
                        }
                    }
                }
            }
        }
    }
}

void STableDispose(tSymbolTablePtr Tab){
    if(Tab!=NULL){
        // pruchod celym stromem
        STableDispose((Tab)->LPtr);
        STableDispose((Tab)->RPtr);

        // uvolneni pameti

        free(&Tab->datastringparametry);
        free(&Tab->datastringnavratovehodnoty);//TODO
        free(&Tab);

        Tab=NULL;
    }
}

void STableDisposeLocal(tSymbolTablePtrPromenna Tab){
    if(Tab!=NULL){
        // pruchod celym stromem
        STableDisposeLocal(Tab->LPtr);
        STableDisposeLocal(Tab->RPtr);

        // uvolneni pameti

        free(Tab);
        Tab=NULL;
    }
}

void STableDisposeZanorene(tSymbolTablePtrPromenna *Tab, unsigned int hloubkazanoreni){

    if(*Tab!=NULL)
    {
        if((*Tab)->HloubkaZanoreni>hloubkazanoreni) //pokud aktualni polozka ma vetsi hloubku zanoreni, byla zanorena a je treba smazat
        {
             STableDisposeLocal(*Tab); *Tab=NULL;
        }
        else if ((*Tab)->LPtr!=NULL) //pokud mame levy podstrom
        {    (*Tab)=(*Tab)->LPtr;
            if((*Tab)->HloubkaZanoreni>hloubkazanoreni)
            {
                 STableDisposeLocal(*Tab); *Tab=NULL; //smazani lptr a jeho podstromu
            }
            else
            {
                STableDisposeZanorene(&(*Tab)->LPtr,hloubkazanoreni); //rekiurizivne dal prochazime fci a hledame
            }
        }

        else if ((*Tab)->RPtr!=NULL) //pokud mame pravy podstrom
        {   (*Tab)=(*Tab)->RPtr;
            if((*Tab)->HloubkaZanoreni>hloubkazanoreni)
            {
                STableDisposeLocal(*Tab); *Tab=NULL; //smazani lptr a jeho podstromu
            }
            else
            {
                STableDisposeZanorene(&(*Tab)->RPtr,hloubkazanoreni);
            }
        }

    }

}
