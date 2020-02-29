#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "assembly.h"

#define SYMBOL_MAX_LENGTH 32
#define EXT_VALUE 0
#define TRUE 1
#define FALSE 0
#define E 1
#define R 2
#define A 4
//-------------VARIABLES-----------------------
extern memory;
extern line_counter;
extern error_flag;
extern IC;
extern memory;
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
void update_data_symbol(); //update the value of data and string to +100