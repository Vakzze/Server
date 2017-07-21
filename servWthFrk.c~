#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
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

int chat(int socketfd);

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, len;
    struct sockaddr_in serv_addr; 
    int count = 0;
    char welcomeMassage[1000] = "Hello and welcome to my server";
    char message[1000];
    char sendBuff[1025];
    char incomeMsg[1000];
    time_t ticks; 


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //memset(&serv_addr, '0', sizeof(serv_addr));
    //memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
     

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10);
    printf ("The IP-Adress: %s\n", inet_ntoa(serv_addr.sin_addr));
    printf ("The IP-Port: %d\n", (int) ntohs(serv_addr.sin_port));


    while(1)
    {
        if ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) > 0)
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                ticks = time(NULL);
                count = count + 1; 
                printf("Number of connections: %d\n",count);
                snprintf(sendBuff, sizeof(sendBuff), "%s\n%.24s\r\n", welcomeMassage, ctime(&ticks));
                write(connfd, sendBuff, strlen(sendBuff));

                chat(connfd);
                exit(0);
            }
            else if (pid > 0)
            {
                printf("Forked a child(%d)", pid);
            }
            else 
            {
                perror("Forked failed");
            } 
        } 
        
        
        
        
        sleep(1);
     }
}


int chat(int connfd)
{
    while(1)
    {

        fd_set rfds;
        struct timeval tv;
        int reRet;
        int nfds;

        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        FD_SET(connfd, &rfds);
        nfds = connfd + 1;

        tv.tv_sec = 1000;
        tv.tv_usec = 0;
        
        reRet = select(nfds, &rfds, NULL, NULL, &tv);

        if(reRet == -1)
        {
            perror("select()");
            break;
        }
        else
        {
            char input_buffer[1024];
            if(FD_ISSET(0,&rfds))
            {
                int l;
                l = read(0,input_buffer,sizeof(input_buffer));
                if(l > 0)
                    write(connfd,input_buffer,l);
                else
                    break;
            }
            else if(FD_ISSET(connfd,&rfds))
            {
                int l;
                l = read(connfd, input_buffer, sizeof(input_buffer));
                if(l > 0 )
                    write(1,input_buffer,l);
                else
                    break;
            }
            else
            {
                printf("No data recived within 5 seconds\n");
            }
        }
    }
    
    close(connfd);
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


