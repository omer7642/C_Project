#include "output.h"

void add_assembly_postfix(char *file_name)
{
    strcat(file_name,ASSEMBLY_POSTFIX);
    return;
}

/*this is simply a shell function that receives a filename and create output files according to the memory image, by calling another 
side functions */
void make_output(char *file_name)
{
    make_object_file(file_name);
    make_symbol_file(file_name,entry,ENTRY_POSTFIX);
    make_symbol_file(file_name,external,EXTERN_POSTFIX);
    return;
}

/*this function creates either .ext and .ent files in case any external or entry lines detected. 
it receives a filename, the type and the postfix. it then concatenates the filename and the postfix, counts the number of symbols of the symbol type, opens 
a file if any symbol of that type were declared, and prints the symbols and their values into the file*/
void make_symbol_file(char *filename,enum line_type type,char *extension)
{
    FILE *fp;
    char *new_file = (char *) malloc ( strlen(filename) + strlen(extension) + EXTRA_SPACE );
    int count;

    /*add postfix to file name*/
    strcpy(new_file,filename);
    strcat(new_file,extension);

    count = get_symbol_amount(type); /*checking if symbols from the specific type exist */


    if(count==0) /*if symbols from the specific type not exist, no need to create the file*/
        return;

    fp = fopen(new_file,"w"); /*create the file in write mode*/
    
    if(!fp) /*if we couldn't create the file in write mode*/
    {
        fprintf(stderr,"Assembler: an error accord when tried to open file %s. file not created\n",new_file);
        free(new_file);
        return;
    }

    print_symbols(fp,type); /*print all symbols into from the specific type to the .ext and their value*/

    /*close the file and free the string of the file name*/
    fclose(fp);
    free(new_file);

    return;
}
 
/*this function receives a filename, adding the .ob extension to it, then print the memory image into the file, and the sum of instruction lines, and 
data lines then print the line number and its content in octal base.*/
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

