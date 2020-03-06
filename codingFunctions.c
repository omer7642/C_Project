#include "codingFunctions.h" 


/*
    'code_data':  This function recives a line from the source code, The type of this line (code/data),
    and a flag that represent if there is a decleration of symbol in the line.
    the function decoding the code line to binary and implementing the data in the data_emory.
    The function Increasing the Data Counter by need, and printing an error to the standart output in error situations.
*/

void code_data(char *line,enum line_type type,int symbol_flag)
{
    char *token,c,str_num[MAX_NUM_LENGTH];
    int count=0,j=0,i=0,length,L,num;
    char *temp_line = (char *)malloc(MAX_LINE);
    
    strcpy(temp_line,line);

    token = strtok(temp_line," "); //Extracting the command from the line (.data / .string)
    if(symbol_flag)
        token = strtok(NULL," "); //if there is a symbol so the previous instraction extracted the symbol and now we extract the command
    

    if(type == string)
    {
        token = strtok(NULL,"\0"); //getting the string from the line
        L = strlen(token);
        
        if(token[0]!='\"' && token[L-1]!='\"') //cheking syntax of string
        {
            error_flag=1;
            fprintf(stdout,"assembler: error - missing quotation marks (line %d)\n",line_counter);
            return;
        }
        
        EXIT_IF_RUNOUT_MEMORY

        i=0;
        while (i<L)
        {
            if(token[i]<MIN_VISABLE_ASCII || token[i] >MAX_VISABLE_ASCII)
            {
                error_flag=1;
                fprintf(stdout,"assembler: Invalid character in .string (line %d)\n",line_counter);
                return;
            } 
           data_memory[DC+i] = token[i++]; //Inserting the 
        }

        data_memory[i]='\0';
        DC += L+END_CHAR_SPACE;
        
    }
    else{ //type == data
        
        token = strtok(NULL,"\0");
        length = strlen(token);
        
        if(!token){ //in case this no parameters.
            error_flag=1;
            fprintf(stdout,"assembler: error - Missing Parametres in '.data' (line %d)\n",line_counter);
            return;
        }

        i=0;
        for(i=0;i<length;i++) //scanning the line and each comma present a number (Exepct the last number)
            if(token[i] == ',')
                count++;
        count++;//Increasing for the last number.

    
        EXIT_IF_RUNOUT_MEMORY

        i=0;                     
        for(L=1;L<=count;L++){  //i - index of token ,  j -loop variable index of str_num , L - increasing each time a number scanned without problem
            int j=0;
           
            while(isspace(token[i])) //skipping spaces
                i++;

            if(!isdigit(token[i])) //cheching for sign in the start of the number
            {
                if(token[i]=='-' || token[i] =='+' ) //Vaild Sign for number
                    str_num[j++] = token[i++];
            
                else
                {
                    error_flag=1;
                    if(token[i]==',') //extra comma OR comma before the first number
                        fprintf(stdout,"assembler: error - Invaid comma in data-line (line %d)\n",line_counter);
                    else
                        fprintf(stdout,"assembler: error - Invaid parameter '%c' in data-line (line %d)\n",token[i++],line_counter);
                    return;
                }  
            }

            while(isdigit(token[i]))
            {
                str_num[j]=token[i];
                i++;
                j++;
            }
            str_num[j]= '\0'; //Finishing the string.
            
            //checking for comma
            while(isspace(token[i]))
                i++;
            
            if(token[i]!=',')
                if(L!=count){
                    error_flag=1;
                    L--; //the previous Increasing was for nothing (6 lines Above)
                    fprintf(stdout,"assembler: error - Missing comma (line %d)\n",line_counter);
                    return;
                }   
            i++;
            num = atoi(str_num); //Coverting the string to integer
            data_memory[DC+L] = num;
           
        
        }

        DC += L;
    
    } 
    free(temp_line);
}

////////////////   INSTRACTION //////////////////

