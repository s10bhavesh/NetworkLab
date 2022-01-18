#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(char *msg)	//To print error, pass the value to *msg, which is a pointer, converting to character array.
{	perror(msg); exit(0);	}	//exit due to abnormal termination


int main(int argc , char *argv[])	//argc= #arguments, argv accepts the arguments.
{				
	int sockfd, newsockfd, portno, clilen, n;
	char buffer[256];	// send/recv values to/from server/client. Can also create 2 separate for serv/cli
	struct sockaddr_in serv_addr, cli_addr;	//serv_addr has all the server addresses.
	struct hostent *server;
if(argc<3) { fprintf(stderr,"Usage %s hostname port\n", argv[0]); exit(1); }	// <3 since in client we use "./a.out localhost port#"

portno=atoi(argv[2]);

sockfd= socket(AF_INET, SOCK_STREAM, 0);	// AF_INET for internet address concept(IP). SOCK_STREAM for specifying TCP concept. 
						// 0 for (any) leaving to the system to get the appropriate socket.
if(sockfd<0) { error("ERROR OPENING SOCKET");	}

server=gethostbyname(argv[1]); //getting the localhost/IP addr in the server

if (server==NULL) {fprintf(stderr, "ERROR, No such host\n"); exit(0);}

bzero((char *) &serv_addr, sizeof(serv_addr));	//bzero for clearing the server address.

serv_addr.sin_family=AF_INET;

bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);  //serv_addr.sin_family=AF_INET
										//serv_addr.sin_family=AF_INET
serv_addr.sin_port=htons(portno);

if (connect(sockfd, &serv_addr, sizeof(serv_addr))<0)	{ error("Error connecting"); }

printf("Please enter the message:\n");
bzero(buffer,256);
fgets(buffer,255,stdin);

n=write(sockfd, buffer, strlen(buffer));	//sockfd is having socket details. value is stored in buffer

if(n<0) { error("Error reading from socket"); }

printf("%s\n",buffer);

return 0;	}

