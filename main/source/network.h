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

#define GHOST_ACTION_NONE 0
#define GHOST_ACTION_CREAK_DOOR 11
#define GHOST_ACTION_FLICKER_LAMP 12
#define GHOST_ACTION_POSSESS_CLOCK 13
#define GHOST_ACTION_TV_STATIC 14
#define GHOST_ACTION_BOO 15
#define GHOST_ACTION_EXPLODE_LAMP 16
#define GHOST_ACTION_SLAM_DOOR 17
#define GHOST_ACTION_SPIN_FAN 18
#define GHOST_ACTION_GLOW_DOLL 19
#define GHOST_ACTION_MOVE_DOLL 20

#define GHOST_ACTION_LOST_HORRIBLY 69

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
void sendPlayerLook(float pitch, float yaw, float FOV, float health);
Position getGhostPosition();
Position getPlayerLook();
float getPlayerHealth();

void sendGhostAction(int action);
int actionReady();


#endif /* defined(__slumber__network__) */
