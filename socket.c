#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

/*This code is taken from the website:
http://www.linuxquestions.org/questions/linux-software-2/how-to-copy-paste-into-vim-466153/ */

/*
AF_INET: Address family - this is IP cersion 4
SOCK_STREAM: TYPE -> TCP protocol
Protocol - o
*/


int main(int argc , char *argv[])
{   
    
    int socket_desc;
    struct sockaddr_in server;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
         
    server.sin_addr.s_addr = inet_addr("74.125.224.72");
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );
    
    //Connect to remote server
    if  (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Not connected");
        puts("Connect error");
        return 1;
    }
    
    //printf("Connected"); 
    puts("Connected");
    return 0;
    
}
