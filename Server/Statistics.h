//
//  Statistics.hpp
//  Server
//
//  Created by Gautam on 2/21/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#ifndef Statistics_h
#define Statistics_h

#include <stdio.h>

void displayStats();;
void insertClientStats(int sentMSG, int recvMSG, char *hName, char *status, int lport, char *IP, int sock);
struct clientStats * create_node(int sentMSG, int recvMSG, char *hName, char *status, int port, char *IP, int sock);
struct clientStats * clientStatsSearchByIP(char *IP);
void sort();
void deleteStatsClient(char *IP1);
void updateMessagesSentCounter(char *IP);
void updateClientStatus(char *IP);
void updateMessagesRecvCounter(char *IP);
struct clientStats * clientStatsSearchBySocket(int socket);


#endif /* Statistics_hpp */
