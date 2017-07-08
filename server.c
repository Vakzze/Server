#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 


int getLine(char s[], int lim);


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int count = 0;
    char welcomeMassage[100] = "Hello and welcome to my server";
    char message[1000];
    char sendBuff[1025];
    time_t ticks; 


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
     

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10);
    printf ("The IP-Adress: %s\n", inet_ntoa(serv_addr.sin_addr));
    printf ("The IP-Port: %d\n", (int) ntohs(serv_addr.sin_port));


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        ticks = time(NULL);
        count = count + 1; 
        printf("Nummber of connections: %d\n",count);
        snprintf(sendBuff, sizeof(sendBuff), "%s\n%.24s\r\n", welcomeMassage, ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 
        
        printf("Type a something to the client:\n");
        getLine(message,1000);
        printf("%s", message);

        close(connfd);
        sleep(1);
     }
}


int getLine(char s[], int lim)
{
    int c, i;
    for(i = 0; i < lim-1 && (c=getchar()) != EOF && c != '\n'; ++i)
    {
        s[i] = c;
    }

    if(c == '\n')
    {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
                     
    return i;
   
}


