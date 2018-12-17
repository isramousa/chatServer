 
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


struct argsSend {
	char name[LENGTH];
	int socketNumber;
};


void* sendDataThread(void* args)
{
	char messageSent[MAX_LEN];
	char message[MAX_LEN - 24];
	char *clientName = ((struct argsSend*)args)->name;
	int mysocket = ((struct argsSend*)args)->socketNumber;
	while(1){
	messageSent[0]='\0';
	message[0]='\0';
	printf("Enter the msg you want to send:\n");
	scanf("%s", message);
	strcat(messageSent,clientName);
	strcat(messageSent,message);

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
		if(recv(mysocket,messageRecd, sizeof(messageRecd),0) < 0 ){
			printf("error in connection\n");
			break;
		}
		printf("message recived from server:\n %s",messageRecd);
	}

}

   

 
int main(int argc, char *argv[])
{
   pthread_t rcvThread, sndThread;
   int  mysocket, rcvThStatus, sndThStatus;
   struct sockaddr_in dest; 

   int *socketArg = malloc(sizeof(*socketArg));
   struct argsSend *client = (struct argsSend *)malloc(sizeof(struct argsSend));   

   mysocket = socket(AF_INET, SOCK_STREAM, 0);
   	
   //fill the info of client need to send
   printf("please enter your name:\n");
   scanf("%s",&client->name);
   client->socketNumber = mysocket;
   // add : to name
   size_t cur_len = strlen(client->name);
	if(cur_len < LENGTH -2) {
		client->name[cur_len] = ':';
	}
  

   *socketArg = mysocket;	
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORT);                /* set destination port number */
	
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));
   pthread_create(&rcvThread,NULL,&recvDataThread,(void *)socketArg);
   pthread_create(&sndThread,NULL,&sendDataThread,(void *)client);
   pthread_join(rcvThread,NULL);
   pthread_join(sndThread,NULL);

   return EXIT_SUCCESS;
}
