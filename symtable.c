//tabulka symbolu
#include "symtable.h"
//BST(BinarySearchTree - viz IAL projekt2, c401)
//Inicializace stromu
void BSTInit (tBSTNodePtr *RootPtr) {

*(RootPtr)=NULL;
}
//Vyhledavani v stromu
int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{

if(RootPtr==NULL)
	{
		return(FALSE);
	}
else
	{
		if(RootPtr->Key==K)
			{
				(*Content)=RootPtr->BSTNodeCont;
				return(TRUE); //pokud je rootKey roven tak koncime a dal nehledame
			}
		else
			{
				if(RootPtr->Key>K)
					{
						return BSTSearch(RootPtr->LPtr,K,Content); //rekurzivnie prohledavani levyho podstromu
					}
					else
						{
						return BSTSearch(RootPtr->RPtr,K,Content);	//rekurzivnie prohledavani pravyho podstromu	
						}
			}

	}	

}
//Vlozeni do stromu
void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
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
			polozka->BSTNodeCont=Content;
			polozka->RPtr=NULL; //konečný list
			polozka->LPtr=NULL;
			*(RootPtr)=polozka;
		} 

	}	
	else
	{
		if((*RootPtr)->Key>K)
		{
			BSTInsert(&((*RootPtr)->LPtr),K,Content); //doleva a rekuzivne jedeme dal
		}
		else
		{
			if((*RootPtr)->Key<K)
			{
				BSTInsert(&((*RootPtr)->RPtr),K,Content); //doprava
			}

			else
			{
				(*RootPtr)->BSTNodeCont=Content; //pokud jiz existuje tak je nahrazen
			}
			
		}
	}
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
		PtrReplaced->BSTNodeCont=(*RootPtr)->BSTNodeCont;
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
void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
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
