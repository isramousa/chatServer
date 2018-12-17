 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
 

#define PORT 8080
#define LENGTH 15
#define MAX_LEN 1024

void* sendDataThread(void* args)
{
	char messageSent[MAX_LEN];
	int mysocket = *((int *)args);
	while(1){
	printf("inside send thread\n");
	
	printf("send %d = \n",mysocket);

		scanf("%s", messageSent);
		//strcat with name of send user
		if((send(mysocket,messageSent,strlen(messageSent),0))==-1) {
			printf("Failure sending message!\n");
			close(mysocket);
			exit(1);
		}

	}
}

void* recvDataThread(void* args)
{
	char messageRecd[MAX_LEN];
	int mysocket = *((int *)args);
	while(1){
	printf("inside recive thread\n");
	printf("rcv %d = \n",mysocket);
		if(recv(mysocket,messageRecd, sizeof(messageRecd),0) < 0 ){
			printf("error in connection\n");
			break;
		}
		printf("message recived from server: %s\n",messageRecd);
	}

}


 
int main(int argc, char *argv[])
{
   pthread_t rcvThread, sndThread;
   int  mysocket, rcvThStatus, sndThStatus;
   int *socketArg = malloc(sizeof(*socketArg));
   struct sockaddr_in dest; 
   
   struct argsSend {
	char name[LENGTH];
	int socketNumber;
   }client;
   

   mysocket = socket(AF_INET, SOCK_STREAM, 0);

   //fill the info of client need to send
   printf("please enter your name:\n");
   scanf("%s",&client.name);
   size_t cur_len = strlen(client.name);
	if(cur_len < LENGTH -2) {
		client.name[cur_len] = ':';
	}
   client.socketNumber = mysocket;

   *socketArg = mysocket;	
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORT);                /* set destination port number */
	
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));
   pthread_create(&rcvThread,NULL,&recvDataThread,(void *)socketArg);
   pthread_create(&sndThread,NULL,&sendDataThread,(void *)socketArg);



   return EXIT_SUCCESS;
}
