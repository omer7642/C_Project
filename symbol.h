#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SYMBOL_MAX_LENGTH 32
#define EXT_VALUE 0

//-------------VARIABLES-----------------------
extern memory;
extern line_counter;
extern error_flag;
//-------------ENUMS-----------------------

enum line_type{code,data,string,entry,external}; //locations of symbols

//-------------TYPEDEF-----------------------

/*defining the struct symbol*/
typedef struct sym{
    char symbol_name[SYMBOL_MAX_LENGTH];
    int value;
    enum line_type location;
    struct sym *next_symbol;
}symbol;

//-----------Functions------------------------

void add_symbol(char * ,int ,enum line_type);
void add_extern(char *);