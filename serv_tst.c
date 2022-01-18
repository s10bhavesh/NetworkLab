#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(char *msg)	//To print error, pass the value to *msg, which is a pointer, converting to character array.
{	perror(msg); exit(1);	}	//exit due to abnormal termination


int main(int argc , char *argv[])	//argc= #arguments, argv accepts the arguments.
{				
	int sockfd, newsockfd, portno, clilen, n;
	char buffer[256];	// send/recv values to/from server/client. Can also create 2 separate for serv/cli
	struct sockaddr_in serv_addr, cli_addr;	//serv_addr has all the server addresses.

if(argc<2) { fprintf(stderr,"ERROR: No port provided\n"); exit(1); }

sockfd= socket(AF_INET, SOCK_STREAM, 0);	// AF_INET for internet address concept(IP). SOCK_STREAM for specifying TCP concept. 
						// 0 for (any) leaving to the system to get the appropriate socket.
if(sockfd<0) { error("ERROR OPENING SOCKET");	}

bzero((char *) &serv_addr, sizeof(serv_addr));	//bzero for clearing the server address.

portno=atoi(argv[1]);	//portno stored at the 1st argument, which is to be converted to integer.

serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;	//does itself
serv_addr.sin_port=htons(portno);	//converts to network format

if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
{ error("Error on Binding"); }

listen(sockfd,5);	//specifying the number of clients at a time.
clilen=sizeof(cli_addr);

newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	//cli_addr has all the values
if(newsockfd<0)	{ error("ERROR ON ACCEPTING"); }

bzero(buffer,256);

n=read(newsockfd, buffer, 255);	//read from the buffer which has all the info, from the buffer of size 255

if(n<0)	{ error("ERROR READING FROM SOCKET"); }

printf("Here is the message: %s\n", buffer);

n= write(newsockfd,"I got your message", 18);

if(n<0)	{ error("ERROR WRITING TO SOCKET"); }

return 0;



}
