/**
 * @agautam2_assignment1
 * @author  Abhishek Gautam <agautam2@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */


//  Code to implement a TCP Server listening incoming connections
//  MBP111.0138.B16
//  System Serial: C02P4SP9G3QH
//  main.cpp
//
//  Created by Abhishek Gautam on 2/7/16.
//  agautam2@buffalo.edu
//  University at Buffalo, The State University of New York.
//  Copyright © 2016 Gautam. All rights reserved.

//#include "../include/global.h"
//#include "../include/logger.h"
#include "clientOperations.h"
#include "clientLocal.h"
#include "Statistics.h"
#include "clientInfo.h"
#include "blockInfo.h"
#include "logger.h"
#include "global.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

#define MAXMESSAGESIZE 1024
#define STDIN 0

using namespace std;

struct clientinfo
{
    char *IPAddress;
    int sourcePort;
    int acceptSocket;
    char *hostname;
    struct clientinfo *next;
};

struct clientStats
{
    //int sno;
    int sentMessages;
    int recvMessages;
    char *hostname;
    char *clientStatus;
    char *IPAddress;
    int listeningPort;
    int socket;
    struct clientStats *next;
};

struct clientLocal
{
    char *IPAddress;
    int sourcePort;
    int acceptSocket;
    char *hostname;
    struct clientLocal *next;
};

class ServerClientChat
{
public: int i, j, r, sockfd, newsockfd, portno, readBytes, returnedInfo, fdMaxNumber;
public: int sockfd2, numbytes;
public: struct addrinfo hints, hints_c, *serverInfo, *p;
public: struct sockaddr_storage clientAddress, localIPAddress;
public: socklen_t clientLength;
public: int yes;
public: char messageBuffer [MAXMESSAGESIZE];
public: char ipBuffer [MAXMESSAGESIZE];
public: char storeAddress[INET6_ADDRSTRLEN];
public: char storeLocalAddress[INET6_ADDRSTRLEN];
public: char boundAddress[INET6_ADDRSTRLEN];
public: char hostname [128];
public: char host [1024];
public: int listenPort;
public: char service [20];
public: int clientPort;
    
    
    fd_set masterDescriptor;
    fd_set tempRead_fds;
    
    
    struct hostinfo
    {
        char IPAddress[30];
        int sourcePort;
        int acceptSocket;
        char hostname[256];
    };
    
