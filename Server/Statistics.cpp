//
//  Statistics.cpp
//  Server
//
//  Created by Gautam on 2/21/16.
//  Copyright © 2016 Gautam. All rights reserved.
//  MBP111.0138.B16
//  System Serial: C02P4SP9G3QH
//  main.cpp
//
//  Created by Abhishek Gautam on 2/7/16.
//  agautam2@buffalo.edu
//  University at Buffalo, The State University of New York.
//  should be fired on - LOGIN, LOGOUT, Message sent by client, Message received by client

#include "Statistics.h"
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
#include "logger.h"
#define MAX 256

using namespace std;

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
}*clientStatsStart;

struct clientStats * create_node(int sentMSG, int recvMSG, char *hName, char *status, int port, char *IP, int sock)
{

    struct clientStats *temp =  (struct clientStats *) malloc(sizeof(struct clientStats));
    if (temp == NULL)
    {
        cout<<"Memory not allocated "<<endl;
        return 0;
    }
    else
    {
        temp->hostname = static_cast<char *> (malloc (MAX));
        temp->clientStatus = static_cast<char *>(malloc(MAX));
        temp->IPAddress = static_cast<char *>(malloc(MAX));
        strcpy(temp->hostname, hName);
        strcpy(temp->clientStatus,status);
        strcpy(temp->IPAddress,IP);
        temp->sentMessages=sentMSG;
        temp->recvMessages=recvMSG;
        temp->next = NULL;
        //temp->sno=1;
        temp->listeningPort=port;
        temp->socket=sock;
        return temp;
    }
}

void insertClientStats(int sentMSG, int recvMSG, char *hName, char *status, int lport, char *IP, int sock)       //will be fired on login
{
    struct clientStats *temp, *p;
    temp = create_node(sentMSG, recvMSG, hName, status, lport, IP, sock);
    if (clientStatsStart == NULL)
    {
        //printf("TEST 1.....\n");
        clientStatsStart = temp;
        clientStatsStart->next = NULL;
    }
    else
    {
        p = clientStatsStart;
        clientStatsStart = temp;
        clientStatsStart->next = p;
    }
    cout<<"Client Inserted in the stats list"<<endl;
    //free(temp);
    //free(p);
}
struct clientStats * clientStatsSearchByIP(char *IP)        //to search a client
{
    
