#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>

#define PORT "3490"
#define MAXBUFLEN 100

int main()
{
	struct addrinfo hints, *servinfo, *p;
	int sockfd, numbytes;
	char msg1[MAXBUFLEN], msg2[MAXBUFLEN];
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1", PORT, &hints, &servinfo);
	while(1){
		for (p = servinfo; p != NULL; p = p->ai_next) {
			sockfd= socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				continue;
			}
			break;
		}
		printf("Client:\t");
		scanf("%s", msg1);
		send(sockfd, msg1, strlen(msg1), 0);
		numbytes = recv(sockfd, msg2, 99, 0);
		msg2[numbytes] = '\0';
		printf("Server:\t%s\n", msg2);
		close(sockfd);
	}
	return 0;
}
