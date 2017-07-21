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

int snNwConnections(char * sck[200], char name[200]);

struct CID
{
    char ID[200];
    int  sockedfd;
};


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, len;
    struct sockaddr_in serv_addr; 
    int count = 0;
    char welcomeMassage[1000] = "Hello and welcome to my server\nType your name below";
    char message[1000];
    char sendBuff[1025];
    char incomeMsg[1000];
    time_t ticks; 
    char connec[200];
    char *p[200]; //to point a connect

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
            p[count] = &connec[count];
            //connec[count] = connfd;i
            *p[count] = connfd;
            
            struct CID IDC;
            count = count + 1; 
            pid_t pid = fork();


            if (pid == 0)
            {   
                
                ticks = time(NULL);
                char message2[1000];
                int currentCount = count;

                snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n%s\n",  ctime(&ticks),welcomeMassage);
                write(connfd, sendBuff, strlen(sendBuff));

                int id = read(connfd,IDC.ID,sizeof(IDC.ID));
                IDC.sockedfd = connfd;
                printf("Number of connections: %d\tSocked: %d\tName: %s",count,IDC.sockedfd,IDC.ID);
                
                //printf("socked:%d\n", IDC.sockedfd);
                
                snprintf(message2, sizeof(message2), "Say hello to %s", IDC.ID);
                snNwConnections(p,message2);

                chat(connfd);
                snprintf(message2, sizeof(message2), "Goodby to %s", IDC.ID);

                snNwConnections(p,message2);
                

                exit(0);
            }
            else if (pid > 0)
            {
                printf("Forked a child(%d), sockedfd: %d\n", pid, connfd);
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

int snNwConnections(char * sck[200], char name[200])
{
    int i;
    char s_buff[1000]; 
    snprintf(s_buff, sizeof(s_buff),"%s\n",name);
    
    //printf("%ld\n", sizeof(&sck));
    for (i = 0; i < strlen(*sck); i++)
    {
        printf("socked: %d\n", *sck[i]);
        write(*sck[i],s_buff,strlen(s_buff));
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


