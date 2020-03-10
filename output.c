#include "output.h"

void add_assembly_postfix(char *file_name)
{
    strcat(file_name,ASSEMBLY_POSTFIX);
    return;
}

void make_output(char *file_name)
{
    make_object_file(file_name);
    make_symbol_file(file_name,entry,ENTRY_POSTFIX);
    make_symbol_file(file_name,external,EXTERN_POSTFIX);
    return;
}

void make_symbol_file(char *filename,enum line_type type,char *extension)
{
    FILE *fp;
    char *new_file = (char *) malloc ( strlen(filename) + strlen(extension) + EXTRA_SPACE );
    int count;

    /*add .ext postfix to file name*/
    strcpy(new_file,filename);
    strcat(new_file,extension);

    count = get_symbol_amount(type); /*return the amount of exxtern symbols to know we we need to create a .ext file*/


    if(count==0) /*if no external symbols no need to create the file*/
        return;

    fp = fopen(new_file,"w"); /*create the .ext file in write mode*/
    
    if(!fp) /*if we couldn't create the .ext file in write mode*/
    {
        fprintf(stderr,"Assembler: an error accord when tried to open file %s. file not created\n",new_file);
        free(new_file);
        return;
    }

    print_symbols(fp,type); /*print all external symbols into .ext file and their value*/

    /*close .ext file and free the string of the file name*/
    fclose(fp);
    free(new_file);

    return;
}

void make_object_file(char *filename)
{
    short temp_IC = 0; 
    FILE *objfp;/*the file with th .ob postfix*/
    char *obj_file = (char *) malloc ( strlen(filename) + strlen(OBJECT_POSTFIX) + EXTRA_SPACE );
    
    /*add the .ob file postfix*/
    strcpy(obj_file,filename);
    strcat(obj_file,OBJECT_POSTFIX);



    if( !IC && !DC ) /*if the value of both IC and DC is zero, no memory allocated*/
    {
        fprintf(stderr,"Assembler: no memory to print. object file not created.\n");
        free(obj_file);
        return;
    }

    objfp = fopen(obj_file,"w"); /*open the file with the .ob file postfix in write mode*/
    
    if(!objfp) /*if couldn't open the file*/
    {
        fprintf(stderr,"Assembler: an error accord when tried to open file %s. file not created\n",obj_file);
        free(obj_file);
        return;
    }

    fprintf(objfp,"\n\t%d %d\n",IC,DC); /*The first line of the file - the values of IC & DC*/
    
    while(temp_IC < (IC+DC) ) /*printing all the data*/
    {
        fprintf(objfp,"%04d %05o\n",temp_IC+LOAD_SPACE,memory[temp_IC]);
        temp_IC++;
    }

    fclose(objfp);
    free(obj_file);

    return;
}