    struct hostinfo hostInformation[5];
    
public: ServerClientChat(char *a)
    {
        if(strcmp(a,"C"))
        {
            gethostname(hostname, sizeof(hostname));
            printf("---------------------------------------------------------------------\n");
            printf("\t establishing server on %s\n", hostname);
            printf("---------------------------------------------------------------------\n");
        }
        else
        {
            gethostname(hostname, sizeof(hostname));
            printf("---------------------------------------------------------------------\n");
            printf("\t establishing client on %s\n", hostname);
            printf("---------------------------------------------------------------------\n");
        }
        
    }
    
public: int establishServer(char *argv)
    {
        int argCounter = 0;
        int sockfdToSend = 0;
        char logoutCheck[30];
        char serverListenPort[30];
        strcpy(serverListenPort,argv);
        
        FD_ZERO(&masterDescriptor);
        FD_ZERO(&tempRead_fds);
        
        memset(&hints, 0, sizeof hints);
        hints.ai_family=AF_UNSPEC;
        hints.ai_socktype=SOCK_STREAM;
        hints.ai_flags=AI_PASSIVE;
        
        returnedInfo = getaddrinfo(NULL, (char *) argv, &hints, &serverInfo);
        
        if(returnedInfo!=0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnedInfo));
            return 1;
        }
        
        for(p=serverInfo; p!=NULL;p=p->ai_next)
        {
            sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);      //SOCKET
            printf("socket file descriptor :%d\n\n", sockfd);
            if(sockfd<0)
            {
                perror("server: socket");
                continue;
            }
            
            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
            {
                perror("setsockopt");
                exit(1);
            }
            
            if(::bind(sockfd, p->ai_addr, p->ai_addrlen) < -1)                      //BIND
            {
                close(sockfd);
                perror("server:bind");
                continue;
            }
            break;
        }
        
        if(p==NULL)
        {
            fprintf(stderr, "server: port binding failed\n");
           // exit(2);
        }
        freeaddrinfo(serverInfo);
        if(listen(sockfd,5) == -1)                                                 //LISTEN
        {
            perror("listen");
            //exit(3);
        }
        
        /*USE OF SELECT() CALL*/
        
        FD_SET(sockfd, &masterDescriptor);
        FD_SET(STDIN, &masterDescriptor);
        fdMaxNumber=sockfd;
        int r=1;
        int dataSocket;
        for(;;) {
            
            char **tokenContainer = (char**)malloc( 1024 * sizeof(char*));
            
            for (i = 0 ; i< 1024 ; i++)
            {
                tokenContainer[i] = (char*)malloc( 2048 * sizeof(char));
            }

            tempRead_fds = masterDescriptor; // copy it
            if (select(fdMaxNumber+1, &tempRead_fds, NULL, NULL, NULL) == -1)      //SELECT
            {
                perror("select");
                exit(4); }
            // run through the existing connections looking for data to read
                for(i = 0; i <= fdMaxNumber; i++)
                {
                    if (FD_ISSET(i, &tempRead_fds))
                    {
                        if (i == sockfd)
                        {
                        
                        printf("found another connection\n");
                        // handle new connections
                        clientLength = sizeof clientAddress;
                        newsockfd = accept(sockfd,(struct sockaddr *)&clientAddress,&clientLength);     //ACCEPT
                            if (newsockfd == -1)
                            {
                            perror("accept");
                            }
                            else
                            {
                            
                            printf("adding to master set\n\n");
                            FD_SET(newsockfd, &masterDescriptor); // add to master set
                                if (newsockfd > fdMaxNumber)
                                {    // keep track of the max
                                fdMaxNumber = newsockfd;
                                printf("fdMaxNumber after adding to master set is: %d\n", fdMaxNumber);
                                
                                }
                                clientLength = sizeof (clientAddress);
                                getpeername(sockfd, (struct sockaddr *) &clientAddress, &clientLength);
                            
                                if(clientAddress.ss_family == AF_INET)
                                {
                                    struct sockaddr_in *peername = (struct sockaddr_in *) &clientAddress;
                                    clientPort = ntohs(peername->sin_port);
                                    inet_ntop(AF_INET, &peername->sin_addr, storeAddress, sizeof (storeAddress));
                                }
                                else
                                {
                                    struct sockaddr_in6 *peername = (struct sockaddr_in6 *) &clientAddress;
                                    clientPort = ntohs(peername->sin6_port);
                                    inet_ntop(AF_INET, &peername->sin6_addr, storeAddress, sizeof (storeAddress));
                                }
                                printf("Accepting new connection from %s: %d on socket %d\n\n", storeAddress, clientPort, newsockfd);
                                printf("Socket %d is bound to %s\n", newsockfd, storeAddress);
                                clientLength = sizeof(clientAddress);
                                getnameinfo((struct sockaddr *) &clientAddress, clientLength, host, sizeof(host), service, sizeof(service), 0);
                                r++;
                        }
                    }       //else of if FD_ISSET
                    
                    else if (FD_ISSET(STDIN,  &tempRead_fds))
                             {
                                 
                                 string str1;
                                 argCounter = 0;
                                 
                                 char **tokenContainer = (char**)malloc( 25 * sizeof(char*));
                                 
                                 for (i = 0 ; i< 25 ; i++)
                                 {
                                     tokenContainer[i] = (char*)malloc( 1024 * sizeof(char));
                                 }
                                 
                                 getline(std::cin, str1);
                                 
                                 std::cout<<"you entered "<<str1<<"\n";
                                 
                                 char *token;
                                 char *ptr;
                                 
                                 ptr= strdup(str1.c_str());
                                 printf("converted string to c: %s\n", ptr);
                                 token = strtok(ptr, " ");
                                 while(token!=NULL)
                                 {
                                     strcpy(tokenContainer[argCounter], token);
                                     argCounter++;
                                     token=strtok(NULL, " ");
                                 }
                                 string command = tokenContainer[0];
                                 int checkFlag=0;
                                //printf("token 0 is: %s\n ", tokenContainer[0]); //------------------------------SERVER command handling-------
                                 
                                 string compareUserInput1 = "AUTHOR";
                                 string compareUserInput2 = "IP";
                                 string compareUserInput3 = "PORT";
                                 string compareUserInput4 = "LIST";
                                 string compareUserInput5 = "STATISTICS";
                                 string compareUserInput6 = "BLOCKED";
                                 for(int i=0;i< command.size();i++)
                                 {
                                     if(isupper(command.at(i)))
                                     {
                                         checkFlag++;
                                     }
                                 }
                                 
                                 if(checkFlag==command.size())                                   //----------------- THE ELSE TO HANDLE CASES
                                 {
                                     //cout<<"Command found in uppercase, moving forward to next logic"<<"\n";
                                     if(command.compare(compareUserInput1)==0)               //if command is author
                                     {
                                         author();
                                     }
                                     else if(command.compare(compareUserInput2)==0)               //if command is IP
                                     {
                                         //THIS WONT WORK ON THE LOCAL SYSTEM AND WOULD CAUSE OTHER COMMANDS TO HANG/FAIL
                                         /*Logic read and understood from - http://easy-tutorials.net/c/linux-c-socket-programming/*/
                                         
                                         int status1,status2,len;
                                         int sockfdExternal;
                                         ssize_t bytesSent;
                                         ssize_t bytesRecv;
                                         struct addrinfo hostdetails;
                                         struct addrinfo *ptrToStruct;
                                         char * msg= "GET / HTTP/1.1\nhost: www.google.com\n\n";
                                         char incomingDataBuf[1024];
                                         
                                         memset(&hostdetails, 0, sizeof hostdetails);
                                         hostdetails.ai_family=AF_INET;
                                         hostdetails.ai_socktype=SOCK_STREAM;
                                         
                                         status1 = getaddrinfo("www.google.com","80", &hostdetails, &ptrToStruct);
                                         if(status1!=0){perror("error making external connection\n");}
                                         //printf("creating socket\n");
                                         sockfdExternal = socket(ptrToStruct->ai_family, ptrToStruct->ai_socktype, ptrToStruct->ai_protocol);
                                        
                                         if(sockfdExternal==-1){perror("sockfdExternal error\n");}
                                         //printf("Now connecting\n");
                                         status2 = connect(sockfdExternal, ptrToStruct->ai_addr, ptrToStruct->ai_addrlen);
                                         if(status2!=0){perror("connect() error\n");}
                                         len=strlen(msg);
                                         bytesSent = send(sockfdExternal, msg, len, 0);
                                         
                                         bytesRecv = recv(sockfdExternal, incomingDataBuf, 1000, 0);
                                         if (bytesRecv == 0) std::cout << "host shut down." << std::endl ;
                                         if (bytesRecv == -1)std::cout << "recieve error!" << std::endl ;
                                         clientLength = sizeof(localIPAddress);
                                         getsockname(sockfdExternal, (struct sockaddr *) &localIPAddress, &clientLength);
                                         
                                         struct sockaddr_in *peername = (struct sockaddr_in *) &localIPAddress;
                                         clientPort = ntohs(peername->sin_port);
                                         inet_ntop(AF_INET, &peername->sin_addr, storeLocalAddress, sizeof (storeLocalAddress));
                                         
                                         whoAmI(storeLocalAddress);
                                     }
                                     
                                     else if(command.compare(compareUserInput3)==0)               //if command is PORT
                                     {
                                         whereAmIListening(serverListenPort);
                                     }
                                     else if (command.compare(compareUserInput4)==0)                //if command is LIST
                                     {
                                         display();
                                     }
                                     else if (command.compare(compareUserInput5)==0)                //if command is STATISTICS
                                     {
                                         displayStats();
                                     }
                                     else if (command.compare(compareUserInput6)==0)                //if command is BLOCKED
                                     {
                                         displayBlockedList(tokenContainer[1]);
                                     }
                                     else
                                     {
                                         printf("[%s FAILED]\n",command.c_str());
                                     }
                                 }
                                 else
                                 {
                                     cout<<"command not in uppercase or invalid"<<"\n";
                                 }
                                 
                                 }//else of STDIN FD_ISSET
                    else
                    {
                        // handle data from a client
                        bzero(ipBuffer, MAXMESSAGESIZE);
                        if ((readBytes = recv(i, ipBuffer, sizeof(ipBuffer) , 0)) <= 0)
                        {
                            //printf("value of i: %d\n",i);
                            //printf("newsockfd is: %d\n", newsockfd);
                            if (readBytes == 0)
                            {
                                printf("selectserver ip: socket %d hung up\n", i);
                            }
                            else
                            {
                                perror("recv ip");
                            }
                            close(i);
                            FD_CLR(i, &masterDescriptor); // remove from master set
                        }
                        else
                        {
                            // we got some data from a client
                            dataSocket = i;
                            printf("ipBuffer is: %s \n", ipBuffer);
                            char *token;
                            char *ptr;
                            argCounter=0;
                            token = strtok(ipBuffer, "-");
                            
                            while(token!=NULL)
                            {
                                strcpy(tokenContainer[argCounter], token);
                                argCounter++;
                                token=strtok(NULL, "-");
                            }
                            //strcpy(tokenContainer[argCounter], token);

                            char *command = tokenContainer[0];  //contains command
                            printf("%s\n", tokenContainer[1]);  //contains IP
                            printf("%s\n", tokenContainer[2]);  //contains Message

                            
                            if(strcmp(command, "LOGIN")==0)
                            {
                                listenPort = atoi(tokenContainer[1]);
                                insertClient(storeAddress, listenPort, newsockfd, host);
                                clientinfo *obj6;
                                obj6=clientSearchBySocket(i);
                                printf("%s logged out %s logged back in\n", logoutCheck, obj6->IPAddress);
                                if(strcmp(logoutCheck,obj6->IPAddress)==0)
                                {
                                    updateClientStatus(obj6->IPAddress);
                                }
                                else
                                {
                                    insertClientStats(0, 0, host, "online", listenPort, storeAddress, i);
                                }
                                char bigBigdata[2048];
                                bzero(bigBigdata, 2048);
                                strcpy(bigBigdata, serialize());
                                printf("SERIALIZED STRING%s\n", bigBigdata);
                                int h = send(i, bigBigdata, sizeof(bigBigdata), 0);
                                if(h<0)
                                {
                                    perror("FAILED TO SEND SERIALIZED DATA");
                                }
                               
                            }
                            else if (strcmp(command, "SEND")==0)
                            {
                                char msgFromIP[MAXMESSAGESIZE];
                                char *abc = tokenContainer[1];

                                clientStats *obj;
                                clientinfo *obj2;
                                obj=clientStatsSearchByIP(abc);//object of reciever
                                
                                obj2 = clientSearchBySocket(i);//object of sender
                                
                                strcpy(msgFromIP, obj2->IPAddress);//IP of sender
                                
                                updateMessagesSentCounter(msgFromIP);
                                
                                bool outcome;
                                outcome=searchForAlreadyBlocked(tokenContainer[1], msgFromIP);
                                if(!outcome)
                                {
                                    if(obj!=NULL && strcmp(obj->clientStatus,"online")==0)
                                    {
                                        clientinfo * obj20;
                                        obj20=clientSearchByIP(abc);
                                        
                                        sockfdToSend=obj20->acceptSocket;
                                        char buffer[MAXMESSAGESIZE];
                                        bzero(buffer,MAXMESSAGESIZE);
                                        strcpy(buffer,"SEND-");
                                        strcat(buffer,msgFromIP);
                                        strcat(buffer,"-");
                                        strcat(buffer, tokenContainer[2]);
                                        int n = send(sockfdToSend, buffer, strlen(buffer),0);
                                        
                                        if(n<0)
                                        {
                                            perror("ERROR LEVEL: send");
                                        }
                                        else
                                        {
                                            updateMessagesRecvCounter(abc);
                                            printf("msg from:%s, to:%s\n[msg]:%s\n", msgFromIP, tokenContainer[1], tokenContainer[2]);
                                        }
                                    }
                                    else if (strcmp(obj->clientStatus,"offline")==0)
                                    {
                                        printf("Requested client not online\n");
                                    }
                                }
                                else
                                {
                                    printf("Message could not be relayed as client is blocked\n");
                                }
                            
                            }
                            else if(strcmp(command, "BLOCK")==0)
                            {
                                printf("inside block\n");
                                char *senderIP;
                                char *abc = tokenContainer[1];
                                printf("IP to block is %s\n",abc);
                                int sockfdToBlock;
                                clientinfo *obj, *obj2;
                                obj=clientSearchBySocket(i);
                                senderIP=obj->IPAddress;
                                
                                obj2=clientSearchByIP(abc);
                                if(obj2==NULL)
                                {
                                    printf("Client is not online");
                                }
                                else
                                {
                                    bool outcome;
                                    outcome=searchForAlreadyBlocked(senderIP, abc);
                                    if(outcome)
                                    {
                                        printf("You have already blocked this client. What do you want more? Consider some Humanity\n");
                                    }
                                    else
                                    {
                                        printf("Ok everybody. Blocking this client now!\n");
                                        clientStats *obj5;
                                        obj5=clientStatsSearchByIP(abc);
                                        printf("%s\n",obj5->hostname);
                                        printf("%d\n",obj5->listeningPort);
                                        if(obj5!=NULL)
                                        {
                                            insertBlockedClient(abc, senderIP, sockfdToBlock, i, obj5->hostname, obj5->listeningPort);
                                        }
                                        //displayBlockedList(senderIP);
                                    }
                                }
                                
                            }
                            else if (strcmp(command, "UNBLOCK")==0)
                            {
                                printf("inside UNBLOCK handling at server\n");
                                char *senderIP;
                                char *abc = tokenContainer[1];
                                printf("IP to block is %s",abc);
                                int sockfdToUnblock;
                                bool outcome;
                                clientinfo *obj;
                                obj=clientSearchBySocket(i);
                                senderIP=obj->IPAddress;
                                outcome=searchForAlreadyBlocked(senderIP, abc);
                                if(outcome)
                                {
                                    deleteBlockedClient(senderIP,abc);
                                }
                                else
                                {
                                    printf("You can't unblock someone you haven't blocked\n");
                                }
                                
                            }
                            else if(strcmp(command, "BROADCAST")==0)
                            {
                                clientinfo *obj14;
                                char broadcaster[30];
                                int flagbcast = 0;
                                obj14=clientSearchBySocket(i);
                                strcpy(broadcaster, obj14->IPAddress);
                                updateMessagesSentCounter(broadcaster);
                                printf("inside broadcast handling at server\n");
                                char *bcastMsg = tokenContainer[1];
                                char buffer[MAXMESSAGESIZE];
                                bzero(buffer,MAXMESSAGESIZE);
                                strcpy(buffer,"BROADCAST-");
                                strcat(buffer, broadcaster);
                                strcat(buffer, "-");
                                strcat(buffer,bcastMsg);
                                
                                for(int j=1; j<=fdMaxNumber; j++)
                                {
                                    if(FD_ISSET(j,&masterDescriptor))
                                    {
                                        if(j!=sockfd && j!=i)
                                        {
                                            clientStats *obj13;
                                            
                                            obj13=clientStatsSearchBySocket(j);
                                            if(obj13!=NULL){
                                            bool outcome1;
                                            char broadcastReceiver[30];
                                            
                                            strcpy(broadcastReceiver, obj13->IPAddress);
                                            
                                            outcome1=searchForAlreadyBlocked(broadcastReceiver, broadcaster);
                                            
                                            if(obj13!=NULL && obj14!=NULL)
                                            {
                                                if (strcmp(obj13->clientStatus, "online")==0 && !outcome1)
                                                {
                                                    int n=send(j, buffer, sizeof(buffer), 0);
                                                    if(n<0)
                                                    {
                                                        perror("ERROR LEVEL: broadcast send");
                                                       
                                                    }
                                                    else
                                                    {
                                                        updateMessagesRecvCounter(broadcastReceiver);
                                                        flagbcast = 1;
                                                    }
                                                }
                                            }

                                            
                                            else
                                            {
                                                perror("NULL value retuned for IP for socket j\n");
                                            }
                                        }
                                        }
                                    }
                                }
                                if(flagbcast==1)
                                {
                                    printf("msg from:%s, to:%s\n[msg]:%s\n", broadcaster, "255.255.255.255", tokenContainer[1]);
                                }
                                else
                                {
                                    //cse4589_print_and_log("[RELAYED:ERROR]\n");
                                    //cse4589_print_and_log("[RELAYED:END]\n");
                                }
                                
                            }
                            else if (strcmp(command, "REFRESH")==0)
                            {
                                
                                char bigBigdata[2048];
                                bzero(bigBigdata, 2048);
                                //char *tempdata = serialize();
                                strcpy(bigBigdata, serialize());
                                printf("SERIALIZED STRING%s\n", bigBigdata);
                                int h = send(i, bigBigdata, sizeof(bigBigdata), 0);
                                if(h<0)
                                {
                                    perror("FAILED TO SEND SERIALIZED DATA");
                                }
                            }
                            else if(strcmp(command, "LOGOUT")==0)
                            {
                                char  IPLoggedOut [30];
                                clientinfo *obj4;
                                obj4=clientSearchBySocket(i);
                                strcpy(IPLoggedOut, obj4->IPAddress);
                                deleteClient(IPLoggedOut);
                                updateClientStatus(IPLoggedOut);
                                strcpy(logoutCheck,IPLoggedOut);
                                printf("This IP logged out:%s\n",IPLoggedOut);
                                
                            }
                            else if(strcmp(command,"EXIT")==0)
                            {
                                char  IPLoggedOut[30];
                                strcpy(IPLoggedOut, tokenContainer[1]);
                                printf("inside EXIT at server handling side\n");
                                if(strcmp(logoutCheck,IPLoggedOut)==0)
                                {
                                    printf("IP %s LOGGED OUT\n",IPLoggedOut);
                                    deleteStatsClient(IPLoggedOut);
                                    
                                }
                                else
                                {
                                    deleteClient(IPLoggedOut);
                                    deleteStatsClient(IPLoggedOut);
                                }
                                
                            }
                            
                        }
                        
                        
                    }
                    
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    //---------------------------------------------------------------TCP CLIENT-------------------------------------------------------------------//
    
public: int establishClient(char *argv)
    {
        int argCounter;
        argCounter=0;
        std::string str1;
        char  temp[256];
        char msgBuf[1024];
        char clientListenPort[30];
        strcpy(clientListenPort, argv);
        fd_set tempRead_fdsC;
        int fdMaxNumberC;
        int logoutFlag=0;
        
                FD_ZERO(&tempRead_fdsC);
                FD_ZERO(&masterDescriptor);
                FD_SET(STDIN,&masterDescriptor);
                int privilegeFlag=0;
                while(1)
                {
                    tempRead_fdsC=masterDescriptor;
                    
                    select(fdMaxNumberC+1, &tempRead_fdsC, NULL, NULL, NULL);
                    
                    char **tokenContainer = (char**)malloc( 25 * sizeof(char*));
                    
                    for (i = 0 ; i< 25 ; i++)
                    {
                        tokenContainer[i] = (char*)malloc( 1024 * sizeof(char));
                    }

                    
                    if(FD_ISSET(STDIN, &tempRead_fdsC))
                    {
                        
                        getline(std::cin, str1);                                       //next step after login
                        char *token;
                        char *ptr;
                        argCounter=0;
                        
                        ptr= strdup(str1.c_str());
                        
                        token = strtok(ptr, " ");
                        
                        while(token!=NULL)
                        {
                            strcpy(tokenContainer[argCounter], token);
                            argCounter++;
                            token=strtok(NULL, " ");
                        }

                        string command = tokenContainer[0];
                        string compareUserInput1 = "AUTHOR";
                        string compareUserInput2 = "IP";
                        string compareUserInput3 = "PORT";
                        string compareUserInput4 = "LOGIN";
                        string compareUserInput5 = "REFRESH";
                        string compareUserInput6 = "SEND";
                        string compareUserInput7 = "BROADCAST";
                        string compareUserInput8 = "BLOCK";
                        string compareUserInput9 = "UNBLOCK";
                        string compareUserInput10 = "LOGOUT";
                        string compareUserInput11 = "EXIT";
                        string compareUserInput12 = "LIST";
                        
                        int checkFlagInner = 0;
                        for(int i=0;i< command.size();i++)
                        {
                            if(isupper(command.at(i)))
                                checkFlagInner++;
                        }
                        
                        
                        if(checkFlagInner==command.size())                          //------------------------ CLIENT COMMAND HANDLING -------------
                        {
                            //printf("Command found in uppercase, moving forward to next logic\n");
                            if(command.compare(compareUserInput1)==0)               //if command is author
                            {   //cse4589_print_and_log("ub");
                                author();
                            }
                            else if(command.compare(compareUserInput2)==0)                  //if command is IP
                            {
                                if(privilegeFlag==1)
                                {
                                clientLength = sizeof(clientAddress);
                                getsockname(sockfd, (struct sockaddr *) &clientAddress, &clientLength);
                                
                                struct sockaddr_in *peername = (struct sockaddr_in *) &clientAddress;
                                clientPort = ntohs(peername->sin_port);
                                inet_ntop(AF_INET, &peername->sin_addr, storeAddress, sizeof (storeAddress));

                                whoAmI(storeAddress);
                                }
                                else
                                {
                                    printf("[%s:ERROR]\n", command.c_str());
                                }
                            }
                            
                            else if(command.compare(compareUserInput3)==0)                  //if command is PORT
                            {
                                if(privilegeFlag==1)
                                {
                                    whereAmIListening(clientListenPort);
                                }
                                else
                                {
                                     printf("[%s:ERROR]\n", command.c_str());
                                }
                            }
                            
                            
                            else if(command.compare(compareUserInput4)==0)                 //if command is LOGIN
                            {
                                //cout<<"login will be fired now\n";
                                //-------------LOGIN-------------
                        
                                char *IP = tokenContainer[1];
                                char *portNumber =  tokenContainer[2];
                                
                                sockaddr_in sa;
                                int h = inet_pton(AF_INET, IP, &(sa.sin_addr));
                                if(h==0)
                                {
                                    printf("IP NOT IN CORRECT FORMAT\n");
                                }
                                
                                if(!(atoi(portNumber)>1024 && atoi(portNumber)<65535))
                                {
                                    printf("PORT IS NOT IN CORRECT FORMAT\n");
                                }
                                
                                memset(&hints, 0, sizeof(hints));
                                hints.ai_family = AF_UNSPEC;
                                hints.ai_socktype = SOCK_STREAM;
                                
                                if((returnedInfo = getaddrinfo(IP , portNumber, &hints, &serverInfo))!=0)
                                {
                                    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnedInfo));
                                }
                                
                                for(p=serverInfo; p!=NULL; p=p->ai_next)
                                {
                                    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
                                    {
                                        perror("client: socket");
                                        continue;
                                    }
                                    printf("sockfd is %d\n",sockfd);
                                    if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
                                    {
                                        close(sockfd);
                                        perror("client: connect");
                                        continue;
                                    }
                                    break;
                                }
                                
                                if(p == NULL)
                                {
                                    fprintf(stderr, "client: failed to connect\n");
                                }
                                
                                clientLength = sizeof (clientAddress);
                                getpeername(sockfd, (struct sockaddr *) &clientAddress, &clientLength);
                                
                                if(clientAddress.ss_family == AF_INET)
                                {
                                    struct sockaddr_in *peername = (struct sockaddr_in *) &clientAddress;
                                    clientPort = ntohs(peername->sin_port);
                                    inet_ntop(AF_INET, &peername->sin_addr, storeAddress, sizeof (storeAddress));
                                }
                                else
                                {
                                    struct sockaddr_in6 *peername = (struct sockaddr_in6 *) &clientAddress;
                                    clientPort = ntohs(peername->sin6_port);
                                    inet_ntop(AF_INET, &peername->sin6_addr, storeAddress, sizeof (storeAddress));
                                }
                                
                                clientLength = sizeof((struct sockaddr_in6 *) &clientAddress);
                                getnameinfo((struct sockaddr *) &clientAddress, clientLength, host, sizeof(host), service, sizeof(service), 0);
                                char portMessage[30];
                                strcpy(portMessage, "LOGIN-");
                                strcat(portMessage, clientListenPort);
                                send(sockfd, portMessage, sizeof(portMessage), 0);
                                printf("Connecting to %-5s%-20s on port %-5d\n\n", storeAddress, host, clientPort);
                                privilegeFlag=1;                                        //set flag for command privilege level
                                FD_SET(sockfd, &masterDescriptor);
                                fdMaxNumberC = (sockfd>STDIN?sockfd:STDIN);
                            }

                           else if(command.compare(compareUserInput5)==0)                   //if command is REFRESH
                            {
                                if(privilegeFlag==1)
                                {
                                    
                                    int b = send(sockfd, "REFRESH", strlen("REFRESH") , 0);
                                    if(b<0)
                                    {
                                        //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                        //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    }
                                    else
                                    {
                                        //cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
                                        //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    }
                                }
                                else
                                {
                                 printf("[%s:ERROR]\n", command.c_str());
                                //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    
                                }
                            }
                            else if(command.compare(compareUserInput6)==0)                  //if command is SEND
                            {
                                
                                if(privilegeFlag==1)
                                {
                                char *IP = tokenContainer[1];
                                strcpy(temp, tokenContainer[2]);
                                
                                for(int i=3;i<=argCounter;i++)
                                {
                                    strcat(temp, " ");
                                    strcat(temp,tokenContainer[i]);
                                    
                                }
                                char *message =  temp;
                                clientLocal *obj;
                                obj=clientLocalSearchByIP(IP);
                                    
                                if(obj==NULL)
                                {
                                    printf("Valid but Inconsistent IP. Try REFRESHing your list\n");
                                    //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    
                                }
                                else
                                {
                                    sendMessage(IP,message,sockfd);
                                    //cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                }
                                }
                            
                                else
                                {
                                    printf("[%s:ERROR]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                }
                            }
                            else if(command.compare(compareUserInput7)==0)                  //if command is BROADCAST
                            {
                                char *bcastMsg;
                                char temp[256];
                                char temp2[356];
                                if(privilegeFlag==1)
                                {
                                    
                                    strcpy(temp, tokenContainer[1]);
                                    for(int i=2;i<=argCounter;i++)
                                    {
                                        strcat(temp, " ");
                                        strcat(temp,tokenContainer[i]);
                                        
                                    }
                                    bcastMsg = temp;
                                    
                                    strcpy(temp2,"BROADCAST-");
                                    strcat(temp2,bcastMsg);
                                    send(sockfd, temp2, sizeof(temp2),0);
                                    //cse4589_print_and_log("[%s:SUCCESS]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    
                                }
                                else
                                {
                                    printf("[%s:ERROR]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                    //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                }
                                
                                //printf("sent to server\n");
                            }
                            else if (command.compare(compareUserInput8)==0)                 //if command is BLOCK
                            {
                                if(privilegeFlag==1)
                                {
                                
                                
                                char *IP = tokenContainer[1];
                                char temp[100];
                                strcpy(temp,"BLOCK");
                                strcat(temp, "-");
                                strcat(temp, IP);
                                    clientLocal *obj1;
                                    obj1=clientLocalSearchByIP(IP);
                                    
                                    if(obj1==NULL)
                                    {
                                        printf("Valid but Inconsistent IP. Try REFRESHing your list\n");
                                    }
                                    else
                                    {
                                        send(sockfd, temp, sizeof(temp), 0);
                                    }
                                }
                                else
                                {
                                   printf("[%s:ERROR]\n", command.c_str());
                                }
                            }
                            else if(command.compare(compareUserInput9)==0)                  //if command is UNBLOCK
                            {
                                printf("token compared and %d\n",privilegeFlag);
                                if(privilegeFlag==1)
                                {
                                    printf("inside block tokenization\n");
                                    char *IP = tokenContainer[1];
                                    char temp[100];
                                    strcpy(temp,"UNBLOCK");
                                    strcat(temp, "-");
                                    strcat(temp, IP);
                                    printf("sending %s to server\n",temp);
                                    send(sockfd, temp, sizeof(temp), 0);
                                    
                                }
                                else
                                {
                                    printf("[%s:ERROR]\n", command.c_str());
                                }
                            }
                            else if(command.compare(compareUserInput10)==0)                 //if command is LOGOUT
                            {
                                
                                if(privilegeFlag==1)
                                {
                                    
                                    int b = send(sockfd, "LOGOUT", strlen("LOGOUT") , 0);
                                    if(b<0)
                                    {
                                        //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                        //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    }
                                    else
                                    {
                                        printf("You have chosen to logout\n");
                                        privilegeFlag=0;
                                        logoutFlag = 1;
                                    }
                                }
                                else
                                {
                                    printf("[%s:ERROR]\n", command.c_str());
                                }

                                
                            }
                            else if(command.compare(compareUserInput11)==0)                 //if command is EXIT
                            {
                                
                                if((logoutFlag==1 && privilegeFlag==0) || privilegeFlag==1)
                                {
                                    clientLength = sizeof(clientAddress);
                                    getsockname(sockfd, (struct sockaddr *) &clientAddress, &clientLength);
                                    
                                    struct sockaddr_in *peername = (struct sockaddr_in *) &clientAddress;
                                    clientPort = ntohs(peername->sin_port);
                                    inet_ntop(AF_INET, &peername->sin_addr, storeAddress, sizeof (storeAddress));
                                
                                    char temp[100];
                                    strcpy(temp, "EXIT-");
                                    strcat(temp,storeAddress);
                                    int b = send(sockfd, temp, strlen(temp) , 0);
                                    if(b<0)
                                    {
                                        //cse4589_print_and_log("[%s:ERROR]\n", command.c_str());
                                        //cse4589_print_and_log("[%s:END]\n", command.c_str());
                                    }
                                        printf("You have chosen to EXIT. Thank you for using the application\n");
                                }
                                    exit(0);
                                
                            }
                            else if (command.compare(compareUserInput12)==0)                //if command is LIST
                            {
                                displayLocal();
                            }
                            else
                            {
                                printf("[%s:ERROR]\n", command.c_str());
                                
                            }
                        }
                        else
                        {
                                printf("[%s:ERROR]\n", command.c_str());
                        }
                    }
                    
                    else if(FD_ISSET(sockfd, &tempRead_fdsC))
                    {
                        
                        bzero(msgBuf, 1024);
                        int h=recv(sockfd, msgBuf, sizeof(msgBuf), 0);
                        if(h<0)
                        {
                            perror("receive error");
                        }
                        else if(h==0)
                        {
                            printf("server hung up\n");
                            close(sockfd);
                            FD_CLR(sockfd, &masterDescriptor);
                        }
                        else
                        {
                            char *token;
                            int argCounter=0;
                            char **tokenContainer = (char**)malloc( 25 * sizeof(char*));
                            
                            for (i = 0 ; i< 25 ; i++)
                            {
                                tokenContainer[i] = (char*)malloc( 1024 * sizeof(char));
                            }
                            
                            //ptr= strdup(str1.c_str());
                            
                            token = strtok(msgBuf, "-");
                            
                            while(token!=NULL)
                            {
                                strcpy(tokenContainer[argCounter], token);
                                argCounter++;
                                token=strtok(NULL, "-");
                            }
                                                                                                            //recv event management starts here
                            if(strcmp(tokenContainer[0], "SEND")==0)
                            {
                            printf("msg from:%s\n[msg]:%s\n", tokenContainer[1], tokenContainer[2]);
                            //cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
                            //cse4589_print_and_log("msg from:%s\n[msg]:%s\n", tokenContainer[1], tokenContainer[2]);
                            //cse4589_print_and_log("[RECEIVED:END]\n");
                            }
                            else if(strcmp(tokenContainer[0], "BROADCAST")==0)
                            {
                            printf("msg :%s\ns", tokenContainer[1]);
                            //cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
                            //cse4589_print_and_log("msg from:%s\n[msg]:%s\n", tokenContainer[1], tokenContainer[2]);
                            //cse4589_print_and_log("[RECEIVED:END]\n");
                            }
                            else if(strcmp(tokenContainer[0], "PACK")==0)
                            {
                                    argCounter=0;
                                    char **serialDataContainer = (char**)malloc( 100 * sizeof(char*));
                                    
                                    for (i = 0 ; i< 100 ; i++)
                                    {
                                        serialDataContainer[i] = (char*)malloc( 1024 * sizeof(char));
                                    }
                               
                                token = strtok(tokenContainer[1], "@");
                               
                                while(token!=NULL)
                                {
                                    strcpy(serialDataContainer[argCounter], token);
                                    argCounter++;
                                    token=strtok(NULL, "@");
                                 }
                                int k=0;
                                char storeIP[30], storeHostname[50];
                                setHeadNull();
                                while(strcmp(serialDataContainer[k], "\0")!=0)
                                      {
                                          strcpy(storeIP,serialDataContainer[k]);
                                          k++;
                                          int storePort=atoi(serialDataContainer[k]);
                                          k++;
                                          int storeSocket=atoi(serialDataContainer[k]);
                                          k++;
                                          strcpy(storeHostname ,serialDataContainer[k]);
                                          insertClientLocal(storeIP, storePort, storeSocket, storeHostname);
                                          k++;
                                      }
                            }
                        }
                    }
                    
                }//while(1) ends
            
             return 0;
                }// tcp client ends

};


int main(int argc, char *argv[])
{
    /*Init. Logger*/
    	//cse4589_init_log(argv[2]);
    
    /* Clear LOGFILE*/
   // fclose(fopen(LOGFILE, "w"));
    
    /*Start Here*/
    
    if (argc < 3)
    {
        fprintf(stderr,"USAGE: ./main <S for Server | C for Client> <PORT NO> \n");
        exit(1);
    }
    
    char portNumber[6];
    char operationChoice[2];
    strcpy(operationChoice, argv[1]);
    strcpy(portNumber, argv[2]);
    printf("\n YOU CHOOSE TO RUN THE APPLICATION IN (%s) MODE \n\n", operationChoice);
    ServerClientChat obj = *new ServerClientChat(operationChoice);
    
    if(operationChoice[0]=='s')
    {
        printf(" Commands are accepted only in UPPERCASE :\n");
        printf("********************************************************************\n\n");
        printf("AUTHOR\n");     //@
        printf("IP - Who am I?\n");     //@
        printf("PORT - Where am I listening?\n");       //@
        printf("LIST - To see the list of currently logged in clients:\n");     //@
        printf("STATISTICS - To see client stats\n");
        printf("BLOCKED - To see a list of blocked clients\n");
        obj.establishServer(portNumber);
    }
    else if(operationChoice[0]=='c')
    {
        printf(" Commands are accepted only in UPPERCASE :\n");
        printf("*********************************************************************\n\n");
        printf("AUTHOR\n");     //@
        printf("IP - Who am I?\n");     //@
        printf("PORT - Where am I listening?\n");       //@
        printf("LOGIN - Login to the system\n");
        printf("LIST - See who's at home?\n");      //@
        printf("REFRESH - Get an updated list of logged-in clients\n");     //@
        printf("SEND <client-ip> <msg> - One to one message\n");
        printf("BROADCAST - One to all message\n");
        printf("BLOCK <client-ip> - Block all incoming connections from this IP\n");
        printf("UNBLOCK <client-ip> - Unblock a previosly blocked client\n");
        printf("LOGOUT - Logout from the server\n");
        printf("EXIT - Logout from the server and exit the application\n");
        obj.establishClient(portNumber);
        
    }
}




