#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define MAXRCVLEN 500
#define PORT 8080
 
int main(int argc, char *argv[])
{
    int index=0;
    int i,tempSocket;
    int clientsSockets[10];
    char buffer[MAXRCVLEN + 1];  
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int mysocket,len,consocket;            /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);

    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORT);           /* set the server port number */    

    mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));
    printf("binding\n");
    /* start listening, allowing a queue of up to 1 pending connection */
    listen(mysocket, 10);
    printf("listening\n");
    //int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
    //printf("first %d",consocket);
     
     //initialize the array:

     for(i=0; i<10;i++){
		clientsSockets[i]=-1;
     }
     while(1) {
	     consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	     if(consocket < 0){
			printf("error in accepting\n");
			exit(0);
	     } 
		//printf("server got connection\n");
        printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));
		while(1) {
				/*if(clientsSockets[index]==-1){
					clientsSockets[index]=consocket;
					tempSocket=consocket;
					index=(index+1)%10;
				}*/
				if ((len = recv(consocket, buffer, MAXRCVLEN,0))== -1) {
					perror("recv");
					exit(1);
				}
				else if (len == 0) {
					printf("Connection closed\n");
					//So I can now wait for another client
					break;
				}
				buffer[len] = '\0';
				printf("Server:Msg Received %s\n", buffer);

				//for(i=0;i<10;i++){
					//if(clientsSockets[i]!=-1){
					//printf("%d",clientsSockets[i]);
						if ((send(consocket,buffer, strlen(buffer),0))== -1) 
						{
							printf("Failure Sending Message\n");
							//clientsSockets[i]=-1;
							//close(clientsSockets[i]);
							close(consocket);
							break;
						}

						printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",buffer, strlen(buffer));
						//close(consocket);
						//clientsSockets[i]=-1;
						//close(clientsSockets[i]);
						
						//}
					//}
				
			} 
        close(consocket);
  }
close(mysocket);
    return EXIT_SUCCESS;
}