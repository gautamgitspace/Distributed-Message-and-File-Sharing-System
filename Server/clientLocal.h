//
//  clientLocal.hpp
//  Server
//
//  Created by Gautam on 2/25/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#ifndef clientLocal_h
#define clientLocal_h

#include <stdio.h>
struct clientLocal * create_node_Local(char *IP, int port, int socket, char *hostname);
struct clientLocal* insertClientLocal(char *IP, int port, int socket, char *hostname);
struct clientLocal * clientLocalSearchByIP(char *IP);
void displayLocal();
void setHeadNull();
void sortLocalList();

#endif /* clientLocal_h */
