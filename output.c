#include "output.h"

void add_assembly_postfix(char *file_name)
{
    strcat(file_name,ASSEMBLY_POSTFIX);
    return;
}

void make_output(char *file_name)
{
    make_object_file(file_name);
    make_entry_file(file_name);
    make_extern_file(file_name);
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

void make_entry_file(char *filename)
{
    FILE *entfp;
    char *ent_file = (char *) malloc ( strlen(filename) + strlen(ENTRY_POSTFIX) + EXTRA_SPACE );
    int ent_count;

    /*add the .ent postfix to the filename*/
    strcpy(ent_file,filename);
    strcat(ent_file,ENTRY_POSTFIX);
    /*return the amount of entry symbols*/
    ent_count = get_symbol_amount(entry);

    if(ent_count==0) /*if no entry symbols, escape without creating the .ent file*/
        return;

    entfp = fopen(ent_file,"w"); /*open the ,ent file in write mode*/
    if(!entfp) /*if couldn't open the .ent file*/
    {
        fprintf(stderr,"Assembler: an error accord when tried to open file %s. file not created\n",ent_file);
        free(ent_file);
        return;
    }

    print_symbols(entfp,entry); /*print all the entry symbols and their value*/

    /*close file and free filename string*/
    fclose(entfp);
    free(ent_file);

    return;
}

void make_extern_file(char *filename)
{
    FILE *extfp;
    char *ext_file = (char *) malloc ( strlen(filename) + strlen(EXTERN_POSTFIX) + EXTRA_SPACE );
    int ext_count;

    /*add .ext postfix to file name*/
    strcpy(ext_file,filename);
    strcat(ext_file,EXTERN_POSTFIX);

    ext_count = get_symbol_amount(external); /*return the amount of exxtern symbols to know we we need to create a .ext file*/


    if(ext_count==0) /*if no external symbols no need to create the file*/
        return;

    extfp = fopen(ext_file,"w"); /*create the .ext file in write mode*/
    
    if(!extfp) /*if we couldn't create the .ext file in write mode*/
    {
        fprintf(stderr,"Assembler: an error accord when tried to open file %s. file not created\n",ext_file);
        free(ext_file);
        return;
    }

    print_symbols(extfp,external); /*print all external symbols into .ext file and their value*/

    /*close .ext file and free the string of the file name*/
    fclose(extfp);
    free(ext_file);

    return;

}
