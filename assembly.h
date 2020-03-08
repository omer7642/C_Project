#include "general.h"

#define ASSEMBLY_POSTFIX ".as"

void first_pass(FILE *, char *);
void second_pass(FILE *,char *);
void add_assembly_postfix(char *);
void make_output(char *);
void free_symbol_table();