#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>


struct msgDetails
   {
      char username[20];//Msg sender(max size=20)
      char password[20];
      char receiver[15][20];//Usernames of users whose msgs are stored
      char message[15][1000];//Received msgs(max size=1000 bytes)
      char messageid[15][20];//Unique id for each and every msg
      int msgCount;//No of messages received
      int recvMsg;//Flag that indicates that there are new msgs in the inbox
      int active;//Flag for status of the user
   };

struct msgDetails client[15];//Max capacity of 15 clients
struct sockaddr_in servaddr;
socklen_t len=sizeof(servaddr);

int activeCount=0;
int userCount=0;
char port[10];


main(int argc,char *argv[])
   {
      char msg[100];
      int i,j,n,sockid;
      char username[20];
      char password[20];
      char temp[30],temp1[30],op;

      if(argc!=2)
         {
            fprintf(stderr,"Specify port number \n");
            exit(1); 
         }
         
      printf("\n\t\t###### INSTANT MESSENGER SERVER ######\n");
      
      strcpy(port,argv[1]);
      if((sockid=socket(AF_INET,SOCK_DGRAM,0))<0)//socket created
         {
            printf("Error creating socket!!!!!!! :-(");
            exit(1);
         }
      printf("\n Socket created successfully :-) \n");

      bzero((char*)&servaddr,sizeof(servaddr));
      servaddr.sin_family=AF_INET;
      servaddr.sin_port=htons(atoi(port));
      servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
      
      if(bind(sockid,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)//Binding the socket
         {
            printf("Bind error!!!!!! :-( ");
            exit(1);
         }

      for(i=0;i<15;i++)
         client[i].msgCount=0;

      while(1)
         {
            n=recvfrom(sockid,msg,100,0,(struct sockaddr*)&servaddr,&len);//receive register msg from client
            if(n==-1)
               {
                  perror("Message not received");
                  exit(1);
               }
            printf("\nMsg received\n");
            msg[n]='\0';  
            printf("%s\n",msg);
            
            if(strstr(msg,"register "))
               {
                  sscanf(msg,"%*s%s",username);       
                  if(strlen(username)<20)      
                     {
                        for(i=0;i<userCount;i++)
                           {
                              if(!strcmp(client[i].username,username))
                                 break;
                           } 
                        if(i==userCount)
                           {
                              printf("\nRegistration successful\n");
                              sprintf(msg,"registration ok \n");
                              strcpy(client[userCount].username,username);
                              client[userCount].active=1;
                              userCount++;
                              activeCount++;
                           }
                        else
                           {
                              if(client[i].active==0)
                                 {
                                    client[i].active=1;
                                    activeCount++;
                                 }
                              else          
                                 sprintf(msg,"registration  error 001\n");         
                           }
                     }
                  else
                     sprintf(msg,"registration  error 002\n");
                  sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));//Sends registration status
               }
               
            else if(strstr(msg,"listusers"))
               {
                  sscanf(msg,"%s",username);         
                  for(i=0;i<userCount;i++)
                     {
                        if(!strcmp(client[i].username,username))
                           break;
                     } 
                  if( (i==userCount) || (client[i].active==0) )
                     {
                        sprintf(msg,"Registration Required");
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                     }  
                  else
                     {
                        /*activeCount=0;
                        for(i=0;i<userCount;i++)
                           if(client[i].active==1)
                              activeCount++;*/          
                        sscanf(msg,"%*s%*s%*s%s",temp);
                        sprintf(msg,"userlist  reqid:%s  No of Users:%d",temp,activeCount);
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));//Send the list request status
                        for(i=0;i<userCount;i++)
                           {
                              if(client[i].active==1) 
                                 {
                                    sprintf(msg,"User %d : %s",i+1,client[i].username);
                                    sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));//Sends the list of active users
                                 }
                           }
                        sprintf(msg,"eol");//eol ; End of list
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));//End of list
                     }
               }
               
            else if(strstr(msg,"quit"))
               {
                  sscanf(msg,"%s",username);         
                  for(i=0;i<userCount;i++)
                     {
                        if(!strcmp(client[i].username,username))
                           break;
                     }
                  if( i==userCount || (client[i].active==0) )
                     {
                        sprintf(msg,"Not Registered");
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
                     }  
                  else
                     {   
                        client[i].active=0;
                        activeCount--;
                        sprintf(msg,"User successfully logged out........\n");
                        sendto(sockid,msg,sizeof(msg),0,(struct sockaddr*)&servaddr,sizeof(servaddr));     
                     }
               }
               
            else if(strstr(msg,"send"))
               {      
                  sscanf(msg,"%s",username);         
                  for(i=0;i<userCount;i++)
                     {
                        if(!strcmp(client[i].username,username))
                           break;
                     }
                  if( i==userCount || (client[i].active==0) )
                     {
                        sprintf(msg,"Registration Required");
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                     }
                  else
                     {
                        sscanf(msg,"%*s%*s%*s%s",temp);         
                        for(i=0;i<userCount;i++)
                           {
                              if(!strcmp(client[i].username,temp))
                                 break;
                           }
                        if(i==userCount || (client[i].active==0) )
                           {
                              sprintf(msg,"The Receiver is not Available");
                              sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
                           }
                        else
                           {   
                              client[i].recvMsg=1;                        
                              client[i].msgCount++;
                              strcpy(client[i].receiver[client[i].msgCount],username);
                              sscanf(msg,"%*s%*s%*s%*s%s",client[i].messageid[client[i].msgCount]);
                              sprintf(msg,"Receiver is Available");
                              sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                                   
                              n=recvfrom(sockid,msg,100,0,(struct sockaddr*)&servaddr,&len);       	
                              if(n==-1)
                                 {
                                    perror("Message not received");
                                    exit(1);
                                 }
                              msg[n]='\0';  
                              strcpy(client[i].message[client[i].msgCount],msg);
                              printf("%s\n",client[i].message[client[i].msgCount]);
                           }  
                  }
               }
               
         else if(strstr(msg,"read"))
               {
                  sscanf(msg,"%s",username);         
                  for(i=0;i<userCount;i++)
                     {
                        if(!strcmp(client[i].username,username))
                           break;
                     } 
                  if( i==userCount || (client[i].active==0) )
                     {
                        sprintf(msg,"Registration Required");
                        sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
                     }  
                  else
                     {
                        if(client[i].recvMsg==1)  
                           {
                              for(j=client[i].msgCount;j>=1;j--)
                                 {          
                                    sprintf(msg,"Message from %s MsgId %s",client[i].receiver[j],client[i].messageid[j]);
                                    sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                                    sprintf(msg,"%s",client[i].message[j]);
                                    sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                                 }
                              //sprintf(msg,"eom");//eom : End of Msgs
                              sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));        
                           } 
                        else
                           {
                              sprintf(msg,"eom");
                              sendto(sockid,msg,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));  
                           }             

                     }
               }

         }
   }
