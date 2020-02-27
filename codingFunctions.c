#include "codingFunctions.h" 


/*
    'code_data':  This function recives a line from the source code, The type of this line (code/data),
    and a flag that represent if there is a decleration of symbol in the line.
    the function decoding the code line to binary and implementing the data in the data_emory.
    The function Increasing the Data Counter by need, and printing an error to the standart output in error situations.
*/

void code_data(char *line,enum line_type type,int symbol_flag)
{
    char *token,c,str_num[MAX_NUM_SIZE];
    int count,j,i,length,L,num;

    token = strtok(line," \t"); //Extracting the command from the line (.data / .string)
    if(symbol_flag)
        token = strtok(NULL," \t"); //if there is a symbol so the previous instraction extracted the symbol and now we extract the command
    

    if(type == string)
    {
        token = strtok(NULL," \t"); //getting the string from the line
        L = strlen(token);
        
        if(token[0]!="\"" || token[L-1]!="\"") //cheking syntax of string
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
        
        token = strtok(NULL,"\n");
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
                if(L!=count-1){
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
}

////////////////   INSTRACTION //////////////////

void code_instraction(char *line,int command_ind,int symbol_flag)
{
    char *token, src_str[OPERAND_MAX_LENGTH],des_str[OPERAND_MAX_LENGTH];
    int number,L=0;
    unsigned mask = 1;

    EXIT_IF_RUNOUT_MEMORY

    switch (command_ind)
    {
        /*  Two operands opcodes*/
        case mov:
        case cmp:
        case add:
        case sum:
                {
                     memory[IC] = A; //decoding The A,R,E FIELD
                     memory[IC] |= (command_ind << OPCODE_SHIFT); //decoding the opcode

                    token = strtok(line," \t"); //skipping the first word
                    if(symbol_flag)
                        token = strtok(NULL," \t"); // if there is a symbol, skipping again.

                    token = strtok(NULL," \t"); //getting out the source operand

                    if NOT_OK_CHAR(token) //incase line with no opernad
                    {   
                        error_flag=1;
                        printf("assembly: Instraction %s No Operand (line %d)\n",opcodes[command_ind],line_counter);
                        return;
                    }

                    if( *token == '#') // source imidiate 
                    {   
                        memory[IC]|= (mask << (immediate+SOURCE_SHIFT)); //decoding the miun method
                        
                        if(!isdigit(token+1))
                        {
                            error_flag =1;
                            printf("assembly: Instraction %s Invaild number after-# (line %d)\n",opcodes[command_ind],line_counter);
                            return;
                        }
                        // I need a function that covnvet the number to minus because if i will implement the number regulary,
                        // i will nedd to shift it left and i will lose numbers. - So Function that convert number to minus with 2 complemnt on 14-3 bits
                        number = atoi(token+1);
                        L++;
                         //decoding the word for the operand
                        memory[IC+L] = A; 
                        memory[IC+L] |= (number << IMMIDIATE_SHIFT);
                        //I need to check the MIUN methods and then create a function that recive the all the information and do the work for me
                        //After that i need to decode the word for the operand and move to the second opernd
                        
                    }
                                                                  

                    break;
                }
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


                    break;
                }
        /*  No operands opcodes*/
        case rts:
        case stop:
                {


                    break;
                }

    }



}





