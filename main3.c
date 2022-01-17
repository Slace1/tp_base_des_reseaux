# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

//Reservation d'un socket


int main(void){
	int sock ;
	if ((sock = socket(AF_INET , SOCK_DGRAM, 0)) < 0) {
		perror ("socket error") ;
	}
	else{
		printf("%d\n", sock);
	}
	exit(EXIT_SUCCESS);
}


// int sock=socket(result->ai_family, result->ai_socktype, result->ai_protocol);
