#include "passes.h" 

short IC;
short DC;
int line_counter;
unsigned char error_flag,second_pass_flag;

void first_pass(FILE *fp, char *file_name)
{
    char line_flag; /*tells if the line has a symbol decleration*/
    unsigned char command; /*index of the command*/
    enum line_type current_type; /*type of the line (instruction,data,string,entry,external)*/
    char *current_line = (char *)malloc(MAX_LINE); /*holds the current command line*/
    char *symbol_name = (char *)malloc(SYMBOL_MAX_LENGTH); /*holds the current symbol name (if there is one)*/
    
    
    IC=0;
    DC=0;
    line_counter=0; 


    while( (line_flag = get_line(fp,current_line))!= EOF)
    {    
        line_counter++;
        if(line_flag == EMPTY_LINE)
            continue;
        
        current_type = check_type(current_line);   /*checking the type of the current line : 0-code,1-data,2-string,3-entry,4-extern*/
        if(current_type)
        { 
            if(line_flag){ /*if symbol is declared in this line*/
                get_symbol(current_line,symbol_name);
                if(symbol_name == NULL || !(*symbol_name))
                {
                    error_flag = 1;
                    fprintf(stderr,"assembly: Invaild symbol name (line %d)\n",line_counter);
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
                    error_flag = 1;
                    fprintf(stderr,"assembly: Invaild symbol name (line %d)\n",line_counter);
                } 
                add_symbol(symbol_name,IC+LOAD_SPACE,code);
            }
            command = get_command(current_line,line_flag); /*if -1 error, else return the index of the command.*/
            code_instraction(current_line,command,line_flag); /*getting the line, Instruction ,the command index, and line flag - If there is a symbol or not*/
            
        }
   
    }/*end of while*/
    /*free the dynamic memory the we used in the function*/
        free(current_line);
        free(symbol_name);
    
    for(int i=IC ,j=0 ; j<DC ; j++,i++) /*merging the two data structures;*/
        memory[i] = data_memory[j];
    update_data_symbol();/*adding to all data symbol the value of IC+100 because the merging of the data structures*/

    
    return;
}


int second_pass(FILE *fp, char *file_name)
{
    int IC_temp=0 ,i=0,j=0,line_cnt_tmp=0, L;
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
            error_flag =1;
            fprintf(stderr,"assembly: returned EOF from the source file in the second pass \n");
            return 0;
        }

        line_cnt_tmp++;
        current_type = check_type(current_line);   /*checking the type of the current line : 0-code,1-data,2-string,3-entry,4-extern*/
        strcpy(temp_line,current_line);


        if(current_type)
        {

            if (current_type == entry)
            {
                
                token = strtok(temp_line," \t");
                if(line_flag)
                    token = strtok(NULL," \t");

                token = strtok(NULL," \t\n"); /*getting the name of the symbol;*/

                if NOT_OK_CHAR(token)
                {
                    error_flag = 1;
                    fprintf(stderr,"assembly: entry line without arguments (line %d) \n",line_cnt_tmp);
                    continue;
                }

                if (!add_entry(token)) /*if the symbol not exist in the symbol_table*/
                {
                    error_flag =1;
                    fprintf(stderr,"assembly: non existing symbol in entry line (line %d)\n",line_cnt_tmp);
                }

            }
            else /*no need to deal with another data lines in the second pass*/
                continue;
        }

        else /*instruction line*/
        {
            command = get_command(current_line, line_flag);

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
                    error_flag=1;
                    fprintf(stderr,"assembly: Using undefined symbol (line %d)\n",line_cnt_tmp);
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
                        error_flag=1;
                        fprintf(stderr,"assembly: Using undefined symbol (line %d)\n",line_cnt_tmp);
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
                        L--; /*the two operand using the same memory word*/
                }
                
                else if( des_address == direct)
                {
                    while ( isspace(*token) ) /*skipping the spaces*/
                    token++;

                    if(! add_symbol_value(token,IC_temp+2)) /*adding the symbol value to the memory, if the symbol don't exist, returns 0*/
                    {
                        error_flag=1;
                        fprintf(stderr,"assembly: Using undefined symbol (line %d)\n",line_cnt_tmp);
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
