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


struct connections
{
    char sockedName[200];
    int  sockedID;
};

void msg_to_all(struct connections connections[200], int n, char msg[1000]);
void msg_hello(char msg[1000]);
void msg_goodby(char msg[1000]);
void msg_from_client(int connfd);
void scan_client(void);

static int count = 0;
static struct connections connec[200]; 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, len;
    struct sockaddr_in serv_addr; 
    char welcomeMassage[1000] = "Hello and welcome to my server\nType your name below";
    char sendBuff[1025];
    char my_buff[1024];
    time_t ticks;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    
    listen(listenfd, 10);
    printf ("The IP-Adress: %s\n", inet_ntoa(serv_addr.sin_addr));
    printf ("The IP-Port: %d\n", (int) ntohs(serv_addr.sin_port));
    
    while(1)
    {   
        
        fd_set rfds;
        struct timeval tv;
        int reRet;
        int nfds;

        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        FD_SET(listenfd,&rfds);
        nfds = listenfd + 1;

        tv.tv_sec = 1000;
        tv.tv_usec = 0;

        if(reRet == -1)
        {
            perror("select()");
            break;
        }
        else
        {
            char input_buffer[1024];
            if(FD_ISSET(listenfd,&rfds))
            {
                connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
                char name_buff[200];
                int ID;

                //To  client
                ticks = time(NULL);
                snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n%s\n",  ctime(&ticks),welcomeMassage);
                write(connfd, sendBuff, strlen(sendBuff));

                //Get client name
                ID = read(connfd, my_buff,strlen(my_buff));

                //Set informantion if struct
                snprintf(connec[count].sockedName,sizeof(connec[count].sockedName),"%s",my_buff);
                connec[count].sockedID = connfd;
                
                printf("connections: %d\tSocked:%d\tName: %s\n",count,connec[count].sockedID,connec[count].sockedName);

                count++;

            }
            else if f(FD_ISSET(0,&rfds))
            {
                int l;
                l = read(0,input_buffer,sizeof(input_buffer));
                if(l > 0)
                {
                     msg_to_all(connec, count, input_buffer);
                }
                else
                    break;
            }

        }

        sleep(1);
    }
}

void msg_to_all(struct connections connections[200], int n, char msg[1000])
{
    int i;
    int k;
    char new_c_buff[1000];
    snprintf(new_c_buff, sizeof(new_c_buff), "%s\n", msg);
    for(i = 0; i < n; i++)
    {
        k = write(connections[i].sockedID,new_c_buff,strlen(new_c_buff));
    }    
   
}

void msg_hello(char msg[1000])
{
    char sendBuff[1000];
    snprintf(sendBuff, sizeof(sendBuff), "Say hello to %s", msg);
    msg_to_all(connec, count, sendBuff);    
}

void msg_goodby(char msg[1000])
{
    char sendBuff[1000];
    snprintf(sendBuff, sizeof(sendBuff), "Say goodby to %s", msg);
    msg_to_all(connec, count, sendBuff);    
}
