#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LINE 81

int line_counter = 0;
int error_flag = FALSE;
char get_line(FILE *fp,char *current_line);
int line_flag;

int main()
{
    FILE *fp;
    fp = fopen("x.as", "r");
    char current_line[MAX_LINE];
    line_flag = get_line(fp, current_line);
    while( (line_flag )!= EOF)
        printf("%d, %s, %d", line_counter, current_line, line_flag);
        
    return 0;
}




char get_line(FILE *fp,char *current_line)
{
    unsigned int InputIndex = 0, ActualIndex = 0; //two indexes, one of input and another without the extra whitespaces
    char c; //the received character
    unsigned char isDeclaration = FALSE; //a flag for symbol declaration
    unsigned char isEmpty = TRUE; //a flag that says if line is empty
    c = fgetc(fp);

    printf("started get line \n");//for debugging

    while(((c != '\n') && (isEmpty == TRUE)) || (c != EOF) || (InputIndex > MAX_LINE - 1))
    {
        if((c == ';') && isEmpty == TRUE) //if the first character in a line is a ';', then its a comment line and we treat it as
        {
            while((c != '\n') || c != EOF) //skip to the end of the line, whether its the end of of the line itself or the end of the file
                c = fgetc(fp);
        }

        if((c != ' ') && (c != '\t') && (c != '\n')) //adds valid characters to line, without whitespaces
        {
            if(c == ':') //if the ':' character appears, it means a symbol is declared in the line
                isDeclaration = TRUE;
            isEmpty = FALSE; //if we entered a valid character, the line isn't empty
            current_line[ActualIndex++] = c;
        }

        else //if whitespace, add in the following conditions
        {
            if((InputIndex > 0) && (current_line[InputIndex - 1] != ' ') && (current_line[InputIndex -1] != '\t'))
                current_line[ActualIndex++] = ' '; //if its a necessary whitespace that devides parts or parameters of the current line, add a space to the actual received line
            else if((c == '\n') && (isEmpty == TRUE)) //if empty line or comment line, still raise the counter
                if(InputIndex > MAX_LINE)
                {
                    fprintf(stdout, "Maximum line length exceeded on line %d", line_counter);
                }
                    line_counter++;
                    InputIndex = 0;
                    continue;
                

        }

        //receive next character from the input stream and advance InputIndex
        c = fgetc(fp);
        InputIndex++;
    }
    current_line[ActualIndex] = '\0'; //adds a '\0' at the end of the line
    if(InputIndex > MAX_LINE + 1) //if the input exceeded maximum line length, throw an error
    {
        error_flag = TRUE;
        fprintf(stdout, "Maximum line length exceeded on line %d", line_counter);
    }
    if(c == EOF)
    {
        //if(isEmpty == TRUE) //last line is empty
            return EOF;
        //else //last line isn't empty, so we will need to receive EOF on next time
            //return isDeclaration; //return to previous fp position so next time we'll get EOF
    }
    if(isDeclaration == TRUE)
        return TRUE;
    else
    {
        return FALSE;
    }
    
}