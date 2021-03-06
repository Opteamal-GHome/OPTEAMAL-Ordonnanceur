//Module containing basic operations allowing to use a tcp server
//Author(s) : Rapahel C.
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ipcs.h"

#define forward_address "127.0.0.1"
#define forward_port 1337

int clientIni(FILE * output)
{
	/*
	struct ifaddrs * iflist;
	char loopback[]="127.0.0.1"; 
	char address[16];
	struct sockaddr_in* addr;
	*/
	int opt = 1;
	struct sockaddr_in server_address;
	int socketd;
	uint16_t port = htons(forward_port);
	fprintf(output,"Creating client\nInitializing...");
	/*
	ifaddrs * interface;

	if (getifaddrs(&iflist) == -1) {
		fprintf(output,"Failed to get iflist\n");
	}
	fprintf(output,"Available interfaces :")
	for (interface = iflist; interface != NULL; interface = interface->ifa_next){
		if (interface->ifa_addr == NULL || interface->ifa_addr->sa_family != AF_INET) {
			continue;
		}
		addr = (sockaddr_in*)interface->ifa_addr;
	   	inet_ntop(AF_INET, &addr->sin_addr,address,ADDR_SIZE);
		fprintf(output,"\t %s : s\n",interface->ifa_name,address);
		if (strcmp(address,loopback)==0){
			//this is a good address, stop looking any further
			fprintf("Found %s\n",ad);
			break;
		}
		
	}
	freeifaddrs(iflist);
	*/
	//We probably don't need that
	socketd = gsocket(AF_INET, SOCK_STREAM, 0);
	if(socketd == -1) {
		return -1;
	}
	if (setsockopt(socketd,SOL_SOCKET,SO_REUSEADDR,&opt,4)) {
		perror("setsockopt ");
		return -1;
	}
	
	memset(&server_address,0,sizeof(struct sockaddr_in)); //clear struct
	server_address.sin_family = AF_INET; //ipv4

	if (inet_pton(AF_INET,forward_address,&server_address.sin_addr) == -1) {
		perror("inet_pton ");
		return -1;
	}
	server_address.sin_port = port; 
	if (connect(socketd,(const struct sockaddr *)&server_address,sizeof(server_address))) {
		perror("connect ");
		return -1;
	}
	printf("Client Initialized\n");
	return socketd;

}
int initServer(listen_port){

	int listen_socketd;
	uint16_t port;
	struct sockaddr_in own_address;
	int options = 1;

	port = htons(listen_port);
	listen_socketd = gsocket(AF_INET, SOCK_STREAM, 0);

	if(listen_socketd == -1) {
		perror("socket creation ");
		return -1; //fail to create new socket
	}
	if (setsockopt(listen_socketd,SOL_SOCKET,SO_REUSEADDR,&options,4)) {
		perror("setsockopt ");
	}
	memset(&own_address,0,sizeof(struct sockaddr_in)); //clear struct
	own_address.sin_family = AF_INET; //ipv4
	own_address.sin_port = port; //listening port
	own_address.sin_addr.s_addr = INADDR_ANY; //own address... 

	if (bind(listen_socketd, (struct sockaddr *) &own_address, sizeof(own_address)) < 0) {
		//fail to name socket;
		perror("bind ");
		return -1;
	}

	if (listen(listen_socketd,1) < 0)
	{
		perror("listen ");
		return -1;
	}
	printf("Server listenning on port %d\n",ntohs(port));
  return listen_socketd;
}

int waitClient(int listenSocket){

	struct sockaddr_in client_address;
	int request_socketd;

	socklen_t client_size = sizeof(client_address);

  request_socketd = gaccept(listenSocket,\
    (struct sockaddr *) &client_address, &client_size);
  if (request_socketd == -1)
  {
    perror("accept ");
    return -1;	
  }
  printf("new connection\n"); 
  printf("\tremote address : %s\n", inet_ntoa(client_address.sin_addr));
  printf("\tremote port : %d\n", ntohs(client_address.sin_port));

	return request_socketd;
}

void closeClient(int clientSock){

  printf("Client deconected\n");
  shutdown(clientSock, SHUT_RDWR);
	close(clientSock);
}
int receive(int socket, char * buff, int size){
  size_t ret = 0;
  size_t bytesRcv = 0;
  int i=0;
  for (bytesRcv=0; bytesRcv<size; ){
    ret=recv(socket,(void*)(buff+bytesRcv),size-ret,0);
    bytesRcv+=ret;
    if (ret==0){
      closeClient(socket);
      return -1;
    }else if(ret==-1){
      perror("recv on socket failed");
      return -1;
    }else{
      printf("Received %d bytes.\n",ret);
      for (i=0; i<ret; i++){
        printf("%hhx ",buff[i]);
      }
    }
  }
  return ret;
}

int transmit(int socket, char * buff, int size){
  if (send(socket, buff, size,0)==-1)
  {
    perror("send on socket failed"); 
    return -1;
  }
  return 0;
}