    printf("inside clientStatsSearchByIP\n");
    char searchIP[30];
    strcpy(searchIP,IP);
    bool flag = false;
    if (clientStatsStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    struct clientStats *s;
    s = clientStatsStart;
    while (s != NULL)
    {
        
        //printf("%s   %s\n",s->IPAddress,searchIP);
        if (strcmp(s->IPAddress,searchIP)==0)
        {
            flag = true;
            cout<<"Client found"<<endl;
            return s;
            
        }
        s = s->next;
    }
    if (!flag)
    {
        cout<<"No client with IP"<<IP<<" found in the list"<<endl;
        return NULL;
    }
    else
    {
        return s;
    }
}

//@@@@@
void sort()         //sorts on the base of PORT NUMBERS taken from the STATS LIST
{
    printf("inside sort in Statistics.cpp\n");
    clientStats *ptr, *ss;
    int p,rm,sm;
    char ipa[30], hn[30], cs[10];
    
    if (clientStatsStart == NULL)
    {
        cout<<"Enteries: None"<<endl;
        return;
    }
    ptr = clientStatsStart;
    while (ptr != NULL)
    {
        
        for (ss = ptr->next;ss !=NULL;ss = ss->next)
        {
            if (ptr->listeningPort > ss->listeningPort)
            {
                
                p = ptr->listeningPort;                     //swap PORTS
                ptr->listeningPort = ss->listeningPort;
                ss->listeningPort = p;
                
                rm=ptr->recvMessages;
                ptr->recvMessages = ss->recvMessages;        //swap recvMessages
                ss->recvMessages = rm;
               
                sm = ptr->sentMessages;
                ptr->sentMessages = ss->sentMessages;        //swap sentMessages
                ss->sentMessages=sm;
                
                strcpy(ipa, ptr->IPAddress);                //swap IPAdress
                strcpy(ptr->IPAddress, ss->IPAddress);
                strcpy(ss->IPAddress, ipa);
                
                strcpy(hn, ptr->hostname);                  //swap hostname
                strcpy(ptr->hostname, ss->hostname);
                strcpy(ss->hostname, hn);
                
                strcpy(cs, ptr->clientStatus);              //swap clientStatus
                strcpy(ptr->clientStatus, ss->clientStatus);
                strcpy(ss->clientStatus, cs);
                
            }
        }
        ptr = ptr->next;
    }
    //free(ptr);
    //free(ss);
}


void displayStats()
{
    //cse4589_print_and_log("[STATISTICS:SUCCESS]\n");
    sort();
    int sno=1;
    struct clientStats *temp;
    if (clientStatsStart == NULL)
    {
        //cse4589_print_and_log("[STATISTICS:SUCCESS]\n");
        cout<<"CLIENTS LOGGED IN: NONE"<<endl;
        //cse4589_print_and_log("[STATISTICS:END]\n");
        return;
        
    }
    temp = clientStatsStart;

    while (temp != NULL)
    {
        printf("%-5d%-35s%-8d%-8d%-8s\n",sno++,temp->hostname,temp->sentMessages,temp->recvMessages,temp->clientStatus);
        //cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", sno++,temp->hostname,temp->sentMessages,temp->recvMessages,temp->clientStatus);
        temp = temp->next;
    }
    cout<<" "<<endl;
    //cse4589_print_and_log("[STATISTICS:END]\n");
}


void updateMessagesSentCounter(char *IP)
{
        printf("inside updateMessagesSentCounter \n");
        char searchIP[30];
        strcpy(searchIP,IP);
        if (clientStatsStart == NULL)
        {
            cout<<"List is empty"<<endl;
            return;
        }
        
        struct clientStats *s;
        s = clientStatsStart;
        while (s != NULL)
        {
            if (strcmp(s->IPAddress,searchIP)==0)
            {
                s->sentMessages++;
                
            }
            s = s->next;
        }
}


void updateClientStatus(char *IP)
{
    printf("inside  updateClientStatus\n");
    char searchIP[30];
    strcpy(searchIP,IP);
    if (clientStatsStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return;
    }
    
    struct clientStats *s;
    s = clientStatsStart;
    while (s != NULL)
    {
        if (strcmp(s->IPAddress,searchIP)==0)
        {
            if(strcmp(s->clientStatus, "online")==0)
            {
                strcpy(s->clientStatus, "offline");
            }
            else if(strcmp(s->clientStatus, "offline")==0)
            {
                strcpy(s->clientStatus, "online");
            }
            
        }
        s = s->next;
    }
}

void updateMessagesRecvCounter(char *IP)
{
    printf("inside updateMessagesRecvCounter \n");
    char searchIP[30];
    strcpy(searchIP,IP);
    if (clientStatsStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return;
    }
    
    struct clientStats *s;
    s = clientStatsStart;
    while (s != NULL)
    {
        if (strcmp(s->IPAddress,searchIP)==0)
        {
            s->recvMessages++;
            
        }
        s = s->next;
    }
}

struct clientStats * clientStatsSearchBySocket(int sock)        //to search a client
{
    printf("inside clientSearchBySocket\n");
    bool flag = false;
    if (clientStatsStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    clientStats *stp;
    stp = clientStatsStart;
    while (stp != NULL)
    {
        if (stp->socket==sock)
        {
            flag = true;
            cout<<"Client found"<<endl;
            return stp;
            
        }
        stp = stp->next;
    }
        cout<<"No client with socketFD"<<sock<<" not found in the list"<<endl;
        return NULL;
}

void deleteStatsClient(char *IP1)     //will be fired on EXIT
{
    int counter = 0;
    char IPExited[30];
    strcpy(IPExited, IP1);
    struct clientStats *s, *prev, *ptr;
    s = clientStatsStart;
    
    while (s != NULL)
    {
        if(strcmp(s->IPAddress, IPExited)==0)
        {
            ptr=s;
            break;
        }
        s = s->next;
        counter++;
    }
    
    
    if(ptr==clientStatsStart)
    {
        if(clientStatsStart->next==NULL)
        {
            clientStatsStart=NULL;
            return;
        }
        else
            clientStatsStart=ptr->next;
    }
    else if(counter>0)
    {
        s=clientStatsStart;
        for(int i=0;i<counter;i++)
        {
            prev=s;
            s=s->next;
        }
        prev->next=s->next;
    }
    //free(s);
    //free(prev);
    //free(ptr);
}