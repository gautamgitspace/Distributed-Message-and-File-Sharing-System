//
//  blockInfo.h
//  Server
//
//  Created by Gautam on 2/19/16.
//  Copyright © 2016 Gautam. All rights reserved.
//

#ifndef blockInfo_h
#define blockInfo_h

#include <stdio.h>

struct blockinfo * create_node(char *IPtoBlock, char *IPblockFor, int sockToBlock, int sockBlockFor, char *hname, int lport);
void insertBlockedClient(char *IPtoBlock, char *IPblockFor, int sockToBlock, int sockBlockFor, char *hname, int lport);
bool searchForAlreadyBlocked(char *IP1, char *IP2);
void displayBlockedList(char *IPBlockedFor);
void deleteBlockedClient(char *IP1, char *IP2);
//struct clientinfo * clientSearch(char *IP);

#endif /* blockInfo_h */
