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
    printf("DEFVAR @LF %s\n",id);
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
        printf("WRITE INT");
        //print cislo
    }
    else if((*token)->type==T_STRING)
    {   printf("test %s\n",(*token)->data);
        gen_string((*token)->data);
        //printf("WRITE %s\n",(*token)->data);
    }
    else if((*token)->type==T_EXP)
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
printf("LABEL $find_substr\n);
printf("PUSHFRAME\n);
printf("DEFVAR LF@%ret\n");
printf("MOVE LF@%ret string@\n")
printf("DEFVAR LF@index\n");
printf("MOVE LF@index LF@i\n");
printf("DEFVAR LF@index_end\n");
printf("MOVE LF@index_end LF@delka\n")
printf("ADD LF@index_end LF@index_end LF@i\n");
printf("DEFVAR LF@znak\n");

printf("LABEL cyklus\n");
printf("GETCHAR LF@znak LF@str LF@index\n");
printf("CONCAT LF@%ret LF@%ret LF@znak\n");
printf("ADD LF@index LF@index int@1\n");
printf("JUMPIFNEQ cyklus LF@index LF@index_end\n");

printf("POPFRAME\n");
printf("RETURN\n");
    
}
