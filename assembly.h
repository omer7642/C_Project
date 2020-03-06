
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORDS 4096
#define ASSEMBLY_POSTFIX ".as"


extern unsigned char error_flag;

typedef unsigned short int word; //memory cell in the computer

void first_pass(FILE *, char *);
void second_pass(FILE *,char *);
void add_assembly_postfix(char *);
void make_output(char *); 