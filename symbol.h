#include "general.h"

/*-----------Functions------------------------*/

void add_symbol(char * ,int ,enum line_type);
void add_extern(char *);
int add_entry(char *);
void update_data_symbol(); /*update the value of data and string to +100*/
int get_symbol_amount(enum line_type); /*returns the amount of the symbols from the recived type*/
void print_symbols(FILE *,enum line_type); /*prints all the symbols from the symbol table from a specific type, and their value into a file*/
void free_symbol_table(); /*frees the symbol table node by node*/
BOOL isSavedPhrase(char *); /*checks to see if a symbol is a saved phrase, defined in input.c*/



