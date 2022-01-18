#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
	
#define BUFSIZE 2000
#define SIZE 20
	
static char *a[SIZE];/*stores the client names*/
static int newsockfd;/*unique socket descriptor for client */
	
void send_msg(int fd, char *msg, fd_set *master, int i)
{
	char append[BUFSIZE] = "received from ";
	
	if (FD_ISSET(fd, master)){
		strcat(append, a[i]);
		strcat(append, ":");
		strcat(append, msg);
		
		if (send (fd, append, strlen(append), 0) == -1){
			perror("send");
		}else
			send(i, "message sent", strlen("message sent"), 0);
	}
	
}
	
int find(int fdmax, char *to_name)
{
	int i;
		
	for (i = 0; i<=fdmax ; i++){
		if((a[i] != NULL) && (strcmp(to_name, a[i]) == 0)){
			return i;
		
		}
	}
	return 0;
}
		
void talking(char *tokens, char *msg, char *to_name, int fdmax, int i, fd_set *master)
{
	int fd;
	
	if (strcmp(tokens, "talk") == 0){
		fd = find(fdmax, to_name);
		send_msg(fd, msg, master, i);
	}else{
		a[newsockfd] = strdup(tokens);
	}
}
		
	
void msg_parse(char *recv_buf, char *tokens, char *to_name, char *msg)
{
	int j, k;
	
	for(j = 0; recv_buf[j] != ' ' && recv_buf[j] != '\0'; j++)
		tokens[j] = recv_buf[j];
	tokens[j] = '\0';
	if (recv_buf[j] != '\0'){
		j++;
		for(k = 0; recv_buf[j] != ' '; j++, k++)
			to_name[k] = recv_buf[j];
		to_name[k] = '\0';
		j++;
		for(k = 0; recv_buf[j] != '\n'; k++, j++)
			msg[k] = recv_buf[j];
		msg[k] = '\0';
	}
}
void send_list(int fdmax, fd_set *master, int i)
{
	char name[SIZE];
	int j;
	
	for(j = 4; j <= fdmax; j++){
		if(FD_ISSET(j, master) && FD_ISSET(i, master)){
			strcpy(name, a[j]);
			strcat(name, "\n");
			send (i, name, strlen(name), 0);
		}
	}
}
void data_recv(int *fdmax, fd_set *master, int *i)
{
	int nbytes_recvd, n;
	char recv_buf[BUFSIZE];
	char msg[BUFSIZE];
	char tokens[SIZE];
	char to_name[SIZE];
		
	for (n = 0; n < BUFSIZE; n++)
		recv_buf[n] = '\0';
	if ((nbytes_recvd = recv(*i, recv_buf, BUFSIZE, 0)) <= 0) {
		if (nbytes_recvd == 0) {
			printf("selectserver: socket %d hung up\n", *i);
		}else {
			perror("recv");
		}
		close(*i);
		FD_CLR(*i, master);
	}else{
		if (strncmp(recv_buf, "list\n", 4) == 0){
			send_list(*fdmax, master, *i);
		}else{
			msg_parse(recv_buf, tokens, to_name, msg);
			talking(tokens, msg, to_name, *fdmax, *i, master);
		}
	}
}


void connection_accept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr)
{
	socklen_t addrlen;
	
	addrlen = sizeof(struct sockaddr_in);
	if((newsockfd = accept(sockfd, (struct sockaddr *)client_addr, &addrlen)) == -1) {
		perror("accept");
		exit(1);
	}else {
		FD_SET(newsockfd, master);
		if(newsockfd > *fdmax){
			*fdmax = newsockfd;
		}
		printf("selectserver: new connection from %s on port %d \n",
			inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
	}
}
void connect_request(int *sockfd, struct sockaddr_in *my_addr, int port)
{
	int yes = 1;

	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	my_addr->sin_family = AF_INET;
	my_addr->sin_port = htons(port);
	my_addr->sin_addr.s_addr = INADDR_ANY;
	memset(my_addr->sin_zero, '\0', sizeof my_addr->sin_zero);
	
	if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if (bind(*sockfd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(1);
	}
	if (listen(*sockfd, 10) == -1) {
		perror("listen");
		exit(1);
	}
	printf("\nTCPServer Waiting for clients on port %d\n", port);
	fflush(stdout);
}
		
void command_parse(int argc, char *argv[], int *port)
{
	if (argc != 3){
		printf("usage : server -p port\n");
		exit(1);
	}
	else if(argc == 3){
		if (strcmp(argv[1], "-p") == 0)
			*port = atoi(argv[2]);
	}
	
}
	

int main(int argc, char *argv[])
{
	int i, fdmax, port;
	struct sockaddr_in my_addr, client_addr;
	fd_set master;
	fd_set read_fds;
	int sockfd = 0;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	command_parse(argc, argv, &port);
	connect_request(&sockfd, &my_addr, port);
	FD_SET(sockfd, &master);
	fdmax = sockfd;
	while(1){
		read_fds = master;
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(1);
		}
		for (i = 0; i <= fdmax; i++){
			if (FD_ISSET(i, &read_fds)){
				if (i == sockfd)
					connection_accept(&master, &fdmax, sockfd, &client_addr);
				else
					data_recv(&fdmax, &master, &i);
			}
		}
	}
	close(sockfd);
	return 0;
}

