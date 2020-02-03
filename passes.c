#include "passes.h"

short IC;
short DC;
int line_counter;
unsigned char error_flag;

int first_pass(FILE *fp, char *file_name)
{
    int word_req;   // current word requirement for the instruction
    unsigned char line_flag;
    unsigned char command;
    enum line_type current_type;
    char *current_line = (char *)malloc(MAX_LINE);
    char *symbol_name = (char *)malloc(SYMBOL_MAX_LENGTH);
    
    
    IC=0;
    DC=0;
    line_counter=0;


    while( (line_flag = getline(fp,current_line))!= EOF){
        line_counter++;
        current_type = check_type(current_line);     //checking the type of the current line : 0-code,1-data,2-string,3-entry,4-extern
        if(current_type){
            if(line_flag){ //if symbol is declared in this line
                get_symbol(current_line,symbol_name);
                add_symbol(symbol_name,DC,data);
            }
            if(current_type == entry) //handling with entry happens in the second pass
                continue;
            else if(current_type == external) {
                add_extern(current_line); //adding to the symbol table all the operators in the extern line.
            }
            else{
                code_data(current_line,DC,current_type,line_flag);
            }
        }
        else{  //instruction line
            if(line_flag){
                add_symbol(symbol_name,IC+LOAD_SPACE,code);
            }
            command = get_command(current_type,line_flag); //if -1 error, else return the index of the command.
            //code_code(current_line,IC);
            //
            //
            //
        }
    
   
    
    }//end of while
    

    return 1;
}

int second_pass(FILE *fp, char *file_name)
{

    return 1;
}