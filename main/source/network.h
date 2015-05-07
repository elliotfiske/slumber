//
//  network.h
//  slumber
//
//  Created by Elliot Fiske on 5/6/15.
//
//

#ifndef __slumber__network__
#define __slumber__network__

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

void doClientNetworking();
void doGhostNetworking();

int tcpSetup();
int tcpClientSetup(char *host_name, char *port);
void sendData(char *data);
void receiveData(int socket);
void processIncomingPacket(char entirePacket[], long dataLen, int clientSocket);


#endif /* defined(__slumber__network__) */
