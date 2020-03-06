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
    short temp_DC = 0, temp_IC = 0; 
    FILE *objfp;
    char *obj_file = (char *) malloc ( strlen(filename) + strlen(OBJECT_POSTFIX) + EXTRA_SPACE );
    
    strcpy(obj_file,filename);
    strcat(obj_file,OBJECT_POSTFIX);

    if( !IC && !DC )
    {
        fprintf(stderr,"Assembly: no memory to print. object file not created.\n");
        free(obj_file);
        return;
    }

    objfp = fopen(obj_file,"w");
    
    if(!objfp)
    {
        fprintf(stderr,"assembly: an error accord when tried to open file %s. file not created\n",obj_file);
        free(obj_file);
        return;
    }

    fprintf(objfp,"\n\t %d %d\n",IC,DC); // The first line of the file - the values of IC & DC
    
    while(temp_IC < IC) //printing all the data
    {
        fprintf(objfp,"\t%4d \t %5o\n",temp_IC+LOAD_SPACE,memory[temp_IC]);
        temp_IC++;
    }

    fclose(objfp);
    free(obj_file);

    fprintf(stderr,"OBJ FILE CREATED SUCCESSESFULLY !\n");

    return;
}

void make_entry_file(char *filename)
{
    FILE *entfp;
    char *ent_file = (char *) malloc ( strlen(filename) + strlen(ENTRY_POSTFIX) + EXTRA_SPACE );
    int ent_count;

    strcpy(ent_file,filename);
    strcat(ent_file,ENTRY_POSTFIX);

    ent_count = get_symbol_amount(entry);
    printf("output - ent amout is %d \n",ent_count);

    if(ent_count==0)
        return;

    entfp = fopen(ent_file,"w");
    if(!entfp)
    {
        fprintf(stderr,"assembly: an error accord when tried to open file %s. file not created\n",ent_file);
        free(ent_file);
        return;
    }

    print_symbols(entfp,entry);

    fclose(entfp);
    free(ent_file);

    return;
}

void make_extern_file(char *filename)
{
    FILE *extfp;
    char *ext_file = (char *) malloc ( strlen(filename) + strlen(EXTERN_POSTFIX) + EXTRA_SPACE );
    int ext_count;

    strcpy(ext_file,filename);
    strcat(ext_file,EXTERN_POSTFIX);

    ext_count = get_symbol_amount(external);
    printf("output - ext amout is %d \n",ext_count);


    if(ext_count==0)
        return;

    extfp = fopen(ext_file,"w");
    if(!extfp)
    {
        fprintf(stderr,"assembly: an error accord when tried to open file %s. file not created\n",ext_file);
        free(ext_file);
        return;
    }

    print_symbols(extfp,external);

    fclose(extfp);
    free(ext_file);

    return;

}
