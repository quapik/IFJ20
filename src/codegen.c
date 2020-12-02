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

    printf("%s\n", strlen(output_float));
    return output_float;
}

char *generate_string(char* input_string) 
{
    
    int len = strlen(input_string);
    char buffer[5]; 
    
    char *output_string = malloc(len*3 + 7);
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

        } else if(input_string[i] <= 32) {
            
            sprintf(buffer, "\\%03d", input_string[i]);
            
            strcat(output_string, buffer);
            k += 3;
            
        } else {
            
            output_string[k] = input_string[i];
            
        }

        k++; 
    }
    
    output_string[k] = '\0';

    return output_string;
}

void CodeGenPrint(tToken *token)
{
    if((*token)->type==T_INT)
    {
        printf("WRITE INT");
        //print cislo
    }
    else if((*token)->type==T_STRING)
    {
        //print string
        printf("WRITE string");
    }
    else if((*token)->type==T_EXP)
    {
        printf("WRITE exp");
        //print flaot? TODO co double?
    }
    else if((*token)->type==T_ID)
    {
        printf("WRITE LF@%s\n",(*token)->data);//print variable
    }

}

