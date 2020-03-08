#include "general.h"

#define ASSEMBLY_POSTFIX ".as"
#define OBJECT_POSTFIX ".ob"
#define EXTERN_POSTFIX ".ext"
#define ENTRY_POSTFIX ".ent"

void make_output(char *);
void add_assembly_postfix(char *);
void make_object_file(char *);
void make_entry_file(char *);
void make_extern_file(char *);
int get_symbol_amount(enum line_type);
void print_symbols(FILE *,enum line_type);
