//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
*/

//tabulka symbolu
#include "symtable.h"
#include <stdlib.h>
#include <stdio.h>


//BST(BinarySearchTree - viz IAL projekt2, c401)


//Inicializace stromu
void BSTInit (tBSTNodePtr *RootPtr) {

*(RootPtr)=NULL;
}

//inicializace tabulky
void SymtableInit(tSymtable* Symtable){
BSTInit(&(Symtable->RootPtr));
}
	

//Vyhledavani v stromu
tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char* K)	{

if(RootPtr==NULL)
    {
        return (0);
    }
else
	{
		if(RootPtr->Key==K)
			{
				return RootPtr; //vraceni aktualniho prvku
			}
		else
			{
				if(RootPtr->Key>K)
					{
						return BSTSearch(RootPtr->LPtr,K); //rekurzivnie prohledavani levyho podstromu
					}
					else
						{
						return BSTSearch(RootPtr->RPtr,K);	//rekurzivnie prohledavani pravyho podstromu
						}
			}

	}	

}

//vyhledavani v tabulce
tBSTNodePtr SymtableSearch(tSymtable* Symtable, char* K){
BSTSearch(&(Symtable->RootPtr),K); //TODO ERROR (string here, char v BST)
}


//Vlozeni do stromu
void BSTInsert (tBSTNodePtr* RootPtr, char* K, tTypUlozenychHodnot PromennaOrFunkce )	{
	tBSTNodePtr polozka;
	if((*RootPtr)==NULL) //pokud je prazdno
	{
		polozka=(tBSTNodePtr) malloc(sizeof(struct tBSTNode));
		if(polozka==NULL) //malloc se nepovedl
		{
			return;
		}
		else
		{
			polozka->Key=K;
			//DATA TODO polozka->BSTNodeCont=Content;
			polozka->PromennaORFunkce=PromennaOrFunkce;
			polozka->RPtr=NULL; //konečný list
			polozka->LPtr=NULL;
			*(RootPtr)=polozka;
		} 

	}	
	else
	{
		if((*RootPtr)->Key>K)
		{
			BSTInsert(&((*RootPtr)->LPtr),K,PromennaOrFunkce); //doleva a rekuzivne jedeme dal
		}
		else
		{
			if((*RootPtr)->Key<K)
			{
				BSTInsert(&((*RootPtr)->RPtr),K,PromennaOrFunkce); //doprava
			}

			else
			{
				 // TODO aktulaizace dat
			}
			
		}
	}
}
void SymtableInsertLocal(tSymtable* Symtable, char* K)
{

BSTInsert(&(Symtable->RootPtr),K,TypUlozenychHodnotPromenna);

}

void SymtableInsertGlobal(tSymtable* Symtable, char* K)
{
    //TODO ukladani vice informaci o funkci
    BSTInsert(&(Symtable->RootPtr),K,TypUlozenychHodnotFunkce);

}
//Nahrazeni nejpravejsim
void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr){
if((*RootPtr)==NULL)
	{
		return;
	}
	else
	{
	tBSTNodePtr polozka;
	if((*RootPtr)->RPtr==NULL) //dalsi pravy neexistuje
	{
			//presun nejpravejsi polozky do PtrReplaced
		PtrReplaced->Key=(*RootPtr)->Key;
		//TODO DATA PtrReplaced->BSTNodeCont=(*RootPtr)->BSTNodeCont;
		polozka=(*RootPtr);	
		(*RootPtr)=(*RootPtr)->LPtr; //posun rootu na levy 
		free(polozka); 
	}
	else //stale musime rekurzivne hledat pravej
	{
		ReplaceByRightmost(PtrReplaced,&(*RootPtr)->RPtr);
	}

	}
}
//Odstraneni uzlu podle klice
void BSTDelete (tBSTNodePtr *RootPtr, char* K) 		{
    if((*RootPtr)==NULL)
	{
			return;
	}
	else
	{
		tBSTNodePtr polozka;
		if((*RootPtr)->Key>K) //ruseny v levem podstromu
		{
			 BSTDelete(&(*RootPtr)->LPtr,K); //rekurzivni volani deletu (dal se hleda K)
		}

		else

		{
			if((*RootPtr)->Key<K) //ruseny v pravme podstrmu
			{
				BSTDelete(&(*RootPtr)->RPtr,K);
			}


			else 
			{
				if((*RootPtr)->Key==K) //nalezen uzel s danym klicem
				{
					if((*RootPtr)->RPtr!=NULL&&((*RootPtr)->LPtr!=NULL)) //ruseny ma oba podstromy
					{
						ReplaceByRightmost((*RootPtr),&(*RootPtr)->LPtr); //nahrada nejpravejsim
					}

					else 
					{	if((*RootPtr)->RPtr==NULL) //nema pravy podstrom (ale ma levy)
						{
							polozka=(*RootPtr);
							(*RootPtr)=(*RootPtr)->LPtr; //premosteni rootu na leveho syna
							free(polozka);

						}
						else
						{
							if (((*RootPtr)->LPtr==NULL)) //nema levy podstrom
							{
								polozka=(*RootPtr);
								(*RootPtr)=(*RootPtr)->RPtr; //premosteni rootu na praveho syna
								free(polozka);

							}
						}
					}
				}		
			}

		}
	
	}
}
//Odstraneni prvku symtable
void SymtableDelete(tSymtable* Symtable,char* K){
BSTDelete(&(Symtable->RootPtr),K); //TODO ERROR (string here, char v BST)
}

//Zruseni stromu
void BSTDispose (tBSTNodePtr *RootPtr) {
 
	 if((*RootPtr)==NULL)
	 	{
			 return; //ukonceni rekurze
	 	}	
	else
		{

			BSTDispose(&(*RootPtr)->LPtr); //rekuze, porad vice do hloubky stromu
			BSTDispose(&(*RootPtr)->RPtr);
			free(*RootPtr);
			(*RootPtr)=NULL; //uvolneni rootu(korene stromu) a navraceni do puvodniho stavu
		}		 

} 
//Zruseni symtadble
void SymtableDispose(tSymtable* Symtable){
BSTDispose(&(Symtable->RootPtr));
}





