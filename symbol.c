#include "symbol.h" 

symbol *symbol_table = NULL;

void add_symbol(char * symbolN,int memory_value ,enum line_type type){
    symbol *tmp,*p;

    /*checking if the symbol exist*/
    tmp = symbol_table;

    if(type!=entry && type!=external)
        while(tmp)
        {
            if(strcmp(tmp->symbol_name,symbolN)==0)/*in case the symbol is exiting*/
            { 
                error_flag=TRUE;
                fprintf(stderr,"symbol-table: error - trying to declare an exist symbol. (line %d) \n",line_counter);
                return;
            }
            tmp = tmp->next_symbol;
        }

    tmp = (symbol *)malloc(sizeof(symbol));
    strcpy(tmp->symbol_name,symbolN); /*adding name*/
    tmp->location = type;             /*adding data type*/
    tmp->value = memory_value;        /*adding memory value*/
    tmp->next_symbol=NULL;            /*initialize the pointer*/

    if(!symbol_table)       /*if the table empty*/
        symbol_table = tmp; 
    
    else{
        p = symbol_table;
        while(p->next_symbol)
            p = p->next_symbol;
        p->next_symbol = tmp;
    }

}

void add_extern(char *line){
    char *token;
    char *temp_line = (char *)malloc(MAX_LINE);
    int i;

    strcpy(temp_line,line);
    
    /*Extracting the symbol name*/
    token = strtok(temp_line, " \t");
    token = strtok(NULL, " \t");

    /*Checking if the syntax is correct*/
    if(!token || !isalpha(token[0]))
    {
        error_flag=TRUE;
        fprintf(stderr,"Assembler: error - symbol name is invaild  (line %d)\n",line_counter);
        return;
    }
    
    for( i=1; i<strlen(token);i++)
        if(!isalnum(token[i])){
            error_flag=TRUE;
            fprintf(stderr,"Assembler: error - symbol name is invaild  (line %d)\n",line_counter);
            return;
        }
    
    /*need to add function - that checks if a word is a saved word (for example: 'mov' , 'add' ....) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
    
    /*if all the test went good, adding the symbol to the symbol table with the value 0 */
    add_symbol(token,EXT_VALUE,external); 

    free(temp_line);
     
}

void update_data_symbol()
{
    symbol *curr = symbol_table;
    while(curr)
    {
        if(curr->location == data || curr->location == string) 
            curr->value += IC+100;
        curr = curr->next_symbol;
    }
}


int add_entry(char *symbol_name)
{
    int isFound = FALSE; /*the argument we return eventually*/
    symbol *curr = symbol_table; /*represents the current node we are checking*/

    while(curr) /*going through the table*/
    {
        if(!strcmp(curr->symbol_name, symbol_name)) /*if the current node has same symbol name , quit the loop, update to entry and update the isFound var*/
        {
            isFound = TRUE;
            curr->location = entry;
            break;
        }
        curr = curr->next_symbol;
    }

    return isFound;
}

int add_symbol_value(char *token, int index)
{
    symbol *curr = symbol_table ; /*represents current node*/
    int isFound = FALSE;

    while(curr) /*looping through the table either until no symbol found or one is found*/
    {
        if( strcmp(curr->symbol_name,token) == 0) /*if we found the symbol that we're looking for*/
        {
            if(curr->location == external) /*if its an external label. it has no address (address 0) and the E indicator is up*/
            {
                if(curr->value==0)
                {
                    curr->value=index+LOAD_SPACE;
                }

                else /*the usage of that is for the file print*/
                {
                    add_symbol(token,index+LOAD_SPACE,external);
                }

                memory[index] = E;
                isFound = TRUE;
                break;
            }


            else /*if its not an external label, than its a label defined in the file and value is R with the addition of the address it is defined*/
            {
                memory[index] = R ;
                memory[index] |= ( (curr->value) << 3);
                isFound = TRUE;
                break;
            }
            
        }
        
        curr = curr->next_symbol;
    }

    return isFound;
}


int get_symbol_amount(enum line_type type)
{
    symbol *p = symbol_table; /*the variable we use to iterate through the symbol table*/
    int count = 0; /*the counter of the amount of times a certain type is defined*/
    while (p)
    {
        if(p->location == type)
            count++;
        p = p->next_symbol;
    }

    return count;        
    
}

void add_entry_symbol(char *current_line,int symbol_flag)
{
    char *token;
    token = strtok(current_line," \t"); /*getting the first word of the line*/
    if(symbol_flag)
    {
        fprintf(stderr,"Warning: defining a symbol in entry line\n");
        token = strtok(NULL," \t");
    }

    token = strtok(NULL," \t"); /*getting the symbol name*/

    if(isSavedPhrase(token)) /*uf the symbol is a saved phrase*/
    {
        error_flag = TRUE;
        fprintf(stderr,"Assembler: using a saved word for symbol name\n");
        return;
    }

    add_symbol(token,0,entry);

}

void print_symbols(FILE *fp, enum line_type type)
{
    symbol *ptr = symbol_table;
    
    while(ptr) /*iteration through the symbol table, and print to file it's name and value*/
    {
        if(ptr->location == type)
        {
            fprintf(fp,"%s %4d\n",ptr->symbol_name,ptr->value);
        }

        ptr = ptr->next_symbol;
    }
    return;
}

void free_symbol_table(){
    symbol *p,*q;

    p=symbol_table;

    while(p) /*iteration through the symbol table, with two nodes, while one advamces the prev one is being freed*/
    {
        q=p;
        p=p->next_symbol;
        free(q);
    }


    
    symbol_table = NULL; /*at the end symbol table gets a NULL value*/

    return;
}

