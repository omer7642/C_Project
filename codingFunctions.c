#include "codingFunctions.h" 


/*
    'code_data':  This function recives a line from the source code, The type of this line (code/data),
    and a flag that represent if there is a decleration of symbol in the line.
    the function decodes the code line to binary and implements the data in the data_memory.
    The function increases the Data Counter by need, and prints an error to the standard output in error situations.
*/
void code_data(char *line,enum line_type type,int symbol_flag)
{
    char *token,str_num[MAX_NUM_LENGTH]; /*the data tokens we get from the line*/
    int count=0,j=0,i=0,length,L,num;  /*count - num of commas, i and j indexes of iteration, length is used as the length of the string in data, L as the l
                                        length of the string in string and also for numbers that are scanned with no issues, num is the result we get in data
                                        after turning the number from string to int*/
    char *temp_line = (char *)malloc(MAX_LINE); /*use it to not make changes to original line*/
    
    strcpy(temp_line,line);

    token = strtok(temp_line," "); /*Extracting the command from the line (.data / .string)*/
    if(symbol_flag)
        token = strtok(NULL," "); /*if there is a symbol so the previous instruction extracted the symbol and now we extract the command*/
            

    if(type == string)
    {
        token = strtok(NULL,"\0"); /*getting the string from the line*/
        L = strlen(token);
        

        if(token[0]!='\"' || token[L-1]!='\"') /*cheking syntax of string*/
        {
            error_flag=TRUE;
            fprintf(stderr,"Assembler: error - missing quotation marks (line %d)\n",line_counter);
            return;
        }
        
        EXIT_IF_RUNOUT_MEMORY

        i=1;j=0;
        while (token[i]!='\"')
        {
            if(token[i]<MIN_VISABLE_ASCII || token[i] >MAX_VISABLE_ASCII) /*of the string has a non ASCII character*/
            {
                error_flag=TRUE;
                fprintf(stderr,"Assembler: Invalid character in .string (line %d)\n",line_counter);
                return;
            } 
           data_memory[DC+j] = token[i]; /*Inserting the character into data_memory*/
           i++; j++;
        }

        data_memory[j]='\0'; /*adds \0 at the end of the string*/
        DC += L+END_CHAR_SPACE-2; /*the data counter increases by the length of the string, minus 2 for quotation marks and plus 1 for the \0*/
        
    }
    else{ /*type == data*/
        
        token = strtok(NULL,"\0"); /*data eneterd as a token*/
        length = strlen(token);  

        if(!token){ /*in case there are no parameters.*/
            error_flag=TRUE;
            fprintf(stderr,"Assembler: error - Missing Parametres in '.data' (line %d)\n",line_counter);
            return;
        }

        i=0;
        for(i=0;i<length;i++)
        { /*scanning the line and each comma presents a number (Exepct the last number)*/
            if(i!=length-1)
                if(token[i]==' ' && isdigit(token[i+1]))
                    count++;
                    
            if(token[i] == ',')
                count++;
        }
        count++;/*Increasing for the last number.*/

    
        EXIT_IF_RUNOUT_MEMORY

        i=0;                     
        for(L=0;L<count;L++){  /*i - index of token ,  j -loop variable index of str_num , L - increasing each time a number is being scanned without problem*/
            int j=0;
           
            while(isspace(token[i])) /*skipping spaces*/
                i++;

            if(!isdigit(token[i])) /*cheching for sign in the start of the number*/
            {
                if(token[i]=='-' || token[i] =='+' ) /*Vaild Sign for number*/
                    str_num[j++] = token[i++];
            
                else
                {
                    error_flag=TRUE;
                    if(token[i]==',') /*extra comma OR comma before the first number*/
                        fprintf(stderr,"Assembler: error - Invaid comma in data-line (line %d)\n",line_counter);
                    else
                        fprintf(stderr,"Assembler: error - Invaid parameter '%c' in data-line (line %d)\n",token[i++],line_counter);
                    return;
                }  
            }

            while(isdigit(token[i]))/*read the digits from the string into str_num*/
            {
                str_num[j]=token[i];
                i++;
                j++;
            }
            str_num[j]= '\0'; /*Finishing the string with \0.*/
            
            /*checking for comma*/
            while(isspace(token[i]))
                i++;
            
            if(token[i]!=',')
                if(L!=count-1)
                {  
                    error_flag=TRUE;
                    L--; /*the previous Increasing was for nothing (6 lines Above)*/
                    
                    while(isspace(token[i])) /*skip whitespaces*/
                        i++;
                    if(isdigit(token[i])) 
                        fprintf(stderr,"Assembler: error - Missing comma (line %d)\n",line_counter);
                    else
                        fprintf(stderr,"Assembler: error - Invaild char (line %d)\n",line_counter);

                    return;
                }   
            i++;

            /*Coverting the string to integer*/
            if(*str_num == '-'){ /*for negative numbers*/
                num = atoi(str_num+1);
                num *= (-1);
            }
            else /*positive number*/
            {
                num = atoi(str_num);
            }

             /*In case the number is too big/small to be represnt in 15 bits */
            if( num>MAX_VALUE_DATA || num<MIN_VALUE_DATA )
                fprintf(stderr,"Warning: Data loss - number %d is out of limit. (line %d) \n",num,line_counter);
            
             
            data_memory[DC+L] = num; /*add the converted number to data_memory*/
            data_memory[DC+L] &=  ~( 1<<( (sizeof(short) * BYTE)-1 ) ); /* Turn off the last bit - not in use in the computers memory */
           
        
        }

        DC += L;
    
    } 
    /*free variables*/
    free(temp_line);
}

