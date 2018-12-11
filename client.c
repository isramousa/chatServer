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
   char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   int  mysocket,choice,num,numbers;
   struct sockaddr_in dest; 
   char messageSent[250];
   char messageRecd[250];

   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORT);                /* set destination port number */
 
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));


    while(1)
   { 
	printf("enter the message\n");
	fgets(messageSent,sizeof(messageSent),stdin);
	if((send(mysocket,messageSent,strlen(messageSent),0))==-1) {
		printf("Failure sending message!\n");
		close(mysocket);
		exit(1);
	}
	if(num = recv(mysocket,messageRecd, sizeof(messageRecd),0) < 0 ){
		printf("error in connection\n");
		break;
	}

	printf("message recived from server: %s\n",messageRecd);	
}
    /*while(1){
	printf("Enter the number to make an operation\n");
        printf("1. send data to server\n");
        printf("2. close the connection\n");

        scanf("%d",&choice);
	switch(choice)
	{
		case  1 :
		    while(1){
			printf("enter the message\n");
			scanf("%s", messageSent);
			//fgets(message,sizeof(message),stdin);
			if((send(mysocket,messageSent,strlen(messageSent),0))==-1) {
				printf("Failure sending message!\n");
				close(mysocket);
				exit(1);
			}
			else {
				printf("message send from client: %s \n",messageSent);
				num = recv(mysocket,messageRecd, sizeof(messageRecd),0);
				if( num <= 0)
				{
					printf("error in connection\n");
					break;
				}
				messageRecd[num]='\0';
				printf("message recived from server: %s\n",messageRecd);

			}
		   }
			break;		
		
		case  2 :
			close(mysocket);
			exit(0);
			break;
		default: 
			close(mysocket);
			exit(0);
			break;

 }
}*/

   return EXIT_SUCCESS;
}