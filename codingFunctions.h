#include "general.h"

/*----------Functions------------------------------------------------*/
void code_data(char *,enum line_type,int); /*this function decodes the data lines*/
void code_instruction(char *,int,int); /*decodes instruction lines*/
int get_address_type(char *); /*receives an operand and returns its addressing type, defined in input.c*/
int complement_2 (int); /*receives a negative number and represents it in a 12 bit form. if there's a loss of information due to large number - it alerts*/

