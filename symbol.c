#include "symbol.h" 

symbol *symbol_table = NULL;

void add_symbol(char * symbolN,int memory_value ,enum line_type type){
    symbol *tmp,*p;
    unsigned flag = 0;

    //checking if the symbol exist
    tmp = symbol_table;
    while(tmp)
        if(strcmp(tmp->symbol_name,symbolN)==0){
            error_flag=1;
            fprintf(stdout,"symbol-table: error - trying to declare an exist symbol. (line %d) \n",line_counter);
            return;
        }
    tmp = (symbol *)malloc(sizeof(symbol));
    strcpy(tmp->symbol_name,symbolN); //adding name
    tmp->location = type;             //adding data type
    tmp->value = memory_value;        //adding memory value
    tmp->next_symbol=NULL;            //initialize the pointer

    if(!symbol_table)       //if the table empty
        symbol_table = tmp;
    
    else{
        p = symbol_table;
        while(p->next_symbol)
            p = p->next_symbol;
        p->next_symbol = tmp;
    }

}

void add_extern(char *line){
    char *token,c;
    
    //Extracting the symbol name
    token = strtok(line, " \t");
    token = strtok(NULL, " \t");

    //Checking if the syntax is correct
    if(!token || !isalpha(token[0]))
    {
        error_flag=1;
        fprintf(stdout,"assembler: error - symbol name is invaild  (line %d)\n",line_counter);
        return;
    }
    
    for(int i=1; i<strlen(token);i++)
        if(!isalnum(token[i])){
            error_flag=1;
            fprintf(stdout,"assembler: error - symbol name is invaild  (line %d)\n",line_counter);
            return;
        }
    
    //need to add function - that checks if a word is a saved word (for example: 'mov' , 'add' ....) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
    
    //if all the test went good, adding the symbol to the symbol table with the value 0 
    add_symbol(token,EXT_VALUE,external);   
     
}