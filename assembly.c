#include "assembly.h"  

word *memory;
word *data_memory;

int main(int argc, char **argv)
{
   
   FILE *current_fp;   
   int i = 1;

   if(argc == 1)
   {
       fprintf(stderr,"Assembler: please enter source code file name\n");
       exit(0);
   }

   while (--argc)
   {
        char *file_name= (char *)malloc(strlen(argv[i])+strlen(ASSEMBLY_POSTFIX)+10); /* memory allocation for the file name and the file postfix +10 for the ending char and extra space*/
        strcpy(file_name,argv[i]); /*copying the file name to the allocated string*/
        add_assembly_postfix(file_name); /*adding the ".as" postfix to the filename*/

        current_fp = fopen(file_name,"r");
        if(!current_fp)
        {
            fprintf(stderr,"Assembler: Can't open file \'%s\'\n",file_name);
            continue;
        }

       memory = (word *)malloc(sizeof(word)*MAX_WORDS);
       data_memory = (word *)malloc(sizeof(word)*MAX_WORDS);

        first_pass(current_fp,file_name);/*the first pass*/

        free (data_memory); /*merging the data structures in the end of first pass*/
        
        second_pass(current_fp,file_name); /*the second pass*/

    
        if(!error_flag)
            make_output(argv[i++]);
    
        fclose(current_fp); /*closing the current file */
        free_symbol_table(); /*free the symbol table */
        free(memory); /* free the memory*/
   }
    
    

    return 0;
}
