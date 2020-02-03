#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define OPCODE_NUM 16
#define REGISTER_NUM 8
#define GUIDES_NUM 4
#define MAX_WORDS 4096
#define LOAD_SPACE 100

//----------EXTERNAL----------------------------------------------------
    extern DC,IC,line_counter,error_flag,memory,tmp_data_memory;
//----------Tables------------------------------------------------------
char *opcodes[OPCODE_NUM]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","red","prn","jsr","rts","stop"}; //all opcodes names
char *guides[GUIDES_NUM]={".data",".string",".entry",".extern"}; //all guides names
char *registers[REGISTER_NUM]={"r0","r1","r2","r3","r4","r5","r6","r7"}; //all registers names
//----------ENUMS-------------------------------------------------
enum line_type{code,data,string,entry,external}; //locations of symbols
enum address_type{immediate,direct,register_indirect,register_direct}; //addresses types

//----------Typedef--------------------------------------------------
typedef unsigned short int word; //memory cell in the computer
//----------Functions------------------------------------------------
void code_data(char *,int,enum line_type,int);