/*////////////////   INSTRUCTION //////////////////*/

/*code instruction receives an instruction line from the source code , the index of of the command or opcode, and the line flag aka symbol flag, which tells if
a symbol was declared in the line. it then divides into 3 different situations, whether the opcode has two operands, one operand and no operands at all.
then it checks for the addressing type. in the end it decodes the data into memory*/

void code_instruction(char *line,int command_ind,int symbol_flag)
{
    char *token;
    char *temp_line = (char *)malloc(MAX_LINE); /*the token we use to not alter the current line with strtok*/
    int number,L=0;
    enum address_type src_address, des_address;
    unsigned mask = 1; /*a mas later to be used for bitwise operations and decoding*/

    EXIT_IF_RUNOUT_MEMORY
    strcpy(temp_line,line);


    switch (command_ind)
    {
        /*  Two operands opcodes*/
        case mov:
        case cmp:
        case add:
        case sub:
        case lea:
                    memory[IC] = A; /*decoding The A,R,E FIELD*/
                    memory[IC] |= (command_ind << OPCODE_SHIFT); /*decoding the opcode*/
                    L = 3; /*one word for the coding and another two for the two operands*/

                    token = strtok(temp_line," "); /*skipping the first word*/
                    if(symbol_flag)
                        token = strtok(NULL," "); /* if there is a symbol, skipping again.*/
                   
                    token = strtok(NULL," ,"); /*getting out the source operand*/

                    if (!token) /*in-case line with no opernad*/
                    {   
                        error_flag=TRUE;
                        fprintf(stderr, "Assembler: Instruction Have No Operand (line %d)\n",line_counter);
                        return;
                    }

                        src_address = get_address_type(token);  /*getting the address type of the source operand*/
                        if(src_address == ERROR)
                        {
                            error_flag =TRUE;
                            return;
                        }
                        memory[IC] |= mask<<(SOURCE_SHIFT+src_address) ; /*decoding the source address of the first word*/
                        if(command_ind == lea && src_address != direct)
                        {
                            error_flag=TRUE;
                            fprintf(stderr, "Assembler: Instruction can't accept this address type (line %d)\n",line_counter);
                            return;
                        }
                        
                        switch(src_address)
                        {
                            case immediate:
                                while(token){  /*skipping spaces and searching for the char '#'*/
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                                number = atoi(token+1);

                                /*In case the number is too big/small to be represnt in 11 bits */
                                if( number>MAX_VALUE_IMMIDIATE || number<MIN_VALUE_IMMIDIATE )
                                    fprintf(stderr,"Warning: Data loss - number %d is out of limit. (line %d) \n",number,line_counter);

                                if(number<0)
                                  number = complement_2(number); /*if the number is negative need to convert it to 14-3 bits*/
                                
                                memory[IC+1] = A;
                                memory[IC+1] |= (number << IMMIDIATE_SHIFT);
                                memory[IC+1] &=   ~( 1<<( (sizeof(short) * BYTE)-1 ) ); /* Turn off the last bit - not in use in the computers memory*/
                                break;
                    
                            case direct:
                            /*will get his value in the second pass*/
                                break;

                            case register_indirect:
                                memory[IC+1] = A;
                                while(token){    /*seraching the number of the register*/
                                    if(isdigit(*token) )
                                        break;
                                    token++;
                                }
                                memory[IC+1] |= (*token - '0')<<(SOURCE_SHIFT-1); /*need to be in bits 6-8*/
                                break;
                    
                            case register_direct:
                                memory[IC+1] = A;
                                while(token){    /*seraching the number of the register*/
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                memory[IC+1] |= (*token - '0')<<(SOURCE_SHIFT-1); /*need to be in bits 6-8*/
                                break;
                    
                        }/*end of switch source*/

                    token = strtok(NULL," \n\t"); /*getting out the target operand*/
                    des_address = get_address_type(token);  /*getting the address type of the target operand*/
                    if(des_address == ERROR)
                        {
                            error_flag =TRUE;
                            return;
                        }
                    memory[IC] |= mask<<(TARGET_SHIFT+des_address) ; /*decoding the target address of the first word*/
                    switch(des_address)
                        {
                            case immediate:
                                if(command_ind != cmp) /*only the compare method can accept this address type in target*/
                                {
                                    error_flag=TRUE;
                                    fprintf(stderr, "Assembler: Instruction don't accept immidate address for target operand (line %d)\n",line_counter);
                                    return;   
                                }
                                while(token){  /*skipping spaces and searching for the char '#'*/
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                                number = atoi(token+1);

                                /*In case the number is too big/small to be represnt in 11 bits */
                                if( number>MAX_VALUE_IMMIDIATE || number<MIN_VALUE_IMMIDIATE )
                                    fprintf(stderr,"Warning: Data loss - number %d is out of limit. (line %d) \n",number,line_counter);

                                if(number<0)
                                  number = complement_2(number); /*if the number is minus need to convert it to 14-3 bits*/
                                  
                                memory[IC+2] = A;
                                memory[IC+2] |= (number << IMMIDIATE_SHIFT); /*need to be in bits 3-14*/
                                memory[IC+2] &=   ~( 1<<( (sizeof(short) * BYTE)-1 ) ); /* Turn off the last bit - not in use in the computers memory*/
                                break;
                    
                            case direct:
                            /* will get his value in the second pass*/
                                break;

                            case register_indirect:
                                while(token){    /*seraching the number of the register*/
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                /*if the source and the target operands are in register address type, there coding are in the same word*/
                                if(src_address == register_indirect || src_address == register_direct){
                                    memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                                    L--;
                                }
                                
                                else{
                                    memory[IC+2] = A;
                                    memory[IC+2] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                                }

                                break;
                    
                            case register_direct:
                                while(token){    /*seraching the number of the register*/
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                                /*if the source and the target operands are in register address type, there coding are in the same word*/
                                if(src_address == register_indirect || src_address == register_direct){
                                    memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                                    L--;
                                }
                                
                                else{
                                    memory[IC+2] = A;
                                    memory[IC+2] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                                }

                                break;
                    
                        }/*end of switch target*/
                    IC+=L; /*updading IC*/                              
                    break; /*end of two operand opcodes*/

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
                    memory[IC] = A; /*decoding The A,R,E FIELD*/
                    memory[IC] |= (command_ind << OPCODE_SHIFT); /*decoding the opcode*/
                    L = 2; /*one word for the coding and another one for the target operands*/

                    token = strtok(temp_line," \t"); /*skipping the first word*/
                    if(symbol_flag)
                        token = strtok(NULL," \t"); /*if there is a symbol, skipping again.*/

                    token = strtok(NULL," \t\n"); /*getting the target operand*/

                    if (!token) /*in-case line with no opernad*/
                    {   
                        error_flag=TRUE;
                        fprintf(stderr, "Assembler: Instruction Have No Operand (line %d)\n",line_counter);
                        return; 
                    }

                    des_address = get_address_type(token);
                    if(des_address == ERROR)
                        {
                            error_flag =TRUE;
                            return;
                        }
                    memory[IC] |= mask<<(TARGET_SHIFT+des_address) ; /*decoding the target address of the first word*/

                    switch(des_address)
                    {
                        case immediate:
                            if(command_ind != prn)
                            {
                                error_flag=TRUE;
                                fprintf(stderr,"Assembler: Invaild address method (line %d)\n",line_counter);
                                return;
                            }
                            while(token){  /*skipping spaces and searching for the char '#'*/
                                    if(*token == '#')
                                        break;
                                    token++;
                                }
                            number = atoi(token+1);
                            
                            /*In case the number is too big/small to be represnt in 11 bits */
                            if( number>MAX_VALUE_IMMIDIATE || number<MIN_VALUE_IMMIDIATE )
                                fprintf(stderr,"Warning: Data loss - number %d is out of limit. (line %d) \n",number,line_counter);

                            if(number<0)
                                number = complement_2(number); /*if the number is minus need to convert it to 14-3 bits*/

                            memory[IC+1] = A;
                            memory[IC+1] |= (number << IMMIDIATE_SHIFT); /*need to be in bits 3-14*/
                            memory[IC+1] &=   ~( 1<<( (sizeof(short) * BYTE)-1 ) ); /* Turn off the last bit - not in use in the computers memory*/
                            break;

                        
                        case  direct:
                            /*will get his value in the second pass*/
                            break;

                        case register_indirect:
                            while(token){    /*seraching the number of the register*/
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                            memory[IC+1] = A;
                            memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                            break;

                        case register_direct:
                            if(command_ind==jmp || command_ind==bne || command_ind==jsr)
                            {
                                error_flag=TRUE;
                                fprintf(stderr,"Assembler: Invaild address method (line %d)\n",line_counter);
                                return;
                            }

                            while(token){    /*seraching the number of the register*/
                                    if( isdigit(*token) )
                                        break;
                                    token++;
                                }
                            memory[IC+1] = A;
                            memory[IC+1] |= (*token - '0')<<(TARGET_SHIFT); /*need to be in bits 3-5*/
                            break;

                    } /*end of address switch*/

                    IC+=L;
                    break; /*end of one operand opcodes*/
                }
        /*  No operands opcodes*/
        case rts:
        case stop:
                {
                    memory[IC] = A; /*decoding The A,R,E FIELD*/
                    memory[IC] |= (command_ind << OPCODE_SHIFT); /*decoding the opcode*/
                    L = 1; /*one word for the coding*/

                    token = strtok(temp_line," \t"); /*skipping the first word*/
                    if(symbol_flag)
                        token = strtok(NULL," \t"); /*if there is a symbol, skipping again.*/
                    
                    token = strtok(NULL," \t\n");
                    while(token)
                    {  
                        if(!isspace(*token++))
                        {
                            error_flag =TRUE;
                            fprintf(stderr,"Assembler: Invaild operand in no-operand command (line %d)\n",line_counter);
                            return;
                        }
                    }
                    IC+=L;
                    break; /*end of no-operand switch*/
                }

        default:
            error_flag=TRUE;
            fprintf(stderr,"Assembler: Invaild opcode (line %d)\n",line_counter);
            return;

    } /*end of opcodes switch*/


     free(temp_line);
} 





