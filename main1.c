#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char const *argv[]) {
	struct addrinfo hints;
	struct addrinfo *res, *tmp;
	char host[256];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	if (argc !=2) { //Test si on a les bon arguments
		printf("Mettre comme argument une fonction, un hebergeur et un fichier");
		exit(EXIT_FAILURE);
	}
	else{
//QUestion 2:
		int ret = getaddrinfo(argv[1], NULL, &hints, &res);
		if (ret != 0) { //Gestion des erreurs
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
			exit(EXIT_FAILURE);
		}

		for (tmp = res; tmp != NULL; tmp = tmp->ai_next) { //On recupere les infos grace a getnameinfo
			getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
			puts(host);
		}
	}
	freeaddrinfo(res);
	exit(EXIT_SUCCESS);
}


//Cette fonctiion retourne l'IP de l'adresse de l'hebergeur rentrée
//L'IP"peut changer pour un même site selon les connections utilisées
