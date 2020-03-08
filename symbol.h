#include "general.h"

/*-----------Functions------------------------*/

void add_symbol(char * ,int ,enum line_type);
void add_extern(char *);
int add_entry(char *);
void update_data_symbol(); /*update the value of data and string to +100*/
int get_symbol_amount(enum line_type); /*returns the amount of the symbols from the recived type*/
void print_symbols(FILE *,enum line_type);
void add_entry_symbol(char *,int);
void free_symbol_table();
BOOL isSavedPhrase(char *);



