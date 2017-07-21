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
    int  state; //0 means waiten for name, 1 writing to all, 2 writting to some people
    int  msg_socked[20];
};



void msg_to_all(struct connections connections[200], int n, char msg[1000]);
void msg_hello(struct connections connec[200], int count, char msg[1000]);
int find_Socked(char name_buff[1024], struct connections connec[200], int count);



int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, len;
    struct sockaddr_in serv_addr; 
    char welcomeMassage[1000] = "Hello and welcome to my server\nType your name below";
    char sendBuff[1025];
    char my_buff[1024];
    time_t ticks;

    static int count = 0;
    static struct connections connec[200];

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
        int reRet, i;
        int j = 0;
        int nfds, nnfds;
        char msg_buff[1000] = "\0";


        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        FD_SET(listenfd, &rfds);
        
        nfds = listenfd;
        
        tv.tv_sec = 1000;
        tv.tv_usec = 0;
        
        
        for(i = 0; i < count; i++)
        {
            int sck = connec[i].sockedID;
            FD_SET(sck,&rfds);
            if(sck > nfds)
            {
                nfds = sck; 
            }
        }
        
        reRet = select(nfds + 1, &rfds, NULL, NULL, &tv);

        if(reRet == -1)
        {
            perror("select()");
        }
        else
        {
            char input_buffer[1024] = "\0";
            if(FD_ISSET(0,&rfds))
            {
                int l;
                l = read(0,msg_buff, sizeof(msg_buff));
                if(l > 0)
                {
                    snprintf(input_buffer, sizeof(input_buffer),"From server: %s", msg_buff);
                    msg_to_all(connec,count,input_buffer);
                    
                }
                //else
                    //break;
            }
            else if(FD_ISSET(listenfd,&rfds))
            {
                int l;
                if ((connfd = accept(listenfd,(struct sockaddr*)NULL,NULL)) > 0)
                {
                    char name_buff[200];
                    int ID;
        
                    //To  client
                    ticks = time(NULL);
                    snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n%s\n",  ctime(&ticks),welcomeMassage);
                    write(connfd, sendBuff, strlen(sendBuff));

                    //Get client name

                    snprintf(name_buff, sizeof(name_buff), "Anonymous nr. %d",count);                   

                    //Set information if struct
                    snprintf(connec[count].sockedName,sizeof(connec[count].sockedName),"%s",name_buff);
                    connec[count].sockedID = connfd;

                    connec[count].state = 0;
                                                    
                    printf("connections: %d\tSocked:%d\tName: %s\n",count,connec[count].sockedID,connec[count].sockedName);
                    
                    count++;
                }
                
            }
            for (i = 0; i < count; i++)
            {
                int sck = connec[i].sockedID;
                int State = connec[i].state;
                int m;
                if(FD_ISSET(sck,&rfds))
                {
                    m = read(sck,msg_buff,sizeof(msg_buff));
                    
                    if (State == 0)
                    {
                        printf("%s changed name to %s\n",connec[i].sockedName,msg_buff);
                        snprintf(connec[i].sockedName,sizeof(connec[i].sockedName),"%s",msg_buff);
                        connec[i].state = 1;
                        msg_hello(connec, count, connec[i].sockedName);
                    }
                    else if (msg_buff[0] == '/' || connec[i].state == 2)
                    {
                        char cur_buff[200] = "\0";
                        char msg_to_server[1024] = "\0";
                        int p;

                        if((p = strcmp(msg_buff,"/All\n")) == 0)
                        {
                            snprintf(cur_buff,sizeof(cur_buff),"You are currently typing to everyone\n");
                            write(sck,cur_buff,sizeof(cur_buff));
                            connec[i].state = 1;
                        }
                        else
                        {
                            if(connec[i].state == 1)
                            {
                                int new_socked_ID = find_Socked(msg_buff,connec,count);
                                connec[i].msg_socked[i] = new_socked_ID;
                                snprintf(cur_buff,sizeof(cur_buff),"You are currently typing to: %s",msg_buff);
                                write(sck,cur_buff,strlen(cur_buff));

                                //add to msg_socked
                                connec[i].state = 2;
                            }
                            else
                            {
                                snprintf(msg_to_server,sizeof(msg_to_server), "hidding msg from %s, to %d\n ",connec[i].sockedName,connec[i].msg_socked[i]);
                                write(0,msg_to_server,strlen(msg_to_server));
                                write(0,msg_buff,strlen(msg_buff));

                                //To to alle in the msg_socked
                                write(connec[i].msg_socked[i],msg_buff,strlen(msg_buff));
                            }
                        }
                  
                        //Type to a specific client
                    }
                    else if(connec[i].state == 1)
                    {
                        //type to all
                        msg_to_all(connec,count,msg_buff);
                    }
                    fflush(stdin);
                }
                
            }
          
        }

        
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
        msg[strlen(msg)] = '\0';
        k = write(connections[i].sockedID,new_c_buff,strlen(new_c_buff));
    } 
}

void msg_hello(struct connections connec[200], int count, char msg[1000])
{
    char send_Buff[1000];
    snprintf(send_Buff, sizeof(send_Buff), "Say hello to %s", msg);
    msg_to_all(connec, count, send_Buff); 
}


int find_Socked(char name_buff[1024], struct connections connec[200], int count)
{   
    int i;
    
    for(i = 0; i < count; i++)
    {
        char cv_name[1025] = "\0";
        snprintf(cv_name,sizeof(cv_name),"/%s",connec[i].sockedName);

        char name[201] = "\0";
        snprintf(name,sizeof(name),"%s",name_buff);

        if(strcmp(name,cv_name) == 0)
        {
            //printf("%s , %s, %d\n",cv_name,name,strcmp(name,cv_name));
            return connec[i].sockedID;
        }   
    }
    return 0;
}


