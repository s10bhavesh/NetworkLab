#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>

#define PORT "3490"
#define BACKLOG 10

int main()
{
	int sockfd, new_fd, n;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int yes=1;
	char buf1[100], s[100], buf[100];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, PORT, &hints, &servinfo);
	for(p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
		continue;
		}
		break;
	}
	listen(sockfd, BACKLOG);
	while(1) {
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		inet_ntop(their_addr.ss_family, &(((struct sockaddr_in*)&their_addr)->sin_addr),s, sizeof s);
		//printf("got connection from %s\n", s);
		n  =  recv(new_fd, buf1, 100, 0);
		buf1[n] = '\0';
		printf("Client\t%s\n", buf1);
		printf("Server:\t");
		scanf("%s", buf);
		send(new_fd, buf, 100, 0);
		close(new_fd);
	}
	return 0;
}
