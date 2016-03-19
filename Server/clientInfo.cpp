//
//  clientInfo.cpp
//  Server
//
//  Created by Gautam on 2/19/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#include "clientInfo.h"
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

struct clientinfo
{
    char *IPAddress;
    int sourcePort;
    int acceptSocket;
    char *hostname;
    struct clientinfo *next;
}*clienttinfoStart;



struct clientinfo * create_node(char *IP, int port, int socket, char *hostname)
{
    struct clientinfo *temp =  (struct clientinfo *) malloc(sizeof(struct clientinfo));
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

void insertClient(char *IP, int port, int socket, char *hostname)       //will be fired on login
{
    struct clientinfo *temp, *p;
    temp = create_node(IP, port, socket, hostname);
    if (clienttinfoStart == NULL)
    {
        //printf("TEST 1.....\n");
        clienttinfoStart = temp;
        clienttinfoStart->next = NULL;
    }
    else
    {
        p = clienttinfoStart;
        clienttinfoStart = temp;
        clienttinfoStart->next = p;
    }
    cout<<"Client Inserted in the server side client list"<<endl;
    //return clienttinfoStart;
}


void sortList()         //sorts on the base of PORT NUMBERS
{
    printf("inside sort in Statistics.cpp\n");
    clientinfo *ptr, *ss;
    int p, s;
    char ipa[30], hn[30];
    
    if (clienttinfoStart == NULL)
    {
        cout<<"Enteries: None"<<endl;
        return;
    }
    ptr = clienttinfoStart;
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



void display()
{
    //cse4589_print_and_log("[LIST:SUCCESS]\n");
    sortList();
    struct clientinfo *temp;
    if (clienttinfoStart == NULL)
    {
        //cse4589_print_and_log("[LIST:SUCCESS]\n");
        cout<<"[CLIENTS LOGGED IN: NONE]"<<endl;
        
        //cse4589_print_and_log("[LIST:END]\n");
        return;
        
    }
    temp = clienttinfoStart;
    int serialNumber=1;
    //printf("\n");
    //printf("S.NO        IP Address          Hostname                PORT\n"   );
    //printf("\n");
    while (temp != NULL)
    {
        printf("%5d%35s%20s%8d\n",serialNumber++,temp->IPAddress,temp->hostname,temp->sourcePort);
        //cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",serialNumber++,temp->IPAddress,temp->hostname,temp->sourcePort);
        temp = temp->next;
    }
    cout<<""<<endl;
    free(temp);
    //cse4589_print_and_log("[LIST:END]\n");
}



struct clientinfo * clientSearchByIP(char *IP)        //to search a client
{
    
    printf("inside clientSearchByIP\n");
    char searchIP[30];
    strcpy(searchIP,IP);
    bool flag = false;
    if (clienttinfoStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    struct clientinfo *s;
    s = clienttinfoStart;
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


struct clientinfo * clientSearchBySocket(int socket)        //to search a client
{
    printf("inside clientSearchBySocket\n");
    bool flag = false;
    if (clienttinfoStart == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    struct clientinfo *s;
    s = clienttinfoStart;
    while (s != NULL)
    {
        if (s->acceptSocket==socket)
        {
            flag = true;
            cout<<"Client found"<<endl;
            return s;
            
        }
        s = s->next;
    }
    if (!flag)
    {
        cout<<"No client with socketFD"<<socket<<" not found in the list"<<endl;
        return NULL;
    }
    else
    {
        return s;
    }
}

char * serialize()
{
    char bigData[2048];
    char tempBigData[100];
    strcpy(bigData,"PACK-");
    clientinfo *temp;
    temp=clienttinfoStart;
    while(temp!=NULL)
    {
        sprintf(tempBigData, "%s@%d@%d@%s@", temp->IPAddress, temp->sourcePort, temp->acceptSocket, temp->hostname);
        strcat(bigData,tempBigData);
        temp=temp->next;
    }
    return bigData;
}

void deleteClient(char *IP1)     //will be fired on logout
{
    int counter = 0;
    char IPLoggedOut[30];
    strcpy(IPLoggedOut,IP1);
    struct clientinfo *s, *prev, *ptr;
    s = clienttinfoStart;
    
    while (s != NULL)
    {
        if(strcmp(s->IPAddress, IPLoggedOut)==0)
        {
            ptr=s;
            break;
        }
        s = s->next;
        counter++;
    }
    
    
    if(ptr==clienttinfoStart)
    {
        if(clienttinfoStart->next==NULL)
        {
            clienttinfoStart=NULL;
            return;
        }
        else
            clienttinfoStart=ptr->next;
    }
    else if(counter>0)
    {
        s=clienttinfoStart;
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



