#include "assembly.h"

int first_pass(FILE *, char *);
int second_pass(FILE *, char *);


int main(int argc, char **argv)
{
   word *memory = (word *)malloc(sizeof(word)*MAX_WORDS);
   word *tmp_data_memory = (word *)malloc(sizeof(word));
   FILE *current_fp;
   char *file_name;
   int i = 1;

   if(argc == 1)
   {
       fprintf(stdout,"assembler: please enter source code file name\n");
       exit(0);
   }

   while (--argc)
   {
     file_name = argv[i];
     current_fp = fopen(file_name,"r");
     if(!current_fp)
     {
        fprintf(stdout,"assembler: Can't open file \'%s\'\n",file_name);
        continue;
     }
    
    first_pass(current_fp,file_name);
    
    second_pass(current_fp,file_name);

    /*
        if(!error_flag)
            make_output();
    */

   }
    

    return 0;
}