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
extern word *memory;
extern int line_counter;
extern unsigned char error_flag;
extern short IC;
//-------------ENUMS-----------------------

extern enum line_type{code,data,string,entry,external}; //locations of symbols

//-------------TYPEDEF-----------------------

/*defining the struct symbol*/
typedef struct sym{
    char symbol_name[SYMBOL_MAX_LENGTH];
    int value;
    enum line_type location;
    struct sym *next_symbol;
}symbol;

typedef unsigned char BOOL; 

//-----------Functions------------------------

void add_symbol(char * ,int ,enum line_type);
void add_extern(char *);
int add_entry(char *);
void update_data_symbol(); //update the value of data and string to +100
int get_symbol_amount(enum line_type); //returns the amount of the symbols from the recived type
void print_symbols(FILE *,enum line_type);
void add_entry_symbol(char *,int);
BOOL isSavedPhrase(char *);



