#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 81
#define ADDRESS_METHOD_NUM 4
#define OPCODE_NUM 16
#define REGISTER_NUM 8
#define GUIDES_NUM 4
#define MAX_COMMEND_LENGTH 5
#define MAX_WORDS 4096
#define LOAD_SPACE 100
#define SYMBOL_MAX_LENGTH 32
#define NO_COMMAND -1

#define CHECK_MEMORY (DC+IC >= MAX_WORDS) //Checking if the total Words in the memory is too much
#define EXIT_IF_RUNOUT_MEMORY {\
    if CHECK_MEMORY {\
        fprintf(stdout,"assembler: RUN OUT OF MEMORY!\n");\
        exit(0);\
    }\
}

#define NOT_OK_CHAR(p)  ( !p || *p =='\n'|| !(*p) ) 



#define EMPTY_LINE 2


//----------ENUMS-------------------------------------------------

#ifndef ENUM
#define ENUM
enum line_type{code,data,string,entry,external}; //locations of symbols
enum address_type{immediate,direct,register_indirect,register_direct}; //addresses types
enum commandE{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,red,prn,jsr,rts,stop}; //opcodes values
#endif

//----------Typedef--------------------------------------------------
typedef unsigned short int word; //memory cell in the computer

//----------Functions------------------------------------------------
char get_line(FILE *,char *);       //omer
enum line_type check_type(char *);  //omer
int get_command(char *,unsigned char);  //omer
char *get_symbol(char *,char *);          //omer
void add_symbol(char * symbol_name,int DC ,enum line_type); //adding new symbol to the symbol table (checking first if the symbol exist)
void add_extern(char *); //adding all the symbols declared in the extern line
void code_data(char *,enum line_type,int); //coding the data line
int get_address_type(char *);
int add_symbol_value(char *,int);
void add_entry_symbol(char *,int);
int add_entry(char *);
void update_data_symbol(); 


extern word *memory;
extern word *data_memory;
