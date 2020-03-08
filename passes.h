#include "general.h"

/*----------Functions------------------------------------------------*/
void add_symbol(char * symbol_name,int DC ,enum line_type); /*adding new symbol to the symbol table (checking first if the symbol exist)*/
void add_extern(char *); /*adding all the symbols declared in the extern line*/
void code_data(char *,enum line_type,int); /*coding the data line*/
int get_address_type(char *);
int add_symbol_value(char *,int);
void add_entry_symbol(char *,int);
int add_entry(char *);
void update_data_symbol(); 


