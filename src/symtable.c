//Implementace pøekladaèe imperativního jazyka IFJ20
//Tým èíslo 041, varianta I
/*Autoøi projektu:
Šíma Vojtìch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Èábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informaèních technologií VUT v Brnì
*/

#include "symtable.h"

void STableInit(tSymbolTablePtr *Tab){
	*Tab=NULL;
}

tSymbolDataPtr STableSearch(tSymbolTablePtr Tab, char Symbol){
	if(Tab==NULL){
		return NULL;
	}
	else{
		if(Tab->Symbol==Symbol){
			return Tab->Data;
		}
		else{
			if(Tab->Symbol>Symbol){
				return (STableSearch(Tab->LPtr,Symbol));
			}
			else{
				return (STableSearch(Tab->RPtr,Symbol));
			}
		}
	}
}

void STableInsert(tSymbolTablePtr *Tab, char *Symbol, tSymbolDataPtr Data){
	if(*Tab==NULL){
		*Tab=malloc(sizeof(struct tSymbolTable));
		if(*Tab==NULL){
			return;
		}
		else{
			(*Tab)->Symbol=Symbol;
			(*Tab)->Data=Data;
			(*Tab)->LPtr=NULL;
			(*Tab)->RPtr=NULL;
		}
	}
	else{
		if(Symbol<(*Tab)->Symbol){
			STableInsert(&((*Tab)->LPtr),&Symbol,Data);
		}
		else if(Symbol>(*Tab)->Symbol){
			STableInsert(&((*Tab)->RPtr),&Symbol,Data);
		}
		else{
			free((*Tab)->Data);
			(*Tab)->Data=Data;
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
			PtrReplaced->Data=(*Tab)->Data;
			BSTDelete(Tab,(*Tab)->Symbol);
		}
		else{
			ReplaceByRightmost(PtrReplaced,&((*Tab)->RPtr));
		}
	}
}

void STableDelete(tSymbolTablePtr *Tab, char Symbol){
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

void STableDispose(tSymbolTable *Tab){
	if(*Tab!=NULL){
		// pruchod celym stromem
		STableDispose(&((*Tab)->LPtr));
		STableDispose(&((*Tab)->RPtr));
		
		// pokud jsem narazil na funkci, respektive pomocny podstrom
		if((*Tab)->Data->LocalFuncData!=NULL){
			STableDispose(&((*Tab)->Data->LocalFuncData));
		}
		
		// uvolneni pameti
		free((*Tab)->Data);
		free(*Tab);
		*Tab=NULL;
	}
}
