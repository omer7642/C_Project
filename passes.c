#include "passes.h" 

short IC;/*instruction counter, counts the number of instruction words in the program's memory*/
short DC;/*data counter, counts the amount of words used for data in the program's memory*/
int line_counter; /*tells which line our errors refers to in the assembly file*/
unsigned char error_flag,second_pass_flag; /*if error flags is up. meaning assembly files has errors, no files will be created. second pass flag makes sure 
                                            errors don't reappear in the second pass as well creating double-errors*/

/*this function receives a file pointer to the file we opened with the .as extension, and a filename we opened. 
it checks line by line. while we don't reach the end of the file, we check if a line has a symbol declaration.
we then divide it by casesm for instruction line or data line, and for synbol declaration in the line or not. 
we add all symbols in the first pass into symbol table so that in the second pass we can go through everything with the rigt lines*/

void first_pass(FILE *fp, char *file_name)
{
    char line_flag; /*tells if the line has a symbol decleration*/
    unsigned char command; /*index of the command*/
    enum line_type current_type; /*type of the line (instruction,data,string,entry,external)*/
    char *current_line = (char *)malloc(MAX_LINE); /*holds the current command line*/
    char *symbol_name = (char *)malloc(SYMBOL_MAX_LENGTH); /*holds the current symbol name (if there is one)*/
    int i,j;
    
    /*zero all counters before run*/

    IC=0; 
    DC=0; 
    line_counter=0; 


    while( (line_flag = get_line(fp,current_line))!= EOF)
    {    
        line_counter++;
        if(line_flag == EMPTY_LINE) /*if emptry line, we just count it in line_counter and go to the next iteration*/
            continue;
        
        current_type = check_type(current_line);   /*checking the type of the current line : 0-code,1-data,2-string,3-entry,4-extern*/
        if(current_type)
        { 
            if(line_flag){ /*if symbol is declared in this line*/
                get_symbol(current_line,symbol_name);
                if(symbol_name == NULL || !(*symbol_name))
                {
                    error_flag = TRUE;
                    fprintf(stderr,"Assembler: Invaild symbol name (line %d)\n",line_counter);
                } 
                add_symbol(symbol_name,DC,current_type);
            }

            if(current_type == entry)  /*I dont think this action needed*/
            {   
                /*add_entry_symbol(current_line,line_flag);*/
                continue;
            }

            else if(current_type == external)
            {
                add_extern(current_line); /*adding to the symbol table all the operators in the extern line.*/
            }

            else
            {
                code_data(current_line,current_type,line_flag);
            }

        }
        
        else{  /*instruction line*/
            
            if(line_flag){
                get_symbol(current_line,symbol_name);
                if(symbol_name == NULL || !(*symbol_name)) /*sign that the symbol name in invalid*/
                {
                    error_flag = TRUE;
                    fprintf(stderr,"Assembler: Invaild symbol name (line %d)\n",line_counter);
                } 
                add_symbol(symbol_name,IC+LOAD_SPACE,code);
            }
            command = get_command(current_line,line_flag); /*if -1 error, else return the index of the command.*/
            code_instruction(current_line,command,line_flag); /*getting the line, Instruction ,the command index, and line flag - If there is a symbol or not*/
            
        }
   
    }/*end of while*/
    /*free the dynamic memory the we used in the function*/
        free(current_line);
        free(symbol_name);
    
    for( i=IC ,j=0 ; j<DC ; j++,i++) /*merging the two data structures;*/
        memory[i] = data_memory[j];
    update_data_symbol();/*adding to all data symbol the value of IC+100 because the merging of the data structures*/

    
    return;
}

