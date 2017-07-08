#include <stdio.h>
#include <ctype.h>
#include <string.h>


//The purpose of this program is to convert a char to upper or lower case depending on a argument argv

int main(int argc, char *argv[])
{   
    
    int u;   

    if (strcmp(argv[1], "u") == 0)
    {
        int c;
        while((c = getchar()) != EOF)
            putchar(toupper(c));
        return 0;

    } 
    else if ((strcmp(argv[1], "l") == 0))
    {
        int c;
        while((c = getchar()) != EOF)
            putchar(tolower(c));
        return 0;        
    } else {
    
        printf ("Is this alive?\n");
    }
    return 0;
}

