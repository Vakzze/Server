#include <stdio.h>
#include <string.h>

int main () 
{
    char master[20];
  
    
    printf("Hello type something:\n");

    while (fgets(master,20,stdin) != "q\n")
    {  
        printf("Your write: %s \n", master);
    }
    
    printf("Goodbye\n");
    
    return 0;
}
