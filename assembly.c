#include "assembly.h"  

word *memory;
word *data_memory;

int main(int argc, char **argv)
{
   
   FILE *current_fp;   
   int i = 1;

   if(argc == 1)
   {
       fprintf(stdout,"assembler: please enter source code file name\n");
       exit(0);
   }
    printf("MAIN STARTED\n");
   while (--argc)
   {
        char *file_name= (char *)malloc(strlen(argv[i])+strlen(ASSEMBLY_POSTFIX)+10); // memory allocation for the file name and the file postfix +10 for the ending char and extra space
        strcpy(file_name,argv[i]); //copying the file name to the allocated string
        add_assembly_postfix(file_name); //adding the ".as" postfix to the filename

        current_fp = fopen(file_name,"r");
        if(!current_fp)
        {
            fprintf(stdout,"assembler: Can't open file \'%s\'\n",file_name);
            continue;
        }

       memory = (word *)malloc(sizeof(word)*MAX_WORDS);
       data_memory = (word *)malloc(sizeof(word)*MAX_WORDS);
        printf("entering first pass\n");
        first_pass(current_fp,file_name);// the first pass

        free (data_memory); //merging the data structures in the end of first pass
        
        printf("entering second pass\n");
        second_pass(current_fp,file_name); //the second pass

    
        if(!error_flag)
            make_output(argv[i]);

        i++;
    

   }
    

    return 0;
}