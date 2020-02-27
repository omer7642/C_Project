#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 81
#define ADDRESS_METHOD_NUM 4
#define OPCODE_NUM 16
#define REGISTER_NUM 8
#define GUIDES_NUM 4
#define MAX_COMMAND_LENGTH 5
#define MAX_WORDS 4096
#define LOAD_SPACE 100
#define SYMBOL_MAX_LENGTH 32
#define NO_COMMAND -1

extern memory;

//----------ENUMS-------------------------------------------------
enum line_type{code,data,string,entry,external}; //locations of symbols
enum address_type{immediate,direct,register_indirect,register_direct}; //addresses types

//----------Typedef--------------------------------------------------
typedef unsigned short int word; //memory cell in the computer
//----------Functions------------------------------------------------
void add_symbol(char * symbol_name,int DC ,enum line_type); //adding new symbol to the symbol table (checking first if the symbol exist)
void add_extern(char *); //adding all the symbols declared in the extern line
void code_data(char *,int,enum line_type,int); //coding the data line
