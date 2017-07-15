#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    
    int sockfd = 0;
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    while(1)
    {
        fd_set rfds;
        struct timeval tv;
        int reRet;
        int nfds;

        //Watch stdin (fd 0) to see when it ha input
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(sockfd, &rfds);
        nfds = sockfd+1;

        //Wait up to five seconds
        
        tv.tv_sec = 10000;
        tv.tv_usec = 0;

        reRet = select(nfds,&rfds, NULL, NULL, &tv);
        //printf("reRet: %d\n",reRet);

        if(reRet == -1 )
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
                l=read(0,input_buffer,sizeof(input_buffer));
                if(l>0)
                    write(sockfd,input_buffer,l);
                else
                    break;
            }
            else if(FD_ISSET(sockfd,&rfds))
            {
                int l;
                l=read(sockfd,input_buffer,sizeof(input_buffer));
                if(l>0)
                    write(1,input_buffer,l);
                else
                    break;
            }
            else
            {
                printf("no data recived within 5 seconds");
            }
        }
    }

    close(sockfd);

    return 0;
}
