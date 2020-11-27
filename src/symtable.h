//Implementace pøekladaèe imperativního jazyka IFJ20
//Tým èíslo 041, varianta I
/*Autoøi projektu:
Šíma Vojtìch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Èábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informaèních technologií VUT v Brnì
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef enum{
	VARIABLE=1,
	FUNCTION=2
}SymbolDataType;

typedef enum{
	TYPE_INT=1,
	TYPE_FLOAT64=2,
	TYPE_STRING=3
}SymbolType;

typedef union Value{
	int i;									// pro hodnotu integer
	double f;								// pro hodnotu float64
	char *s;								// pro obsah stringu
}SymbolValue;


typedef struct tSymbolData{
	SymbolType Type;						// typ - promenna/funkce
	SymbolDataType DataType;				// datovy/navratovy typ
	
	// data pro promennou
	SymbolValue Value;						// hodnota promenne
	
	// data pro funkci
	bool Defined;							// zda byla fce definovana
	struct tSymbolTable *LocalFuncData;		// pomocna tabulka pro lokalni funkce a jejich promenne
	
}*tSymbolDataPtr;

typedef struct tSymbolTable{
	char *Symbol;							// nazev identifikatoru
	struct tSymbolData *Data;				// data promenne/funkce predavane strukturou dat
	struct tSymbolTable *LPtr;				// prvek na globalni urovni s mensim klicem
	struct tSymbolTable *RPtr;				// prvek na globalni urovni s vetsim klicem
}*tSymbolTablePtr;


void STableInit(tSymbolTable *Tab);
tSymbolDataPtr STableSearch(tSymbolTable Tab, char Symbol);
void STableInsert(tSymbolTablePtr *Tab, char *Symbol, tSymbolDataPtr Data);
void ReplaceByRightmost(tSymbolTablePtr PtrReplaced, tSymbolTablePtr *Tab);
void STableDelete(tSymbolTablePtr *Tab, char Symbol);
void STableDispose(tSymbolTable *Tab);
