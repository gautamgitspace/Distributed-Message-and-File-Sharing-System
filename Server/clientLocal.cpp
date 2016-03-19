//
//  clientLocal.cpp
//  Server
//
//  Created by Gautam on 2/25/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#include "clientLocal.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <inttypes.h>
#include "logger.h"
#include "global.h"



#define MAX 256
using namespace std;

struct clientLocal
{
    char *IPAddress;
    int sourcePort;
    int acceptSocket;
    char *hostname;
    struct clientLocal *next;
}*clientLocalStart;

struct clientLocal * create_node_Local(char *IP, int port, int socket, char *hostname)
{
    struct clientLocal *temp =  (struct clientLocal *) malloc(sizeof(struct clientLocal));
    if (temp == NULL)
    {
        cout<<"Memory not allocated "<<endl;
        return 0;
    }
    else
    {
        temp->IPAddress = static_cast<char *> (malloc (MAX));
        temp->hostname = static_cast<char *>(malloc(MAX));
        strcpy(temp->IPAddress, IP);
        strcpy(temp->hostname, hostname);
        temp->sourcePort=port;
        temp->acceptSocket=socket;
        temp->next = NULL;
        return temp;
    }
}

struct clientLocal* insertClientLocal(char *IP, int port, int socket, char *hostname)       //will be fired on login
{
    struct clientLocal *temp, *p;
    temp = create_node_Local(IP, port, socket, hostname);
    if (clientLocalStart == NULL)
    {
        //printf("TEST 1.....\n");
        clientLocalStart = temp;
        clientLocalStart->next = NULL;
    }
    else
    {
        p = clientLocalStart;
        clientLocalStart = temp;
        clientLocalStart->next = p;
    }
    //cout<<"Client Inserted in the stats list"<<endl;
    return clientLocalStart;
}


void sortLocalList()         //sorts on the base of PORT NUMBERS
{
    printf("inside sort in Statistics.cpp\n");
    clientLocal *ptr, *ss;
    int p, s;
    char ipa[30], hn[30];
    
    if (clientLocalStart == NULL)
    {
        cout<<"Enteries: None"<<endl;
        return;
    }
    ptr = clientLocalStart;
    while (ptr != NULL)
    {
        
        for (ss = ptr->next;ss !=NULL;ss = ss->next)
        {
            if (ptr->sourcePort > ss->sourcePort)
            {
                
                p = ptr->sourcePort;                     //swap PORTS
                ptr->sourcePort = ss->sourcePort;
                ss->sourcePort = p;
                
                strcpy(ipa, ptr->IPAddress);                //swap IPAdress
                strcpy(ptr->IPAddress, ss->IPAddress);
                strcpy(ss->IPAddress, ipa);
                
                strcpy(hn, ptr->hostname);                  //swap hostname
                strcpy(ptr->hostname, ss->hostname);
                strcpy(ss->hostname, hn);
                
                s= ptr->acceptSocket;                       //swap sockets
                ptr->acceptSocket=ss->acceptSocket;
                ss->acceptSocket=s;
                
            }
        }
        ptr = ptr->next;
    }
}


void displayLocal()
{
    //cse4589_print_and_log("[LIST:SUCCESS]\n");
    sortLocalList();
    struct clientLocal *temp;
    if (clientLocalStart == NULL)
    {
        cout<<""<<endl;
        return;
    }
    temp = clientLocalStart;
    int serialNumber=1;
    printf("\n");
    printf("S.NO        IP Address          Hostname                PORT\n"   );
    printf("\n");
    while (temp != NULL)
    {
        
        printf("%-5d%-35s%-20s%-8d\n",serialNumber++,temp->IPAddress,temp->hostname,temp->sourcePort);
        //cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",serialNumber++,temp->IPAddress,temp->hostname,temp->sourcePort);
        temp = temp->next;
    }
    cout<<""<<endl;
    //cse4589_print_and_log("[LIST:END]\n");
    free(temp);
}

void setHeadNull()
{
    clientLocalStart=NULL;
}

struct clientLocal * clientLocalSearchByIP(char *IP)        //to search a client
{
    
    printf("inside clientSearchByIP\n");
    char searchIP[30];
    strcpy(searchIP,IP);
    bool flag = false;
    if (clientLocalStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    struct clientLocal *s;
    s = clientLocalStart;
    while (s != NULL)
    {
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




