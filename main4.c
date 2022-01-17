#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 512

int RRQ(char* buf,char* filename,char* mode){  //FOnction pour envoyer une requête RRQ
	int lenFile=strlen(filename);
	int lenMode=strlen(mode);
	buf=calloc((lenMode+1+lenFile+1+3+1),sizeof(char));
	/*buf[0]=0;
	buf[1]=1;

	strcpy(buf+2,filename);
	strcpy(buf+2+lenFile+2,mode);*/
	return lenMode+1+lenFile+1+3+1;
}

int main(int argc, char** argv){

	//Definitions des variables:



	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s, i, j,res;
	size_t len;
	ssize_t nread;
	char buf[BUF_SIZE];
	char * msgBuf;//=malloc(BUF_SIZE*sizeof(char));
	char * rscBuf=malloc(BUF_SIZE*sizeof(char));
	char* filename="zeros1024";
	char* mode="octet";
	int lenFile=strlen(filename);
	int lenMode=strlen(mode);



	//Verification des arguments

	if (argc !=2) {
		fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	/* Obtenir l'adresse du port hebergeur */


	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Autorise IPv4 ou IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagrame socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Tout les protocoles */

	//argv[1]="127.0.0.1";argv[2]="1069";
	s = getaddrinfo(argv[1], argv[2], &hints, &result);


	//Gestion des erreurs
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	i=0;



	/*struct addrinfo* sockBase=result;
	for(i=0;i<result->ai_addrlen;i++){
		s=socket(sockBase->ai_family,sockBase->ai_socktype,sockBase->ai_protocol);
		if (s!=-1) break;
		else sockBase=sockBase->ai_next;
	}*/
	s=-1;
	//result=result->ai_next;



	//reservation des sockets


	while(result!=NULL){
		s=socket(result->ai_family,result->ai_socktype,result->ai_protocol);
		if (s!=-1) break;
		else result=result->ai_next;
	}


	//CONNEXION AU SOCKET


	if(s!=-1) printf("connexion: %d\t%d\n",i,s);//return EXIT_SUCCESS;
	else {printf("connexion: %d\t%d\n",i,s); return EXIT_FAILURE;}




	//res=connect(s,result->ai_addr,result->ai_addrlen);
	//if(res!=-1) printf("%d\n",res);//return EXIT_SUCCESS;
	//else {printf("%d\n",res); return EXIT_FAILURE;}


	for(i=0;i<BUF_SIZE/8;i++){
		rscBuf[i*8]=-1;
		rscBuf[i*8+4]=0;
	}
	rscBuf[0]=12;rscBuf[40]=186;rscBuf[BUF_SIZE-4]=0;
	for(j=0;j<BUF_SIZE/64;j++){
		for(i=0;i<16;i++){
			printf("0x%x ",rscBuf[j*64+i*4]);
		} printf("\n");
	}
	printf("\nfin\n");
	//int longmsg=RRQ(msgBuf,"zeros1024","octet");

	msgBuf=calloc((lenMode+1+lenFile+1+3+1),sizeof(char));
	msgBuf[0]=0;
	msgBuf[1]=1;

	strcpy(msgBuf+2,filename);
	strcpy(msgBuf+2+lenFile+2,mode);
	int longmsg =lenMode+1+lenFile+1+3+1;


	for(i=0;i<longmsg;i++){
		printf("%c,",msgBuf[i]);
	} printf("\n\n");
	//send(s,msgBuf,BUF_SIZE,0);
	//recv(s,rscBuf,BUF_SIZE,0);
	sendto(s,msgBuf,longmsg,0,result->ai_addr,result->ai_addrlen);
	//close(s);
	struct addrinfo *servInfo=malloc(sizeof(struct addrinfo));
	recvfrom(s,rscBuf,BUF_SIZE,0,servInfo->ai_addr,&servInfo->ai_addrlen);


	/*int pid,status;
	pid = fork();
		if (pid!= 0) {
			write(STDOUT_FILENO,"pere\n",strlen("pere\n"));
				struct addrinfo *servInfo=malloc(sizeof(struct addrinfo));
				recvfrom(s,rscBuf,BUF_SIZE,0,NULL,NULL);
			wait(&status);
		} else{
			write(STDOUT_FILENO,"fils\n",strlen("fils\n"));
				sendto(s,msgBuf,BUF_SIZE,0,result->ai_addr,result->ai_addrlen);
			exit(EXIT_SUCCESS);
		}
	printf("status: %d\n",status);*/


	//servInfo->ai_addr,&servInfo->ai_addrlen


	for(j=0;j<BUF_SIZE/64;j++){
		for(i=0;i<16;i++){
			printf("0x%x ",rscBuf[j*64+i*4]);
		} printf("\n");
	}
	printf("\nfin\n");
	close(s);

	printf("byebye\n"); return EXIT_SUCCESS;
}
