#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define OPCODE_NUM 16
#define MAX_LINE 81
#define REGISTER_NUM 8
#define BYTE 8
#define ADDRESS_METHOD_NUM 4
#define MAX_WORDS 4096
#define MAX_VALUE 2048
#define LOAD_SPACE 100
#define END_CHAR_SPACE 1
#define ERROR_SIGN -1
#define MAX_NUM_LENGTH 50
#define MIN_VISABLE_ASCII 32
#define MAX_VISABLE_ASCII 127
#define OPERAND_MAX_LENGTH 50 
#define A 4
#define R 2
#define E 1
#define TRUE 1
#define FALSE 0
#define MAX_COMMAND_LENGTH 5
#define EXTRA_SPACE 5
#define OPCODE_SHIFT 11
#define SOURCE_SHIFT 7
#define TARGET_SHIFT 3
#define IMMIDIATE_SHIFT 3
#define NO_COMMAND -1
#define ERROR -1
#define EXT_VALUE 0
#define SYMBOL_MAX_LENGTH 32
#define EMPTY_LINE 2

#define CHECK_MEMORY (DC+IC >= MAX_WORDS) /*Checking if the total Words in the memory is too much*/
#define EXIT_IF_RUNOUT_MEMORY {\
    if CHECK_MEMORY {\
        fprintf(stderr,"assembler: RUN OUT OF MEMORY!\n");\
        exit(0);\
    }\
}

#define NOT_OK_CHAR(p)  ( !p || *p =='\n'|| !(*p) ) 

#ifndef ENUM
#define ENUM
enum line_type{code,data,string,entry,external}; /*locations of symbols*/
enum address_type{immediate,direct,register_indirect,register_direct}; /*addresses types*/
enum commandE{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,red,prn,jsr,rts,stop}; /*opcodes values*/
#endif

/*memory cell in the computer*/
typedef unsigned short int word; 

/*defining the struct symbol*/
typedef struct sym{
    char symbol_name[SYMBOL_MAX_LENGTH];
    int value;
    enum line_type location;
    struct sym *next_symbol;
}symbol;

typedef unsigned char BOOL; 

extern short DC,IC;
extern int line_counter;
extern unsigned char error_flag,second_pass_flag;
extern word *memory;
extern word *data_memory;