void code_instraction(char *line,int command_ind,int symbol_flag)
{
    char *token, src_str[OPERAND_MAX_LENGTH],des_str[OPERAND_MAX_LENGTH];
    char *temp_line = (char *)malloc(MAX_LINE);
    int number,L=0,i=0;
    enum address_type src_address, des_address;
    unsigned mask = 1;

    EXIT_IF_RUNOUT_MEMORY
    strcpy(temp_line,line);

    printf("\ncoding instractin - LINE =  %s\n",line);

    switch (command_ind)
    {
        /*  Two operands opcodes*/
        case mov:
        case cmp:
        case add:
        case sub:
        case lea:
                    printf("Case - Two opernd opcodes the current opcode is %s\n",opcodes[command_ind]);
                    memory[IC] = A; //decoding The A,R,E FIELD
                    memory[IC] |= (command_ind << OPCODE_SHIFT); //decoding the opcode
                    L = 3; //one word for the coding and another two for the two operands

                    token = strtok(temp_line," "); //skipping the first word
                    if(symbol_flag)
                        token = strtok(NULL," "); // if there is a symbol, skipping again.
                   
                    token = strtok(NULL," ,"); //getting out the source operand

                    if (!token) //in-case line with no opernad
                    {   
                        error_flag=1;
                        printf("assembly: Instraction %s Have No Operand (line %d)\n",opcodes[command_ind],line_counter);
                        return;
                    }

                        src_address = get_address_type(token);  //getting the address type of the source operand
                        if(src_address == ERROR)
                        {
                            error_flag =1;
                            fprintf(stderr,"assembly: Invaild address method (line %d)\n",line_counter);
                            return;
                        }
                        memory[IC] |= mask<<(SOURCE_SHIFT+src_address) ; //decoding the source address of the first word
                        if(command_ind == lea && src_address != direct)
                        {
                            error_flag=1;
                            printf("assembly: Instraction %s can't accept this address type (line %d)\n",opcodes[command_ind],line_counter);
                            return;
                        }
                        
                        switch(src_address)
                        {
                            case immediate:
                                while(token){  //skipping spaces and searching for the char '#'
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                                number = atoi(token+1);
                                if(number<0)
                                  number = complement_2(number); //if the number is minus need to convert it to 14-3 bits
                                memory[IC+1] = A;
                                memory[IC+1] |= (number << IMMIDIATE_SHIFT);
                                break;
                    
                            case direct:
                            // will get his value in the second pass
                                break;

                            case register_indirect:
                                memory[IC+1] = A;
                                while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                memory[IC+1] |= (*token - '0')<<(SOURCE_SHIFT-1); //need to be in bits 6-8
                                break;
                    
                            case register_direct:
                                memory[IC+1] = A;
                                while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                memory[IC+1] |= (*token - '0')<<(SOURCE_SHIFT-1); //need to be in bits 6-8
                                break;
                    
                        }//end of switch source

                    token = strtok(NULL," \n\t"); //getting out the target operand
                    des_address = get_address_type(token);  //getting the address type of the target operand
                    if(des_address == ERROR)
                        {
                            error_flag =1;
                            fprintf(stderr,"assembly: Invaild address method (line %d)\n",line_counter);
                            return;
                        }
                    memory[IC] |= mask<<(TARGET_SHIFT+des_address) ; //decoding the target address of the first word
                    switch(des_address)
                        {
                            case immediate:
                                if(command_ind != cmp) //only the compare method can accept this address type in target
                                {
                                    error_flag=1;
                                    printf("assembly: Instraction %s don't accept immidate address for target operand (line %d)\n",opcodes[command_ind],line_counter);
                                    return;   
                                }
                                while(token){  //skipping spaces and searching for the char '#'
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                                number = atoi(token+1);
                                if(number<0)
                                  number = complement_2(number); //if the number is minus need to convert it to 14-3 bits
                                memory[IC+2] = A;
                                memory[IC+2] |= (number << IMMIDIATE_SHIFT); //need to be in bits 3-14
                                break;
                    
                            case direct:
                            // will get his value in the second pass
                                break;

                            case register_indirect:
                                while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                /*if the source and the target operands are in register address type, there coding are in the same word*/
                                if(src_address == register_indirect || src_address == register_direct){
                                    memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                                    L--;
                                }
                                
                                else{
                                    memory[IC+2] = A;
                                    memory[IC+2] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                                }

                                break;
                    
                            case register_direct:
                                while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                /*if the source and the target operands are in register address type, there coding are in the same word*/
                                if(src_address == register_indirect || src_address == register_direct){
                                    memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                                    L--;
                                }
                                
                                else{
                                    memory[IC+2] = A;
                                    memory[IC+2] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                                }

                                break;
                    
                        }//end of switch target
                    IC+=L; //updading IC                              
                    break; //end of two operand opcodes

        /*  One operand opcodes*/        
        case clr:
        case not:
        case inc:
        case dec:
        case jmp:
        case bne:
        case red:
        case prn:
        case jsr:
                {
                    printf("Case - One opernd opcodes \n");
                    memory[IC] = A; //decoding The A,R,E FIELD
                    memory[IC] |= (command_ind << OPCODE_SHIFT); //decoding the opcode
                    L = 2; //one word for the coding and another one for the target operands

                    token = strtok(temp_line," \t"); //skipping the first word
                    if(symbol_flag)
                        token = strtok(NULL," \t"); // if there is a symbol, skipping again.

                    token = strtok(NULL," \t\n"); //getting the target operand

                    if (!token) //in-case line with no opernad
                    {   
                        error_flag=1;
                        printf("assembly: Instraction %s Have No Operand (line %d)\n",opcodes[command_ind],line_counter);
                        return;
                    }

                    des_address = get_address_type(token);
                    if(des_address == ERROR)
                        {
                            error_flag =1;
                            fprintf(stderr,"assembly: Invaild address method (line %d)\n",line_counter);
                            return;
                        }
                    memory[IC] |= mask<<(TARGET_SHIFT+des_address) ; //decoding the target address of the first word

                    switch(des_address)
                    {
                        case immediate:
                            if(command_ind != prn)
                            {
                                error_flag=1;
                                fprintf(stdout,"assembly: Invaild address method (line %d)\n",line_counter);
                                return;
                            }
                            while(token){  //skipping spaces and searching for the char '#'
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                            number = atoi(token+1);
                            if(number<0)
                                number = complement_2(number); //if the number is minus need to convert it to 14-3 bits
                            memory[IC+1] = A;
                            memory[IC+1] |= (number << IMMIDIATE_SHIFT); //need to be in bits 3-14
                            break;

                        
                        case  direct:
                            //will get his value in the second pass
                            break;

                        case register_indirect:
                            while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                            memory[IC+1] = A;
                            memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                            break;

                        case register_direct:
                            if(command_ind==jmp || command_ind==bne || command_ind==jsr)
                            {
                                error_flag=1;
                                fprintf(stdout,"assembly: Invaild address method (line %d)\n",line_counter);
                                return;
                            }

                            while(token){    //seraching the number of the register
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                            memory[IC+1] = A;
                            memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); //need to be in bits 3-5
                            break;

                    } //end of address switch

                    IC+=L;
                    break; //end of one operand opcodes
                }
        /*  No operands opcodes*/
        case rts:
        case stop:
                {
                    printf("Case - NO opernd opcodes \n");
                    memory[IC] = A; //decoding The A,R,E FIELD
                    memory[IC] |= (command_ind << OPCODE_SHIFT); //decoding the opcode
                    L = 1; //one word for the coding

                    token = strtok(temp_line," \t"); //skipping the first word
                    if(symbol_flag)
                        token = strtok(NULL," \t"); // if there is a symbol, skipping again.
                    
                    token = strtok(NULL," \t\n");
                    while(token)
                    {  
                        if(!isspace(*token++))
                        {
                            error_flag =1;
                            fprintf(stdout,"assembly: Invaild operand in no-operand command (line %d)\n",line_counter);
                            return;
                        }
                    }
                    IC+=L;
                    break; //end of no-operand switch
                }

        default:
            error_flag=1;
            fprintf(stdout,"assembly: Invaild opcode (line %d)\n",line_counter);
            return;

    } // end of opcodes switch


     free(temp_line);
} // end of function code_instraction





