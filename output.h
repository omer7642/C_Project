#include "general.h"

#define ASSEMBLY_POSTFIX ".as"
#define OBJECT_POSTFIX ".ob"
#define EXTERN_POSTFIX ".ext"
#define ENTRY_POSTFIX ".ent"

void make_output(char *); /*if no errors this function create the .ob, .ent and .ext file with the current memory image */
void add_assembly_postfix(char *);/*adds an .as postfix to a given filename*/
void make_object_file(char *); /*this function makes an .ob file, with the right postfix, printing the memory image into the file*/
int get_symbol_amount(enum line_type); /*prints the amount of symbols from a specific type, defined in symbols.c*/
void print_symbols(FILE *,enum line_type); /*prints symbols of a specific type and their value into a file, defined in symbols.c*/
void make_symbol_file(char *,enum line_type,char *);
