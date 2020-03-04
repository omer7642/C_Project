#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define OPCODE_NUM 16
#define REGISTER_NUM 8
#define GUIDES_NUM 4
#define MAX_WORDS 4096
#define LOAD_SPACE 100
#define END_CHAR_SPACE 1
#define MAX_NUM_LENGTH 50
#define MIN_VISABLE_ASCII 32
#define MAX_VISABLE_ASCII 127
#define OPERAND_MAX_LENGTH 50
#define A 4
#define R 2
#define E 1
#define OPCODE_SHIFT 11
#define SOURCE_SHIFT 7
#define TARGET_SHIFT 3
#define IMMIDIATE_SHIFT 3
#define ERROR -1

#define NOT_OK_CHAR(p)  ( !p || *p =='\n'|| !(*p) )


#define CHECK_MEMORY (DC+IC >= MAX_WORDS) //Checking if the total Words in the memory is too much
#define EXIT_IF_RUNOUT_MEMORY {\
    if CHECK_MEMORY {\
        fprintf(stdout,"assembler: RUN OUT OF MEMORY!\n");\
        exit(0);\
    }\
}

//----------Typedef--------------------------------------------------
typedef unsigned short int word; //memory cell in the computer
//----------EXTERNAL----------------------------------------------------
extern short DC,IC;
extern int line_counter;
extern unsigned char error_flag;
extern word *memory;
extern word *data_memory;
//----------Tables------------------------------------------------------
char *opcodes[OPCODE_NUM]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"}; //all opcodes names
char *guides[GUIDES_NUM]={".data",".string",".entry",".extern"}; //all guides names
char *registers[REGISTER_NUM]={"r0","r1","r2","r3","r4","r5","r6","r7"}; //all registers names
//----------ENUMS-------------------------------------------------
#ifndef ENUM
#define ENUM
enum line_type{code,data,string,entry,external}; //locations of symbols
enum address_type{immediate,direct,register_indirect,register_direct}; //addresses types
enum commandE{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,red,prn,jsr,rts,stop}; //opcodes values
#endif
//----------Functions------------------------------------------------
void code_data(char *,enum line_type,int);
void code_Instraction(char *,int,int);
int get_address_type(char *);
int complement_2 (int); //receives a negative number and represents it in a 11 bit form. if there's a loss of information due to large number - it alerts