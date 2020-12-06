//Implementace překladače imperativního jazyka IFJ20
//Tým číslo 041, varianta I
/*Autoři projektu:
Šíma Vojtěch 	xsimav01@stud.fit.vutbr.cz
Fabián Michal   xfabia13@stud.fit.vutbr.cz
Čábela Radek    xcabel04@stud.fit.vutbr.cz
Poposki Vasil   xpopos00@stud.fit.vutbr.cz
Prosinec 2020, Fakulta informačních technologií VUT v Brně
Generování kódu
*/

#include "codegen.h"

void CodeGenStart()
{
    printf(".IFJcode20\n");
    printf("JUMP $$main\n");
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

void CodeGenPrint(tToken *token)
{
    if((*token)->type==T_INT)
    {
        printf("WRITE int@%s", (*token)->data);
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
{
printf("LABEL $find_substr\n");
printf("PUSHFRAME\n");
printf("DEFVAR LF@%%ret\n");
printf("MOVE LF@%%ret string@\n");
printf("DEFVAR LF@index\n");
printf("MOVE LF@index LF@i\n");
printf("DEFVAR LF@index_end\n");
printf("MOVE LF@index_end LF@delka\n");
       
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
}

void gen_input()
{
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
{
printf("LABEL $find_len\n");
printf("PUSHFRAME\n");
printf("DEFVAR LF@%str\n");
printf("MOVE LF@%str LF@str\n");
printf("DEFVAR LF@%delka\n");
printf("MOVE LF@%delka nil@nil\n");
printf("DEFVAR LF@%err_ind\n");
printf("MOVE LF@%err_ind int@0\n");
printf("STRLEN LF@%delka LF@%str\n");
printf("JUMPIFEQ $err_len LF@%delka nil@nil\n");
printf("JUMP $find_len_end\n");
printf("LABEL $err_len\n");
printf("MOVE LF@%err_ind int@1\n");
printf("LABEL $find_len_end\n");
printf("POPFRAME \n");
printf("RETURN\n");
printf(" \n");
printf("LABEL $find_ord\n");
printf("PUSHFRAME\n");
printf("DEFVAR LF@%str\n");
printf("MOVE LF@%str LF@str\n");
printf("DEFVAR LF@%index\n");
printf("MOVE LF@%index LF@index\n");
printf("DEFVAR LF@%ret\n");
printf("MOVE LF@%ret nil@nil\n");
printf("DEFVAR LF@%err_ind\n");
printf("LT LF@%err_ind LF@%index int@0\n");
printf("JUMPIFEQ $err_ord LF@%err_ind bool@true\n");
printf("DEFVAR LF@str_len\n");
printf("STRLEN LF@str_len LF@str\n");
printf("SUB LF@str_len LF@str_len int@1\n");
printf("GT LF@%err_ind LF@%index LF@str_len\n");
printf("JUMPIFEQ $err_ord LF@%err_ind bool@true\n");
printf("STRI2INT LF@%ret LF@%str LF@%index\n");
printf("MOVE LF@%err_ind int@0\n");
printf("JUMP $end_ord\n");
printf("LABEL $err_ord\n");
printf("MOVE LF@%err_ind int@1\n");
printf("LABEL $end_ord\n");
printf("POPFRAME \n");
printf("RETURN\n");
printf("\n");
printf("LABEL $find_chr\n");
printf("PUSHFRAME\n");
printf("DEFVAR LF@%cislo\n");
printf("MOVE LF@%cislo LF@cislo\n");
printf("DEFVAR LF@%ret\n");
printf("MOVE LF@%ret nil@nil\n");
printf("DEFVAR LF@%err_ind\n");
printf("LT LF@%err_ind LF@%cislo int@0\n");
printf("JUMPIFEQ $err_chr LF@%err_ind bool@true\n");
printf("GT LF@%err_ind LF@%cislo int@255\n");
printf("INT2CHAR LF@%ret LF@%cislo\n");
printf("MOVE LF@%err_ind int@0\n");
printf("JUMP $end_chr\n");
printf("LABEL $err_chr\n");
printf("MOVE LF@%err_ind int@1\n");
printf("LABEL $end_chr\n");
printf("POPFRAME \n");
printf("RETURN\n");
}
