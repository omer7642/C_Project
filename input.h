#include "general.h"
/*defines*/
#define NEGATIVE_NUM_SPACE 11


char get_line(FILE *,char *);       /*gets the line from the users and checks for general errors. returns -1 if EOF, 0 if line without a symbol declaration, or 1 with symbol declaration*/
enum line_type check_type(char *);  /*returns the type of the line , code, data, string, extern or entry*/ 
int get_command(char *,unsigned char);  /*extracts the command from the line and return its opcode*/
char *get_symbol(char *,char *);          /*extracts the symbol from the line and returns it*/
BOOL isSavedPhrase(char *); /*assures that the given symbol is not a saved phrase*/
int get_address_type(char *); /*recieves an operand and returns its adderessing type. if error it returns -1*/
int complement_2 (int); /*receives a negative number and represents it in a 11 bit form. if there's a loss of information due to large number - it alerts*/

