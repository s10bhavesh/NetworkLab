#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#define MAX 80
#define PORT 43454
#define SA struct sockaddr

struct rec
 { char user[20];
   char pass[20];
 }h[100];
void func(int sockfd);
void func1(int sockfd);
void a()
 {
  int i,j;
   for(i=0;i<100;i++)
    {
     strcpy(h[i].user,0);
     strcpy(h[i].pass,0);
    }
 } 
int main()
 {
  
  int sockfd,connfd,len,pid,status;
   struct sockaddr_in servaddr,cli;

   sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
      {
       printf("socket creation failed...\n");
       exit(0);
      }
    else
          printf("Socket successfully created...\n");
     bzero(&servaddr,sizeof(servaddr));
     servaddr.sin_family=AF_INET;
     servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
     servaddr.sin_port=htons(PORT);
     if((bind(sockfd,(SA*)&servaddr, sizeof(servaddr)))!=0)
       {
         printf("socket bind failed...\n");
         exit(0);
        }
      else
           printf("Socket successfully binded...\n");
     if((listen(sockfd,5))!=0)
         {
           printf("Listen failed...\n");
           exit(0);
          }
     else
          printf("Server listening..\n");
 
    
      len=sizeof(cli);
       connfd=accept(sockfd,(SA *)&cli,&len);
        if(connfd<0)
          {
            printf("server acccept failed...\n");
             exit(0);
          }
        else
            printf("server acccept the client...\n");
                                                         
            func1(connfd);
  
      close(sockfd);
   return 0;
 }

void func1(int sockfd)
  {
   char buff[MAX],abc[20];
    int n,i,j;
    a();
      
 l1:  
      bzero(buff,MAX);
      read(sockfd,buff,sizeof(buff));
       
      printf("From client username: %s\t",buff); 
       i=0; 
       while(strcmp(h[i].user,0) != 0)
         {
            if(strcmp(h[i].user,buff) == 0)
              break;
           else i++;
         }
     if(strcmp(h[i].user,0) != 0)  
       {
           bzero(buff,MAX);                printf("kya hua bhai ");
         strcpy(buff,"all ready exist try other");
         write(sockfd,buff,sizeof(buff));
         printf("wait for another user ....     ");
        goto l1;
       }
     else
       { 
         strcpy(h[i].user,buff);
         bzero(buff,MAX);                printf("kya hua bhai ");
         strcpy(buff,"enter your password");
         write(sockfd,buff,sizeof(buff));
        }
       bzero(buff,MAX);
      read(sockfd,buff,sizeof(buff));
       
      printf("From client password : %s ",buff);  //puts(buff); printf("\n");
   
        strcpy(h[i].pass,buff);

         bzero(buff,MAX);
         strcpy(buff,"welcome you logedin in chat server .......");
         write(sockfd,buff,sizeof(buff));

        func(sockfd);
    }


void func(int sockfd)
  {
   char buff[MAX];
    int n;
   for(;;)
    {
     bzero(buff,MAX);
     read(sockfd,buff,sizeof(buff));
       
      printf("From client: %s\t",buff); 
      
         printf("TO Clint :  ");

      bzero(buff,MAX);
      n=0;
      while((buff[n++]=getchar())!='\n');
      write(sockfd,buff,sizeof(buff));
      if(strncmp("exit",buff,4)==0)
       {
        printf("Server Exit...\n");
        break;
        }
      }  
    }
