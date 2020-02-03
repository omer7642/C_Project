#include "codingFunctions.h"


void code_data(char *line,int memory_value,enum line_type type,int symbol_flag)
{
    char *token,c;
    int i,L;

    token = strtok(line, " \t");
    if(symbol_flag)
        token = strtok(NULL, " \t");
    token = strtok(NULL, " \t");

    if(type == string)
    {
        L = strlen(token);
        if(token[0]!="\"" || token[L-1]!="\"")
        {
            error_flag=1;
            fprintf(stdout,"assembler: error - missing quotation marks (line %d)\n",line_counter);
            return;
        }
        
        tmp_data_memory = (word *)realloc(tmp_data_memory,sizeof(word)*(DC+L));
        i=DC;
        while (i<L)
        {
            
        }
        
    } 
}

