/*this program serves as an assembler. the user puts in the files with the .as file extensions and the program goes through them twice:
first time it decodes most of the code, and builds a table for the symbols so that each line is defined correctly.
second pass decodes all the symbol that are left inside the code, and if no errors occure during the run of the program, it prints out
to .ob, .ext and .ent files the memory image of the program. memory image includes the number of instruction lines, the nubmer of data lines and
for each command, its place in the memory and its value in octal base.
ASSUMPTIONS: (defined)
1. memory is limited to 4096 "words" 
2. each line is limited to only 80 characters
3. each symbol is limited to only 32 characters
4. each statemant is presented in a separate line

thie program was written by:
Omer Zinman - 313175739
Rotem Mizrahi 
*/


#include "assembly.h"  

word *memory;
word *data_memory;

/*the main function of the program. receives as input .as file names , iterates on the command line arguments, opens each line, does first and second 
pass. if no error occured during these passes, then it prints the output*/

int main(int argc, char **argv)
{
   
   FILE *current_fp; /*the current file we open and work on, without the .as postfix*/
   int i = 1; /*index of iteration of the command line arguments*/

   if(argc == 1) /*if no filename was entered in the command line*/
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
        free(data_memory);
        second_pass(current_fp,file_name); /*the second pass*/

        if(!error_flag)
            make_output(argv[i]);
    
        fclose(current_fp); /*closing the current file */
        free_symbol_table(); /*free the symbol table */
        free(memory); /* free the memory*/
        
        i++;
   }
    
    

    return 0;
}
