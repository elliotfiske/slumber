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

#define BUFF_SIZE 56

// Ghost->client packets
#define GHOST_POSITION_UPDATE_FLAG 1
#define GHOST_APPEARS_FLAG 2
#define GHOST_DISAPPEARS_FLAG 3

// Client->ghost packets
#define USER_LOOK_UPDATE_FLAG 4


typedef struct InfoHeader {
    uint8_t  flag;
    int16_t  value1;
    int16_t  value2;
    int16_t  value3;
} __attribute__((packed)) InfoHeader;

typedef struct Position {
    float x;
    float y;
    float z;
} Position;

void doClientNetworking();
void doGhostNetworking();

int tcpSetup();
int tcpClientSetup(char *host_name, char *port);
void sendData(char *data);
void receiveData(int socket);
void processIncomingPacket(char *entirePacket, long dataLen, int clientSocket);

// Methods for external use
void sendGhostPosition(float x, float y, float z);
Position getGhostPosition();
Position getPlayerLook();


#endif /* defined(__slumber__network__) */
