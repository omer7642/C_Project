#include "general.h"

/*----------Functions------------------------------------------------*/
void add_symbol(char * symbol_name,int DC ,enum line_type); /*adding new symbol to the symbol table (checking first if the symbol exist), defubed in symbol.c*/
void add_extern(char *); /*adds an extern symbol to the memory, checking its validity and adds with value of 0*/
void code_data(char *,enum line_type,int); /*coding the data line. defined in codingFunctions.c*/
char get_line(FILE *,char *);       /*gets the line from the users and checks for general errors. returns -1 if EOF, 0 if line without a symbol declaration, or 1 with symbol declaration
                                        defined in input.c*/
enum line_type check_type(char *);  /*returns the type of the line , code, data, string, extern or entry. defined in input.c*/
char *get_symbol(char *,char *);          /*extracts the symbol from the line and returns it. defined in input.c*/
int get_command(char *,unsigned char);  /*extracts the command from the line and return its opcode*/
void code_data(char *,enum line_type,int); /*this function decodes the data lines, defined in codingFunctions.c*/
void code_instruction(char *,int,int); /*decodes instruction lines, defined in codingFunctions.c*/
int get_address_type(char *); /*recieves an operand and returns its adderessing type. if error it returns -1. defined in input.c*/
int add_symbol_value(char *,int); /*fills the field of value of the symbol according to the type of the symbols, with the right flags up*/
int add_entry(char *); /*checks to see if an entry symbol exists, update its type and its value accordingly*/
void update_data_symbol();  /*update value to IC+100*/

