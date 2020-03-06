#include "input.h"

char get_line(FILE *fp, char *current_line)
{
	char c; /*current char read from the file*/
	char isDeclaration = FALSE; /*a flag that will eventually say is the line has a symbol declared in it*/
	char isEmpty = TRUE; /*if the line isn't empty, this flag will change*/
	int writeIndex = 0; /*the index which writes of the line inside current_line*/
	while (( (c = fgetc(fp)) != '\n')  && (c != EOF))/*we stop receiving input either at the end of a
																		  non empty line, or at the end of the file*/
	{
		if (isspace(c)) /*if a whitespace, see if a whitespace is neede to be added artificially and reset the loop*/
		{
			if (!isEmpty && current_line[writeIndex - 1] != ' ') /*if we already added*/
				current_line[writeIndex++] = ' ';
		}

		else if (c == ';') /*if its a comment line, count it and skip to the end of it*/
			while ((c = fgetc(fp)) != '\n'); /*go through the line*/

		else /*if not a space or a comment line, add it to the line*/
		{
			if (c == ':') /*if ':' shows up , a symbol is declared probably and will be treated as such*/
				isDeclaration = TRUE;
			isEmpty = FALSE;
			current_line[writeIndex++] = c;
		}
	}
	current_line[writeIndex] = '\0';
	if (c == EOF && isEmpty) /*if the file has reached the end*/
		return EOF;
	
	if (isEmpty)
		return 2;

	return isDeclaration;
}

enum line_type check_type(char *current_line)
{
    char *temp = (char *)malloc(sizeof(char) * strlen(current_line)); //instead of changing the original line, do all changes on a temp string
    strcpy(temp, current_line); //copy current_line into temp to avoid changing current_line
    char *token = strtok(temp, " "); //get first token

    while(token != NULL)
    {
        if(!strcmp(token, ".extern")) //if extern, exit function and returns external
            return external;

        else if(!strcmp(token, ".entry")) //if entry, exit and return entry
            return entry;
        
        else if(!strcmp(token, ".data")) //if data, exit and return data
            return data;
        
        else if (!strcmp(token, ".string")) //if string, exit and return string
            return string;

        token = strtok(NULL, " "); //look for the next word if none was found
    }
    //if nothing was found, free temp and treat the line as a code line - will later need to extract opcode
    free(temp);
    return code;
}

int get_command(char *current_line,unsigned char line_flag)
{
    char *command = (char *)malloc(sizeof(char) * (MAX_COMMAND_LENGTH + 1)); 
    int i=0; //the index we use to access current_line
    int j=0; //the index we use on command
    if(line_flag) //if a symbol is declared in the line, skip the symbol and read command
        while(current_line[i++] != ' '); //skip symbol declaration, position i on the index of the first char in the opcode

    //get the command into the string "command"
    while(current_line[i] != ' ')
        command[j++] = current_line[i++]; //copy the command between the two spaces or until the first space is encounters
    command[j] = '\0'; //add '\0' at the end of the command string

    //return the correct opcode, or an error
    if(!strcmp(command, "mov"))
        return mov;
    else if(!strcmp(command, "cmp"))
        return cmp;
    else if(!strcmp(command, "add"))
        return add;
    else if(!strcmp(command, "sub"))
        return sub;
    else if(!strcmp(command, "lea"))
        return lea;
    else if(!strcmp(command, "clr"))
        return clr;
    else if(!strcmp(command, "not"))
        return not;
    else if(!strcmp(command, "inc"))
        return inc;
    else if(!strcmp(command, "dec"))
        return dec;
    else if(!strcmp(command, "jmp"))
        return jmp;
    else if(!strcmp(command, "bne"))
        return bne;
    else if(!strcmp(command, "red"))
        return red;
    else if(!strcmp(command, "prn"))
        return prn;
    else if(!strcmp(command, "jsr"))
        return jsr;
    else if(!strcmp(command, "rts"))
        return rts;
    else if(!strcmp(command, "stop"))
        return stop;
    else //if no opcode found, throw an error
    {
        error_flag = TRUE;
        fprintf(stdout, "Unknown opcode entered on line %d", line_counter);
        return ERROR_SIGN;
    }
}

