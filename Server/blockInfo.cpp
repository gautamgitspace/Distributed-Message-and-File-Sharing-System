//
//  blockInfo.cpp
//  Server
//
//  Created by Gautam on 2/19/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#include "blockInfo.h"
#include "clientInfo.h"
#include "Statistics.h"
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
#include "logger.h"

#define MAX 256

using namespace std;


struct blockinfo
{
    char *IPAddressToBlock;
    char *IPAddressBlockedFor;
    int socketToBlock;
    int socketBlockedFor;
    struct blockinfo *next;
    int listeningPort;
    char *hostname;
    
}*blockinfoStart;

struct blockinfo * create_node(char *IPtoBlock, char *IPblockFor, int sockToBlock, int sockBlockFor, char *hname, int lport)
{
    //blockinfoStart = NULL;
    printf("received data is %s %s \n",IPtoBlock, IPblockFor);
    //struct blockinfo *s;
    struct blockinfo *temp =  (struct blockinfo *) malloc(sizeof(struct blockinfo));
    if (temp == NULL)
    {
        cout<<"Memory not allocated "<<endl;
        return 0;
    }
    else
    {
        temp->IPAddressToBlock = static_cast<char *> (malloc (MAX));
        temp->IPAddressBlockedFor = static_cast<char *>(malloc(MAX));
        temp->hostname = static_cast<char *>(malloc(MAX));
        strcpy(temp->IPAddressToBlock, IPtoBlock);
        strcpy(temp->IPAddressBlockedFor, IPblockFor);
        strcpy(temp->hostname,hname);
        temp->socketToBlock=sockToBlock;
        temp->socketBlockedFor=sockBlockFor;
        temp->listeningPort=lport;
        temp->next = NULL;
        return temp;
    }
}

/*gets populated when client blocks someone, takes hostname and listening port from the clientStats struct*/
void insertBlockedClient(char *IPtoBlock, char *IPblockFor, int sockToBlock, int sockBlockFor, char *hname, int lport)      
{
    printf("inside insertBlockedClient\n");
    
    printf("received data is %s | %s \n",IPtoBlock, IPblockFor);
    
    struct blockinfo *temp, *p;
    temp = create_node(IPtoBlock, IPblockFor, sockToBlock, sockBlockFor, hname, lport);
    if (blockinfoStart == NULL)
    {
        printf("Inside blockinfostart null");
        blockinfoStart = temp;
        blockinfoStart->next = NULL;
    }
    else
    {
        p = blockinfoStart;
        blockinfoStart = temp;
        blockinfoStart->next = p;
    }
    cout<<"Element added to BLOCK LIST"<<endl;
    //free(temp);
    //free(p);
}


void deleteBlockedClient(char *IP1, char *IP2)     //will be fired on logout
{
    int counter = 0;
    char IPblockFor[30];
    char IPtoBlock[30];
    strcpy(IPblockFor, IP1);
    strcpy(IPtoBlock, IP2);
    struct blockinfo *s, *prev, *ptr;
    s = blockinfoStart;
    
        while (s != NULL)
        {
            if(strcmp(s->IPAddressToBlock,IPtoBlock)==0 && strcmp(s->IPAddressBlockedFor, IPblockFor)==0)
            {
                ptr=s;
                break;
            }
            s = s->next;
            counter++;
        }
    
    
        if(ptr==blockinfoStart)
        {
            if(blockinfoStart->next==NULL)
            {
                blockinfoStart=NULL;
                return;
            }
            else
            blockinfoStart=ptr->next;
        }
        else if(counter>0)
        {
            s=blockinfoStart;
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

/*  will call sort (in Statistics.cpp) first to sort the list and then display it at the SERVER END when BLOCKED is triggered
    takes Blocker IP as an argument and displays IPs blocked by the Blocker
    iterates through the linkedlist of blockinfo struct*/
void displayBlockedList(char *IPBlockedFor) //
{
    //cse4589_print_and_log("[BLOCKED:SUCCESS]\n");
    char  blocker[256];
    strcpy(blocker,IPBlockedFor);
    sort();
    struct blockinfo *temp1;
    if (blockinfoStart == NULL)
    {
        //printf("inside null check block\n");
        cout<<"CLIENTS BLOCKED: NONE"<<endl;
        //cse4589_print_and_log("[LIST:SUCCESS]\n");
        //cse4589_print_and_log("[LIST:END]\n");
        return;
    }
    
    temp1 = blockinfoStart;
    int serialNumber=1;
    printf("S.NO         IP Address Blocked           hostname         PORT\n");
    while (temp1 != NULL)
    {
        if(strcmp(blocker,temp1->IPAddressBlockedFor)==0)
        {
            //printf("inside comparison block\n");
            printf("%-5d%-35s%-20s%-8d\n",serialNumber++,temp1->IPAddressToBlock,temp1->hostname,  temp1->listeningPort );
            cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",serialNumber++,temp1->IPAddressToBlock,temp1->hostname,  temp1->listeningPort );
        }
        temp1 = temp1->next;
    }
    cout<<" "<<endl;
    //cse4589_print_and_log("[BLOCKED:END]\n");
    //free(temp1);
}


bool searchForAlreadyBlocked(char *IP1, char *IP2)
{
    printf("inside searchForAlreadyBlocked\n");
    
    //bool flag;
    char IPblockFor[30];
    char IPtoBlock[30];
    strcpy(IPblockFor, IP1);
    strcpy(IPtoBlock, IP2);
    
    if(blockinfoStart==NULL)
    {
        cout<<"List is empty"<<endl;
        return false;
    }
    struct blockinfo *s;
    s=blockinfoStart;
    while (s != NULL)
    {

    if(strcmp(s->IPAddressToBlock,IPtoBlock)==0 && strcmp(s->IPAddressBlockedFor, IPblockFor)==0)
    {
        return true;
    }
       s = s->next;
    }
    return false;
    
}


