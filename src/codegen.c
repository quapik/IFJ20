//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Generování kódu, na souboru pracovali Šíma Vojtěch, Poposki Vasil 
*/

#include "codegen.h"


void CodeGenStart()
{
    printf(".IFJcode20\n");
    printf("JUMP $$main\n");
    gen_substr();
    gen_input();
    gen_string_functions();
}
void CodeGenDefVar(char* id)
{
    printf("DEFVAR LF@%s\n",id);
}

char *gen_float(char *input_float)
{
    double ret = strtod(input_float, NULL);
    char *output_float = (char *) malloc(100);
    sprintf(output_float, "float@%a", ret);

    return output_float;
}

void gen_string(char* input_string)
{
    unsigned int len = strlen(input_string);
    char buffer[5];

    char *output_string =malloc(len*4 + 7);
    strcpy(output_string, "string@");

    int i, k = 7;

    for(i = 0; i < len; i++)
    {
        if(input_string[i] == '"') {

            strcat(output_string, "\\034");
            k += 3;

        } else if(input_string[i] == '#') {

            strcat(output_string, "\\035");
            k += 3;

        } else if(input_string[i] == '\\') {
            strcat(output_string, "\\092");
            k += 3;

        }  else if(input_string[i] == ' ') {
            strcat(output_string, "\\032");
            k += 3;
        }
        else if(input_string[i] < 32) {

            sprintf(buffer, "\\%03d", input_string[i]);

            strcat(output_string, buffer);
            k += 3;

        } else {

            output_string[k] = input_string[i];

        }

        k++;
    }

    output_string[k] = '\0';
    printf("%s\n",output_string);
    free(output_string);

}