char *get_symbol(char *current_line,char *symbol_name)
{
    int i = 0,j=0; //index of iteration
    char c;
    while(isspace(current_line[i]))
        i++;

    j=i;

    while((i <= SYMBOL_MAX_LENGTH) && (c = current_line[i] != ':'))
    {
        if(i == j)
        {
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) //if first character is a letter
                symbol_name[i] = c;
            else //if first char isn't a letter
            {
                error_flag = TRUE;
                symbol_name = "\n\0";
                fprintf(stdout, "invalid symbol name on line %d", line_counter);
                return NULL;
            }
        }

        else //if not in first place
        {
            
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) //if a letter or a number
                symbol_name[i] = c;
            
            else //if neither a letter nor a number
            {
                error_flag = TRUE;
                symbol_name = "\n\0";
                fprintf(stdout, "invalid symbol name on line %d", line_counter);
                return NULL;
            }
        }
        i++;        //advance the index by 1
    }
    
    //if we come this far without an error, and is a valid - meaning not a saved phrase, return it
    if(isSavedPhrase(symbol_name))
    {
        error_flag = TRUE;
        fprintf(stdout, "declared a symbol that's a reserved phrase in line %d", line_counter);
        return NULL;
    }

    else //if not a reserved phrase, then it's a legal symbol and thus can be returned
        return symbol_name;
}

BOOL isSavedPhrase(char *symbol_name)
{
    //checks each saved phrase
    if(!strcmp(symbol_name, "r0"))
        return TRUE;
    else if(!strcmp(symbol_name, "r1"))
        return TRUE;
    else if(!strcmp(symbol_name, "r2"))
        return TRUE;
    else if(!strcmp(symbol_name, "r3"))
        return TRUE;
    else if(!strcmp(symbol_name, "r4"))
        return TRUE;
    else if(!strcmp(symbol_name, "r5"))
        return TRUE;
    else if(!strcmp(symbol_name, "r6"))
        return TRUE;
    else if(!strcmp(symbol_name, "r7"))
        return TRUE;
    else if(!strcmp(symbol_name, "mov"))
        return TRUE;
    else if(!strcmp(symbol_name, "cmp"))
        return TRUE;
    else if(!strcmp(symbol_name, "add"))
        return TRUE;
    else if(!strcmp(symbol_name, "sub"))
        return TRUE;
    else if(!strcmp(symbol_name, "lea"))
        return TRUE;
    else if(!strcmp(symbol_name, "clr"))
        return TRUE;
    else if(!strcmp(symbol_name, "not"))
        return TRUE;
    else if(!strcmp(symbol_name, "inc"))
        return TRUE;
    else if(!strcmp(symbol_name, "dec"))
        return TRUE;
    else if(!strcmp(symbol_name, "jmp"))
        return TRUE;
    else if(!strcmp(symbol_name, "bne"))
        return TRUE;
    else if(!strcmp(symbol_name, "red"))
        return TRUE;
    else if(!strcmp(symbol_name, "prn"))
        return TRUE;
    else if(!strcmp(symbol_name, "jsr"))
        return TRUE;
    else if(!strcmp(symbol_name, "rts"))
        return TRUE;
    else if(!strcmp(symbol_name, "stop"))
        return TRUE;
    else
        return FALSE;
}



int get_address_type(char * operand)
{
    enum address_type curr_type; // we return it eventually, if all is well
    int i; //running index

    if(operand[0] == '#') // if immediate addressing
    {
        if (operand[1] == '-')
        {
            i = 2;
            while(operand[i] != '\0') //running through the string to make sure its all numbers
            {
                if(operand[i] < '0' || operand[i] > '9') //if not a number in decimal base, its an error and we throw it out with error flag
                {
                    error_flag = TRUE;
                    fprintf(stdout, "address type did not match operand type in line %d", line_counter);
                    return ERROR_SIGN;
                }
            }
        }
        curr_type = immediate;
    }
    
    

    else if(operand[0] == '*') 
    {
        if(operand[1] == 'r' && (operand[2] >= '0' && operand[2] <= '7')) //if a valid register name after the *
            curr_type = register_indirect;
        
        else
        {
            error_flag = TRUE;
            fprintf(stdout, "invalid register name for current addressing type, in line %d", line_counter);
            return ERROR_SIGN;
        }
    }


    else if(operand[0] == 'r') //if a register direct addressing type
    {
        if (operand[1] >= '0' && operand[1] <= '7') //if a valid register name
            curr_type = register_direct;

        else
        {
            error_flag = TRUE;
            fprintf(stdout, "invalid register name for current addressing type, in line %d", line_counter);
            return ERROR_SIGN;
        }
        
    }

    else //probably a symbol name, the correctness of the symbol will be checked in the second pass
        curr_type= direct;

    return curr_type;
    
}

int complement_2 (int num)
{
    num *= -1; //make the number positive.
    //issue a warning if the number is too high
    if(num >= MAX_VALUE)
        fprintf(stdout, "WARNING, number too large on line %d, data loss is applicable/", line_counter);
    num = ~num; //invert the bits in the number
    num <<= (BYTE * sizeof(int) - 11); //move and zero all the numbers left to the first 11 figures
    num >>= (BYTE * sizeof(int) - 11);//move back

    num += 1; //add 1

    return num;
}
