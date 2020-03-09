#include "general.h"

/*----------Functions------------------------------------------------*/
void add_symbol(char * symbol_name,int DC ,enum line_type); /*adding new symbol to the symbol table (checking first if the symbol exist)*/
void add_extern(char *); /*adding all the symbols declared in the extern line*/
void code_data(char *,enum line_type,int); /*coding the data line*/
char get_line(FILE *,char *);       /*gets the line from the users and checks for general errors. returns -1 if EOF, 0 if line without a symbol declaration, or 1 with symbol declaration*/
enum line_type check_type(char *);  /*returns the type of the line , code, data, string, extern or entry*/
char *get_symbol(char *,char *);          /*extracts the symbol from the line and returns it*/
int get_command(char *,unsigned char);  /*extracts the command from the line and return its opcode*/
void code_data(char *,enum line_type,int);
void code_instraction(char *,int,int);
int get_address_type(char *);
int add_symbol_value(char *,int);
void add_entry_symbol(char *,int);
int add_entry(char *);
void update_data_symbol();  