void CodeGenPrint(tToken *token) //funkce pro funkci print a jeji spravne printovani
{
    if((*token)->type==T_INT)
    {
        printf("WRITE int@%s\n", (*token)->data);
        //print cislo
    }
    else if((*token)->type==T_STRING)
    {   printf("WRITE ");
        gen_string((*token)->data);
        //printf("WRITE %s\n",(*token)->data);
    }
    else if((*token)->type==T_EXP || (*token)->type==T_DOUBLE)
    {
        (*token)->data=gen_float((*token)->data);
        printf("WRITE %s\n",(*token)->data);
    }
    else if((*token)->type==T_ID)
    {
        printf("WRITE LF@%s\n",(*token)->data);//print variable
    }

}
void gen_substr()
{   printf("#---VESTAVENA FUNKCE SUBSTR---\n");
    printf("LABEL $find_substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%ret\n");
    printf("MOVE LF@%%ret string@\n");
    printf("DEFVAR LF@index\n");
    printf("MOVE LF@index LF@i\n");
    printf("DEFVAR LF@index_end\n");
    printf("MOVE LF@index_end LF@%%delka\n");

    printf("# priznak chyby\n");
    printf("DEFVAR LF@%%ret_err1\n");
    printf("MOVE LF@%%ret_err1 int@0\n");

    printf("# osetreni chybovych stavu\n");
    printf("DEFVAR LF@err_ind\n");
    printf("LT LF@err_ind LF@index int@0\n");
    printf("JUMPIFEQ $err_substr LF@err_ind bool@true\n");
    printf("DEFVAR LF@substr_len\n");
    printf("STRLEN LF@substr_len LF@str\n");
    printf("GT LF@err_ind LF@index LF@substr_len\n");
    printf("JUMPIFEQ $err_substr LF@err_ind bool@true\n");
    printf("LT LF@err_ind LF@index_end int@0\n");
    printf("JUMPIFEQ $err_substr LF@err_ind bool@true\n");
    printf("DEFVAR LF@err_delka\n");
    printf("MOVE LF@err_delka LF@substr_len\n");
    printf("SUB LF@err_delka LF@err_delka LF@index\n");
    printf("GT LF@err_ind LF@index_end LF@err_delka\n");
    printf("JUMPIFNEQ body LF@err_ind bool@true\n");
    printf("MOVE LF@index_end LF@err_delka\n");

    printf("# telo funkce\n");
    printf("LABEL body\n");
    printf("DEFVAR LF@znak\n");
    printf("ADD LF@index_end LF@index_end LF@i\n");
    printf("LABEL cyklus\n");
    printf("GETCHAR LF@znak LF@str LF@index\n");
    printf("CONCAT LF@%%ret LF@%%ret LF@znak\n");
    printf("ADD LF@index LF@index int@1\n");
    printf("JUMPIFNEQ cyklus LF@index LF@index_end\n");
    printf("JUMP end_substr\n");

    printf("LABEL $err_substr\n");
    printf("MOVE LF@%%ret_err1 int@1\n");

    printf("LABEL end_substr\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

//func substr(s string, i int, n int) (string, int)
// mozna problem se zmenou nazvu promennych

    /* v $main
    DEFVAR LF@substr        # pro navratove hodnoty
    DEFVAR LF@err

    CREATEFRAME

    DEFVAR TF@str
    MOVE TF@str string@novy\032podretezec
    DEFVAR TF@i
    MOVE TF@i int@0
    DEFVAR TF@delka
    MOVE TF@delka int@5

    CALL $find_substr

    MOVE LF@substr TF@%ret
    MOVE LF@err TF@%ret_err1

    WRITE LF@substr    #pro vypis
    WRITE LF@err

    */
}

void gen_input()
{   printf("#---VESTAVENA FUNKCE INPUTS---\n");
    printf("LABEL $read_inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%string_read\n");
    printf("MOVE LF@%%string_read string@\n");
    printf("DEFVAR LF@%%read_err1\n");
    printf("MOVE LF@%%read_err1 int@0\n");
    printf("READ LF@%%string_read string\n");
    printf("JUMPIFEQ $err_inputs LF@%%string_read string@\n");
    printf("JUMP $end_inputs\n");
    printf("LABEL $err_inputs\n");
    printf("MOVE LF@%%read_err1 int@1\n");
    printf("LABEL $end_inputs\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("#---VESTAVENA FUNKCE INPUTI---\n");
    printf("LABEL $read_inputi\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%int_read\n");
    printf("MOVE LF@%%int_read nil@nil\n");
    printf("DEFVAR LF@%%read_err2\n");
    printf("MOVE LF@%%read_err2 int@0\n");
    printf("READ LF@%%int_read int\n");
    printf("JUMPIFEQ $err_inputi LF@%%int_read nil@nil\n");
    printf("JUMP $end_inputi\n");
    printf("LABEL $err_inputi\n");
    printf("MOVE LF@%%read_err2 int@1\n");
    printf("LABEL $end_inputi\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("#---VESTAVENA FUNKCE INPUTF---\n");
    printf("LABEL $read_inputf\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%float_read\n");
    printf("MOVE LF@%%float_read nil@nil\n");
    printf("DEFVAR LF@%%read_err3\n");
    printf("MOVE LF@%%read_err3 int@0\n");
    printf("READ LF@%%float_read float\n");
    printf("JUMPIFEQ $err_inputf LF@%%float_read nil@nil\n");
    printf("JUMP $end_inputf\n");
    printf("LABEL $err_inputf\n");
    printf("MOVE LF@%%read_err3 int@1\n");
    printf("LABEL $end_inputf\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

}

void gen_string_functions()
{   printf("#---VESTAVENA FUNKCE LEN---\n");
    printf("LABEL $find_len\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%str\n");
    printf("MOVE LF@%%str LF@str\n");
    printf("DEFVAR LF@%%delka\n");
    printf("MOVE LF@%%delka nil@nil\n");
    printf("DEFVAR LF@%%err_ind\n");
    printf("MOVE LF@%%err_ind int@0\n");
    printf("STRLEN LF@%%delka LF@%%str\n");
    printf("JUMPIFEQ $err_len LF@%%delka nil@nil\n");
    printf("JUMP $find_len_end\n");
    printf("LABEL $err_len\n");
    printf("MOVE LF@%%err_ind int@1\n");
    printf("LABEL $find_len_end\n");
    printf("POPFRAME \n");
    printf("RETURN\n");
    printf("\n");
    printf("#---VESTAVENA FUNKCE ORD---\n");
    printf("LABEL $find_ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%str\n");
    printf("MOVE LF@%%str LF@str\n");
    printf("DEFVAR LF@%%index\n");
    printf("MOVE LF@%%index LF@index\n");
    printf("DEFVAR LF@%%ret\n");
    printf("MOVE LF@%%ret nil@nil\n");
    printf("DEFVAR LF@%%err_ind\n");
    printf("LT LF@%%err_ind LF@%%index int@0\n");
    printf("JUMPIFEQ $err_ord LF@%%err_ind bool@true\n");
    printf("DEFVAR LF@str_len\n");
    printf("STRLEN LF@str_len LF@str\n");
    printf("SUB LF@str_len LF@str_len int@1\n");
    printf("GT LF@%%err_ind LF@%%index LF@str_len\n");
    printf("JUMPIFEQ $err_ord LF@%%err_ind bool@true\n");
    printf("STRI2INT LF@%%ret LF@%%str LF@%%index\n");
    printf("MOVE LF@%%err_ind int@0\n");
    printf("JUMP $end_ord\n");
    printf("LABEL $err_ord\n");
    printf("MOVE LF@%%err_ind int@1\n");
    printf("LABEL $end_ord\n");
    printf("POPFRAME \n");
    printf("RETURN\n");
    printf("\n");
    printf("#---VESTAVENA FUNKCE CHR---\n");
    printf("LABEL $find_chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%cislo\n");
    printf("MOVE LF@%%cislo LF@cislo\n");
    printf("DEFVAR LF@%%ret\n");
    printf("MOVE LF@%%ret nil@nil\n");
    printf("DEFVAR LF@%%err_ind\n");
    printf("LT LF@%%err_ind LF@%%cislo int@0\n");
    printf("JUMPIFEQ $err_chr LF@%%err_ind bool@true\n");
    printf("GT LF@%%err_ind LF@%%cislo int@255\n");
    printf("INT2CHAR LF@%%ret LF@%%cislo\n");
    printf("MOVE LF@%%err_ind int@0\n");
    printf("JUMP $end_chr\n");
    printf("LABEL $err_chr\n");
    printf("MOVE LF@%%err_ind int@1\n");
    printf("LABEL $end_chr\n");
    printf("POPFRAME \n");
    printf("RETURN\n");

}
tToken CodeGenInputi(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[])
{   //pokud jiny nez spravny pocet parametru
    if(IDCounter!=2){(*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;
    if (((*token)->type == T_LDBR) && ((*token)->nextToken->type == T_RDBR)) {
        (*token) = (*token)->nextToken;
        printf("CREATEFRAME\nCALL $read_inputi\n");
        if(strcmp(UchovaniID[1],"_")!=0){ //pokud _ tak zahazujeme, jinak musi byt int
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 'i') {
                (*token)->type = T_UNKNOWN;  (*token)->data = "ERR_SEM_KOMP";
                return *token;
            }
            printf("MOVE LF@%s TF@%%int_read\n", UchovaniID[1]);
        }
        if(strcmp(UchovaniID[2],"_")!=0) {
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i') {
                (*token)->type = T_UNKNOWN;(*token)->data = "ERR_SEM_KOMP";
                return *token;

            }

            printf("MOVE LF@%s TF@%%read_err2\n", UchovaniID[2]);
        }

        return *token;
    }
    else
    {   //pokud nekde nastala chyba
        (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX";
        return *token;
    }

}
tToken CodeGenInputf(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]){
    if(IDCounter!=2){(*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;
    if (((*token)->type == T_LDBR) && ((*token)->nextToken->type == T_RDBR)) {
        (*token) = (*token)->nextToken;
        printf("CREATEFRAME\nCALL $read_inputf\n");
        if(strcmp(UchovaniID[1],"_")!=0)
        {
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 'f')
            {
                (*token)->type = T_UNKNOWN;  (*token)->data = "ERR_SEM_KOMP";
                return *token;
            }
            printf("MOVE LF@%s TF@%%float_read\n", UchovaniID[1]);
        }


        if(strcmp(UchovaniID[2],"_")!=0)
        {
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i') {
                (*token)->type = T_UNKNOWN;   (*token)->data = "ERR_SEM_KOMP";
                return *token;
            }

            printf("MOVE LF@%s TF@%%read_err3\n", UchovaniID[2]);
        }

        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX"; return *token;
    }
}
tToken CodeGenInputs(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]) {

    if(IDCounter!=2){(*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;
    if (((*token)->type == T_LDBR) && ((*token)->nextToken->type == T_RDBR)) {
        (*token) = (*token)->nextToken;
        printf("CREATEFRAME\nCALL $read_inputs\n");
        if(strcmp(UchovaniID[1],"_")!=0)
        {
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 's') {

                (*token)->type = T_UNKNOWN;  (*token)->data = "ERR_SEM_KOMP";
                return *token;
            }
            printf("MOVE LF@%s TF@%%string_read\n", UchovaniID[1]);
        }

        if(strcmp(UchovaniID[2],"_")!=0)
        {
            if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i')
            {
                (*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_KOMP";
                return *token;
            }

            printf("MOVE LF@%s TF@%%read_err1\n", UchovaniID[2]);
        }
        return *token;
    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX"; return *token;
    }

}
// int=len(string)
tToken CodeGenLen(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]){
    if(IDCounter!=1){ (*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;
    if ((*token)->type == T_LDBR)
    {
        (*token) = (*token)->nextToken;
        //kontrola zda byla definovana a zda je to int
        if ((*token)->type == T_ID)
        {
            if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
            if(STableSearchLocalReturnType(LocalTable,(*token)->data)!='s')
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
            }
            printf("CREATEFRAME\nDEFVAR TF@str\nMOVE TF@str LF@%s\n", (*token)->data);
            printf("CALL $find_len\n");

            if(strcmp(UchovaniID[1],"_")!=0) //pokud _ tak nikam neukladame
            {
                if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 'i')
                {
                    (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SEM_KOMP";  return *token;

                }
                printf("MOVE LF@%s TF@%%delka\n",UchovaniID[1]);
            }

            (*token) = (*token)->nextToken;
            if ((*token)->type == T_RDBR)
            {
                return *token;
            }
            else
            {
                (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX"; return *token;
            }
        }
        else if ((*token)->type == T_STRING)
        {     printf("CREATEFRAME\nDEFVAR TF@str\nMOVE TF@str "); gen_string((*token)->data);
            printf("CALL $find_len\n");
            if(strcmp(UchovaniID[1],"_")!=0)
            {
                if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 'i')
                {
                    (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SEM_KOMP";  return *token;

                }
                printf("MOVE LF@%s TF@%%delka\n",UchovaniID[1]);
            }

            (*token) = (*token)->nextToken;
            if ((*token)->type == T_RDBR)
            {
                return *token;
            }
        }
        else
        {
            (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX";
            return *token;
        }

    }
    else
    {
        (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX";
        return *token;
    }
    (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SYNTAX"; return *token;
}
//string, int = substr (string, int,int)
tToken CodeGenSubstr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[]){
    if(IDCounter!=2){ (*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;

    printf("CREATEFRAME\n DEFVAR TF@str\n");

    if ((*token)->type == T_LDBR)
    {

        (*token) = (*token)->nextToken;
        if (((*token)->type == T_ID)||((*token)->type == T_STRING))
        {
            if((*token)->type == T_STRING)
            {
                printf("MOVE TF@str "); gen_string((*token)->data);
                (*token) = (*token)->nextToken;

            }
            else
            {
                printf("MOVE TF@str LF@%s\n", (*token)->data);
                if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                }
                if(STableSearchLocalReturnType(LocalTable,(*token)->data)!='s')
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                }
                (*token) = (*token)->nextToken;
            }
            if((*token)->type != T_COMMA)
            {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
            }
            (*token) = (*token)->nextToken;

            if (((*token)->type == T_ID)||((*token)->type == T_INT))
            {

                if ((*token)->type == T_INT)
                {
                    printf("DEFVAR TF@%%i\n MOVE TF@%%i int@%s\n",(*token)->data);
                    (*token) = (*token)->nextToken;
                }
                else
                {
                    printf("DEFVAR TF@%%i\n MOVE TF@%%i LF@%s\n",(*token)->data);
                    if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                    }
                    if(STableSearchLocalReturnType(LocalTable,(*token)->data)!='i')
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                    }
                    (*token) = (*token)->nextToken;
                }
                if((*token)->type != T_COMMA)
                {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
                }
                (*token) = (*token)->nextToken;
                if (((*token)->type == T_ID)||((*token)->type == T_INT))
                {    //editted

                    if ((*token)->type == T_INT)
                    {
                        printf("DEFVAR TF@%%delka\nMOVE TF@%%delka int@%s\nCALL $find_substr\n",(*token)->data);
                        (*token) = (*token)->nextToken;
                    }
                    else
                    {
                        printf("DEFVAR TF@%%delka\nMOVE TF@%%delka LF@%s\nCALL $find_substr\n",(*token)->data);
                        if(STableSearchLocal(LocalTable,(*token)->data)==NULL)
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                        }
                        if(STableSearchLocalReturnType(LocalTable,(*token)->data)!='i')
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data="ERR_SEM_POCET"; return *token;
                        }
                        (*token) = (*token)->nextToken;
                    }
                    if((*token)->type != T_RDBR)
                    {
                        (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
                    }
                    if(strcmp(UchovaniID[1],"_")!=0)
                    {
                        if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 's')
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SEM_KOMP";  return *token;
                        }
                        printf("MOVE LF@%s TF@%%ret\n",UchovaniID[1]);
                    }
                    if(strcmp(UchovaniID[2],"_")!=0)
                    {
                        if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i')
                        {
                            (*token)->type=T_UNKNOWN; (*token)->data = "ERR_SEM_KOMP";  return *token;
                        }
                        printf(" MOVE LF@%s TF@%%ret_err1\n",UchovaniID[2]);
                    }

                    return *token;

                }
            }

        }

    }
    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
}
//int,int=ord(string,int)
tToken CodeGenOrd(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[])
{
    printf("CREATEFRAME\nDEFVAR TF@str\n");
    if(IDCounter!=2){ (*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;} //pokud nesedi pocet
    (*token) = (*token)->nextToken;
    if ((*token)->type == T_LDBR)
    {
        (*token) = (*token)->nextToken;
        if (((*token)->type == T_ID)||((*token)->type == T_STRING))
        {
            if ((*token)->type == T_STRING)
            {
                printf("MOVE TF@str "); gen_string((*token)->data);
                (*token) = (*token)->nextToken;
            } else {
                printf("MOVE TF@str LF@%s", (*token)->data);
                if (STableSearchLocal(LocalTable, (*token)->data) == NULL) {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_POCET";
                    return *token;
                }
                if (STableSearchLocalReturnType(LocalTable, (*token)->data) != 's') {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_POCET";
                    return *token;
                }
                (*token) = (*token)->nextToken;
            }
            if ((*token)->type != T_COMMA) {
                (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
            }
            (*token) = (*token)->nextToken;

            if (((*token)->type == T_ID)||((*token)->type == T_INT))
            {    printf("DEFVAR TF@index\nMOVE TF@index int@%s\nCALL $find_ord\n",(*token)->data);


                if ((*token)->type == T_INT) {
                    (*token) = (*token)->nextToken;
                } else {
                    if (STableSearchLocal(LocalTable, (*token)->data) == NULL) {
                        (*token)->type = T_UNKNOWN;
                        (*token)->data = "ERR_SEM_POCET";
                        return *token;
                    }
                    if (STableSearchLocalReturnType(LocalTable, (*token)->data) != 'i') {
                        (*token)->type = T_UNKNOWN;
                        (*token)->data = "ERR_SEM_POCET";
                        return *token;
                    }
                    (*token) = (*token)->nextToken;
                }

                if(strcmp(UchovaniID[1],"_")!=0)
                {
                    if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 'i') {
                        (*token)->type = T_UNKNOWN;
                        (*token)->data = "ERR_SEM_KOMP";
                        return *token;
                    }
                    printf("MOVE LF@%s TF@%%ret\n",UchovaniID[1]);

                }
                if(strcmp(UchovaniID[1],"_")!=0)
                {
                    if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i') {
                        (*token)->type = T_UNKNOWN;
                        (*token)->data = "ERR_SEM_KOMP";
                        return *token;
                    }
                    printf("MOVE LF@%s TF@%%err_ind\n",UchovaniID[1]);
                }
                if ((*token)->type != T_RDBR) {
                    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
                }

                return *token;
            }
        }

    }

    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
}
//string, int = chr(int)
tToken CodeGenChr(tToken *token, tSymbolTablePtrPromenna LocalTable,int IDCounter,char *UchovaniID[])
{
    if(IDCounter!=2){ (*token)->type = T_UNKNOWN; (*token)->data = "ERR_SEM_POCET"; return *token;}
    (*token) = (*token)->nextToken;
    if ((*token)->type == T_LDBR)
    {    printf("CREATEFRAME\nDEFVAR TF@cislo\n");
        (*token) = (*token)->nextToken;

        if (((*token)->type == T_ID)||((*token)->type == T_INT))
        {
            if ((*token)->type == T_INT) {
                printf("MOVE TF@cislo int@%s\n CALL $find_chr\n",(*token)->data);
                (*token) = (*token)->nextToken;
            } else {
                printf("MOVE TF@cislo LF@%s\n CALL $find_chr\n",(*token)->data);
                if (STableSearchLocal(LocalTable, (*token)->data) == NULL) {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_POCET";
                    return *token;
                }
                if (STableSearchLocalReturnType(LocalTable, (*token)->data) != 'i') {
                    (*token)->type = T_UNKNOWN;
                    (*token)->data = "ERR_SEM_POCET";
                    return *token;
                }
                (*token) = (*token)->nextToken;

                if(strcmp(UchovaniID[1],"_")!=0)
                {
                    if (STableSearchLocalReturnType(LocalTable, UchovaniID[1]) != 's') {
                        (*token)->type = T_UNKNOWN;
                        (*token)->data = "ERR_SEM_KOMP";
                        return *token;
                    }
                    printf("MOVE LF@%s TF@%%ret\n",UchovaniID[1]);
                }

                if(strcmp(UchovaniID[2],"_")!=0)
                {    if (STableSearchLocalReturnType(LocalTable, UchovaniID[2]) != 'i')
                    {
                        (*token)->type = T_UNKNOWN;(*token)->data = "ERR_SEM_KOMP";
                        return *token;
                    }
                    printf("MOVE LF@%s TF@%%err_ind\n",UchovaniID[2]);
                }
            }
            if ((*token)->type != T_RDBR) {
                (*token)->type = T_UNKNOWN;
                (*token)->data = "ERR_SYNTAX";
                return *token;
            }

            return *token;
        }
    }
    (*token)->type=T_UNKNOWN; (*token)->data="ERR_SYNTAX"; return *token;
}
