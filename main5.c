#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, char * argv[]){

	//dEF DES VARIABLES
	struct addrinfo hints;
	struct addrinfo *res;
	char wrq[50];


	memset(&hints, 0, sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=IPPROTO_UDP;



	if ( getaddrinfo(argv[1], "1069", &hints, &res) !=0){
		printf("error getaddrinfo\n");
		exit(EXIT_FAILURE);
	}
//rESERVATION DU SOCKET
	int sk;
	if ((sk = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) <0){
		perror("socket error\n");
		exit(EXIT_FAILURE);
	}
//Requete wrq
	wrq[0]= 0;
	wrq[1]= 2;
	strcpy(wrq+2, argv[2]);
	wrq[2+ strlen(argv[2])]= 0;
	strcpy(wrq+2+strlen(argv[2])+1, "octet");
	wrq[5+3+strlen(argv[2])+1]=0;
	int sizewrq =5+3+strlen(argv[2])+1;
	write(1, wrq, sizewrq);
	write(1, "\n", 1);

	sendto(sk,wrq,sizewrq,0,res->ai_addr, res->ai_addrlen);

	char ack[4];
	recvfrom(sk, ack, 4, 0, res->ai_addr,&(res->ai_addrlen));

	int fd;
	if((fd = open( argv[2], O_RDONLY, S_IRUSR))==0){
		perror("open");
		exit(EXIT_FAILURE);
	}

	char request[516];
	int r=1;
//ACK
	while(ack[1]==4){
		request[0] = 0;
		request[1] = 3;
		request[2] = ack[2];
		request[3] = ack[3]+1;
		r = read(fd , request+4, 512);
		if(r ==0){
			break;
		}
		write(1, request, r+4);
		write(1, "\n", 1);
		printf("boucle\n");
		sendto(sk,request,r+4,0,res->ai_addr, res->ai_addrlen);
		recvfrom(sk, ack, 4, 0, res->ai_addr,&(res->ai_addrlen));
	}
}
