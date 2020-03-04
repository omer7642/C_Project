#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ASSEMBLY_POSTFIX ".as"
#define OBJECT_POSTFIX ".ob"
#define EXTERN_POSTFIX ".ext"
#define ENTRY_POSTFIX ".ent"
#define EXTRA_SPACE 5
#define LOAD_SPACE 100

typedef unsigned short int word; //memory cell in the computer
extern enum line_type{code,data,string,entry,external}; //locations of symbols


extern short IC,DC;
extern word *memory;

void make_output(char *);
void add_assembly_postfix(char *);
void make_object_file(char *);
void make_entry_file(char *);
void make_extern_file(char *);

int get_symbol_amount(enum line_type);
void print_symbols(FILE *,enum line_type);
