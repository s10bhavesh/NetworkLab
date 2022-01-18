#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>


main(int argc,char *argv[])
   {
      int n,sockid,i;
      struct sockaddr_in servaddr;	      
      char username[25],temp[100],recvname[25],password[20];
      char buf[100];
      int reqid=0;
      int msgid=0;

      if(argc!=2)
         {
            fprintf(stderr,"Specify port number \n");
            exit(1);
         }

      printf("\n\t\t###### INSTANT MESSENGER CLIENT ######\n");
      if((sockid=socket(AF_INET,SOCK_DGRAM,0))<0)
         {
            printf(" Error creating socket!!!!!!! :-( ");
            exit(1);
         }   
      printf("\nSocket created successfully\n");

      bzero((char*)&servaddr,sizeof(servaddr));
      servaddr.sin_family=AF_INET;
      servaddr.sin_port=htons(atoi(argv[1]));	 
      servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
      
      if(servaddr.sin_addr.s_addr==INADDR_NONE)
         {
            printf("\nWrong server address\n");
            exit(0);
         }
      int z;
      printf("\nClients do register before use\n");
      
      while(1)
         {
            scanf("%s",temp);
            
            if(!strcmp(temp,"register"))
               {
                  printf("\nServer:Enter a username\n");
                  scanf("%s",username);
 		  printf("\nServer:Enter  password\n");
                  scanf("%s",password);
                  sprintf(buf,"register %s %s localHost %s",username,password,argv[1]);
                  z=sendto(sockid,buf,strlen(buf),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  if(z<0)
                     {
                        printf("\n Error sending msg!!! Exiting...\n");
                        exit(0);
                     }
                  if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                     {
                        perror("UDP Read Error");
                        exit(1);
                     }
                  buf[n]='\0';
                  printf("Server:%s",buf);	  
               }
               
            /*else if(strstr(temp,"list"))
               {
                  sprintf(buf,"%s : listusers %d",username,reqid++);
                  z=sendto(sockid,buf,strlen(buf),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  if(z<0)
                     {
                        printf("\n Error sending msg!!! Exiting...\n");
                        exit(0);
                     }
                  if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                     {
                        perror("UDP Read Error");
                        exit(1);
                     }
                  buf[n]='\0';
                  printf("Server : %s\n",buf);	
                  if(!strstr(buf,"Registration"))
                     {       
                        while(1)
                           {         
                              if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                                 {
                                    perror("UDP Read Error");
                                    exit(1);
                                 }
                              buf[n]='\0';
                              if(!strcmp(buf,"eol"))
                                 break; 
                              printf("\n%s",buf);      
                           }   
                     }
               }*/

            else if(!strcmp(temp,"quit"))
               { 
                  sprintf(buf,"%s : quit",username);
                  sendto(sockid,buf,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                     {
                        perror("UDP Read Error");
                        exit(1);
                     }
                  buf[n]='\0';
                  printf("Server:%s",buf);
                  break;
               }

            else if(strstr(temp,"send"))
               {    
                  printf("Message to :");  
                  scanf("%s",recvname);         
                  sprintf(buf,"%s : send %s %d",username,recvname,msgid++);
                  z=sendto(sockid,buf,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  if(z<0)
                     {
                        printf("\n Error sending msg!!! Exiting...\n");
                        exit(0);
                     }
                  if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                     {
                        perror("UDP Read Error");
                        exit(1);
                     }
                  buf[n]='\0';
                  printf("Server:%s",buf);        
                  if(strstr(buf,"Receiver is Available"))
                     {
                        printf("\nMessage:\n");
                        bzero(temp,sizeof(temp));
                        i=-1;
                        scanf("%c",&temp[i]);
                        do
                           {
                              i++;
                              scanf("%c",&temp[i]);
                           }while(temp[i]!='\n');
                        sprintf(buf,"%s",temp);
                        sendto(sockid,buf,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                     }                         
               }
            else if(strstr(temp,"read"))
               {
                  sprintf(buf,"%s : read",username);
                  sendto(sockid,buf,100,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                     {
                        perror("UDP Read Error");
                        exit(1);
                     }
                  buf[n]='\0';
                  printf("Server:\n%s",buf);        
                  while(1)
                     {
                        if(!strstr(buf,"Registration Required"))
                           {
                              if((n=recvfrom(sockid,buf,100,0,NULL,NULL))==-1)
                                 {
                                    perror("UDP Read Error");
                                    exit(1);
                                 }
                              buf[n]='\0';
                              if(!strstr(buf,"eom"))
                                 printf("\n%s",buf); break;
                           }
                        else
                           break;
                     }     
               }
            printf("\n%s:",username);
         }
   }
