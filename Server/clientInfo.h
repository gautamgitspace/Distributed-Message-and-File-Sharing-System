//
//  clientInfo.h
//  Server
//
//  Created by Gautam on 2/19/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#ifndef clientInfo_h
#define clientInfo_h

#include <stdio.h>

struct clientinfo* create_node(char *IP, int port, int socket, char *hostname);
void insertClient(char *IP, int port, int socket, char *hostname);
void display();
struct clientinfo * clientSearchByIP(char *IP);
struct clientinfo * clientSearchBySocket(int socket);
void serializeList(clientinfo *item, char *buffer);
int listSize(clientinfo *item);
char * work(clientinfo *);
char * serialize();
void deleteClient(char *IP1);
void sortList();


#endif /* clientInfo_h */

