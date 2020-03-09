#include "general.h"

#define ASSEMBLY_POSTFIX ".as"

void first_pass(FILE *, char *); /*a function that goes through a file and checks its validity, and creates a symbol table for the second pass*/
void second_pass(FILE *,char *); /*after having a complete symbol table does the final decoding*/
void add_assembly_postfix(char *); /*adds an .as postfix to a given filename, defined in output.c*/
void make_output(char *); /*if no errors this function create the .ob, .ent and .ext file with the current memory image, defined in output.c*/
void free_symbol_table(); /*goes through the symbol table and free all the nodes in it*/

