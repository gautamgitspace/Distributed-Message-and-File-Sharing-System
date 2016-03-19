//
//  clientOperations.cpp
//  Server
//
//  Created by Gautam on 2/14/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#include "clientOperations.h"
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
#include "logger.h"
#include "global.h"

using namespace std;

#define MAXMESSAGESIZE 256


void author()
{
    char UbitName[30];
    strcpy(UbitName, "agautam2");
    printf("I, %s, have read and understood the course academic integrity policy.\n",UbitName);
    //cse4589_print_and_log("[AUTHOR:SUCCESS]\n");
    //cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n",UbitName);
    //cse4589_print_and_log("[AUTHOR:END]\n");
    
}

void whoAmI(char *a)
{
    char thisIsMe[30];
    strcpy(thisIsMe, a);
    printf("IP: %s\n",thisIsMe);
    //cse4589_print_and_log("[IP:SUCCESS]\n");
    //cse4589_print_and_log("IP: %s\n",thisIsMe);
    //cse4589_print_and_log("[IP:END]\n");
    
}

void whereAmIListening(char *a)
{
    char hereIamListening[10];
    strcpy(hereIamListening, a);
    printf("PORT: %s\n",hereIamListening);
    //cse4589_print_and_log("[PORT:SUCCESS]\n");
    //cse4589_print_and_log("PORT: %s\n",hereIamListening);
    //cse4589_print_and_log("[PORT:END]\n");
    
}

void login(char *a, char *b, int c)
{
    //int sockfd=c;
    int portno;
    struct sockaddr_in serv_addr;
    struct addrinfo hints, *serverInfo, *p;
    struct hostent *server;
    portno = atoi(b);
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sockfd in client%d\n",c);
    if (c < 0)
        perror("ERROR opening socket");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    server = gethostbyname(a);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(c,p->ai_addr, p->ai_addrlen) < 0)
        {
        perror("ERROR connecting");
        }
        else
        {
        printf("Logged in");
        }
}

void sendMessage(char *a, char *b, int c)
{
    int n;
    int sockfd = c;
    char messageBuffer[MAXMESSAGESIZE];
    char ipBuffer[256];
    bzero(messageBuffer,256);
    char temp[1024];
    bzero(ipBuffer,256);
    bzero(temp,1024);
    strcpy(messageBuffer, b);
    strcpy(ipBuffer,a);
    string completeString;
    strcpy(temp, "SEND");
    strcat(temp, "-");
    strcat(temp,ipBuffer);
    strcat(temp,"-");
    strcat(temp,messageBuffer);
 
        n = send(sockfd,temp,sizeof(temp),0);
        printf("%d",n);
        printf("clientOperations sockfd: %d\n", sockfd);
        if (n < 0)
            perror("ERROR writing to socket");
        bzero(temp,1024);

}