/*this function is being executed after the first pass. we receive the pointer to the file we opened and a filename. the symbol table should already been
dealt with and we simply decode the symbols and go through it again. if  the line is instruction then we divide it by number of opcodes it has, if its non instruction
then its entry and we check its validity and decode it if necessary.*/
int second_pass(FILE *fp, char *file_name)
{
    int IC_temp=0 ,line_cnt_tmp=0, L; 
    char *token,line_flag,command;
    enum line_type current_type; /*type of the line (instruction,data,string,entry,external)*/
    enum address_type address,des_address,src_address;
    char *current_line = (char *)malloc(MAX_LINE); /*holds the current command line*/
    char *temp_line = (char *)malloc(MAX_LINE);


    EXIT_IF_RUNOUT_MEMORY
    rewind(fp); /*returning the file pointer to the beggining of the file*/
    second_pass_flag = 1;
    
    while(IC_temp < IC)
    {
        L=0;

        if( (line_flag = get_line(fp,current_line)) == EOF)
        {
            error_flag =TRUE;
            fprintf(stderr,"Assembler: returned EOF from the source file in the second pass \n");
            return 0;
        }

        line_cnt_tmp++;
        current_type = check_type(current_line);   /*checking the type of the current line : 0-code,1-data,2-string,3-entry,4-extern*/
        strcpy(temp_line,current_line);

        if (line_flag == EMPTY_LINE) /*of the line is empty, simply do nothing and continue to next line*/
            continue;

        if(current_type) /*if a non-instruction line*/
        {

            if (current_type == entry) /*if line is of type entry - meaning an entry symbol is defined there*/
            {
                
                token = strtok(temp_line," \t");
                if(line_flag)
                    token = strtok(NULL," \t");

                token = strtok(NULL," \t\n"); /*getting the name of the symbol;*/

                if NOT_OK_CHAR(token) /*if symbol startswith a non-legit character*/
                {
                    error_flag = TRUE;
                    fprintf(stderr,"Assembler: entry line without arguments (line %d) \n",line_cnt_tmp);
                    continue;
                }

                if (!add_entry(token)) /*if the symbol not exist in the symbol_table*/
                {
                    error_flag = TRUE;
                    fprintf(stderr,"Assembler: non existing symbol in entry line (line %d)\n",line_cnt_tmp);
                }

            }
            else /*no need to deal with another data lines in the second pass*/
                continue;
        }

        else /*instruction line*/
        {
            command = get_command(current_line, line_flag); /*extract the opcode of the command*/

            if(command<0) /*undefined command line*/
                continue; 
            
            if( command==stop || command==rts) /*opcodes without arguments*/
            {
                IC_temp++; /*this commands require only one word of memory*/
                continue;
            }/*end of no operand opcodes*/

            else if(command <rts && command >lea ) /*all the commands with one opcode argument*/
            {
                L=2; /*this opcodes require two memory words*/
                
                token = strtok(temp_line," \t");
                if(line_flag)
                    token = strtok(NULL," \t");

                token = strtok(NULL," \t\n"); /*getting the operand*/
                
                if NOT_OK_CHAR(token){ /*found an error in the second pass*/
                    L--;
                    continue;
                }

                address = get_address_type(token); /*getting the adrress method*/

                if(address!=direct) /*no symbol in the line*/
                {
                    IC_temp+=L;
                    continue;
                }            
                
                while ( isspace(*token) ) /*skipping the spaces*/
                    token++;

                if(! add_symbol_value(token,IC_temp+1)) /*adding the symbol value to the memory, if the symbol don't exist, returns 0*/
                {
                    error_flag=TRUE;
                    fprintf(stderr,"Assembler: Using undefined symbol (line %d)\n",line_cnt_tmp);
                }

                IC_temp+=L;
            }/*end of one operand opcodes*/

            else /*two operand op codes*/
            {
                L=3;

                token = strtok(current_line," \t");
                if(line_flag)
                    token = strtok(NULL," \t");

                token = strtok(NULL,","); /*getting the first operand*/
                
                if NOT_OK_CHAR(token){ /*found an error in the second pass*/
                    L--;
                    continue;
                }

                src_address = get_address_type(token);
                
                if(src_address == direct)
                {
                    while ( isspace(*token) ) /*skipping the spaces*/
                    token++;

                    if(! add_symbol_value(token,IC_temp+1)) /*adding the symbol value to the memory, if the symbol don't exist, returns 0*/
                    {
                        error_flag=TRUE;
                        fprintf(stderr,"Assembler: Using undefined symbol (line %d)\n",line_cnt_tmp);
                    }
                }

                token = strtok(NULL," \t\n");
                des_address = get_address_type(token);

                if NOT_OK_CHAR(token){ /*found an error in the second pass*/
                    L--;
                    continue;
                }

                if( des_address==register_indirect || des_address==register_direct ){
                    if( src_address==register_indirect || src_address==register_direct )
                        L--; /*if both operands has registers we use same word to define them*/
                }
                
                else if( des_address == direct)
                {
                    while ( isspace(*token) ) /*skipping the spaces*/
                    token++;

                    if(! add_symbol_value(token,IC_temp+2)) /*adding the symbol value to the memory, if the symbol don't exist, returns 0*/
                    {
                        error_flag=TRUE;
                        fprintf(stderr,"Assembler: Using undefined symbol (line %d)\n",line_cnt_tmp);
                    }
                }

                /*if the address type is immidite, no changes need to be done*/


                IC_temp+=L; /*updating the IC*/

            } /*end of two operand opcodes*/
            

        }/*end of instruction line*/
        

    } /*end of while*/

    free(current_line);


    return 1;
}
