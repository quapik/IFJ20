//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně

Precedencni analyza zdola nahoru pro vyrazy
*/

#pragma once
#include "scanner.h"
#include "parser.h"
#include "codegen.h"
#include "symtable.h"


#define EXPR_STACK_ALLOC_SIZE 64


// typy neterminalu

typedef enum {
    X_INT,
    X_FLOAT,
    X_STRING,
    X_UNKNOWN
    //mozna pridat nil
} xNTermType;


// operátory z tabulky pro precedenční analýzu

typedef enum{
    X_MULDIV,	    // Násobení / dělení
    X_ADDSUB,	    // Sčítaní / odčítání
    X_LEQGEQ,		// Porovnání větší / menší / rovná se
    X_EQL,		    // Porovnání (ne)rovná se
    X_LDBR,		    // Levá (default) závorka
    X_RDBR,		    // Pravá (default) závorka
    X_N,		    // Proměnná
    X_$			    // Jiné
} xOperator;



// položky precedenční tabulky

typedef enum{
    X_OPEN,		// (<)
    X_CLOSE,	// (>)
    X_EQUAL,	// (=)
    X_EMPTY		// ( )
} xPriority;


// typ prvku na zasobniku

typedef enum{
    XT_OPEN,	// shift (<)
    XT_TERM,	// terminál
    XT_NONTERM,	// neterminál
} xItemType;

// unie dat prvku na zasobniku

typedef union{
    tToken token; // token / terminal
    xNTermType ntype; // typ neterminalu
} xItemData;


// prvek na zasobniku

typedef struct xItem{
    xItemType type;
    xItemData data;
} *txItem;


// zásobník pro precedenční analýzu

typedef struct xStack{
    int size;	// velikost zásobníku
    int top;	// index nejvyššího prvku
    txItem *xs;	// pole prvků
} *txStack;

xNTermType exprTyp;


// funkce
/**
 * Podle tabulky poprovná operátor v řádku s operátorem ve sloupci a vrátí odpovídající relaci (neterminál)
 *
 * @param currOperator Současný operátor na vrcholu zásobníku
 * @param newTerm Nový terminál, zjištěný ve funkci exprBUParse
 * @return xPriority Odpovídající relace (neterminál)
 */
xPriority exprBUGetPriority (xOperator currOperator, xOperator nextOperator);
/**
 * Hlavní funkce pro precedenční analýzu. Pracuje s posloupností tokenů.
 * Volají se zde funkce z celého souboru, např. i inicializace zásobníku.
 * Zjišťují se zde terminály a neterminály na vrcholu zásobníku a s nimi jsou volány další funkce.
 * Obsahuje switch, který volá další funkce v případě neterminálů "<" a ">" a sama se vypořádá s neterminály "=" a " ".
 * Po ukončení ukazatel na další token ukazuje za výraz.
 *
 * @param token Ukazatel na první token v seznamu, který je součástí výrazu
 * @param table Tabulka lokálních poměnných
 * @return tToken T_UNKNOWN s obsahem chybového kódu v případě chyby, jinak další token za výrazem
 */
tToken exprBUParse (tToken *token, tSymbolTablePtrPromenna table);
/**
 * Zde je inicializován zásobník
 *
 * @param *stack Ukazatel na zásobník
 */
void exprBUStackInit (txStack *stack);
/**
 * Převedení typu tokenu na typ terminálu dle prec. tabulky
 *
 * @param tokenType Typ tokenu
 * @return xOperator Typ operátoru nebo operandu (terminálu) ve výrazu
 */
xOperator exprTokenTypeToOperator(tType tokenType);
/**
 * Push, vložení dat na vrchol zásobníku
 * V případě nedostatku místa pro novou datovou položku se zde paměť zásobníku realokuje
 *
 * @param stack Zásobník
 * @param item Datová položka
 */
void exprBUStackPush (txStack stack, txItem item);
/**
 * Vyjme položku na vrcholu zásobníku a vrátí ji do položky
 *
 * @param stack Zásobník
 * @return txItem Prvek na vrcholu zásobníku, vrací se NULL pokud je zásobník prázdný
 */
txItem exprBUStackPop(txStack stack);
/**
 * Vloží otevírací "<" operátor pod zpracovaný neterminál a prvky nad ním posune
 *
 * @param stack Zásobník
 * @param posun Aktuální posun prvku, se kterým pracujeme
 */
void exprBUStackOpen (txStack stack, int posun); // < shift
/**
 * 1. Získá typ výrazu odděleného závorkami nebo pravidlem a řeší tento výraz
 * 2. Kontroluje sémantiku, pokud jsou ve výrazu dříve (ne)definované proměnné
 * 3. Generuje mezikód ifjcode20 pro řešení výrazů
 * 4. Generuje mezikód ifjcode20 pro tisknutí chyby při dělení nulou
 *
 * @param stack Zásobník
 * @param idTable Lokální tabulka proměnných
 * @return unsigned Hodnota pro vyplnění obsahu chybového kódu pro token. Hodnota dle zadání.
 */
unsigned exprBUStackClose(txStack stack, tSymbolTablePtrPromenna table); // > parse
/**
 * Uvolnení obsahu zásobníku a paměti
 *
 * @param *stack Ukazatel na zásobník
 */
void exprBUStackDispose(txStack *stack);
/**
 * Vytiskne kód pro kontrolu běhové chyby dělení nulou
 *
 * @param zero Text typu "int" nebo "float" pro změnu instrukce dělení
 */
void exprBUDivZeroCheck(char *zero); // kontrola deleni nulou, prebira typ 0 (float/int)







