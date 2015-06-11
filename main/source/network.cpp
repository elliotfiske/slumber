//
//  network.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/6/15.
//
//

#include "network.h"
#include "network_setup.h"

#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

int partnersSocket = 0;

// These correspond to properties in GameState
float ghostPosX, ghostPosY, ghostPosZ;

// These correspond to properties in GhostState
float playerLookYaw, playerLookPitch, playerLookFOV;
float recvPlayerHealth;

bool shouldShowGhost;

void doGhostNetworking() {
    int serverSocket = 0;
    
    // create the server socket
    serverSocket = tcpSetup();
    
    // Start listenin' for the player
    if (listen(serverSocket, 1) < 0) {
        perror("listen call");
        exit(-1);
    }
    
    // Wait for client to connect
    int clientSocket = accept(serverSocket, (struct sockaddr*)0,  (socklen_t *)0);
    if (clientSocket < 0) {
        perror("accept call FAILED.");
        exit(-1);
    }
    
    printf("CLIENT HAS CONNECTED!\n");
    
    partnersSocket = clientSocket;
    
    while (1) {
        receiveData(clientSocket);
    }
}

void doClientNetworking() {
    int serverSocket = 0;
    
    char *host = (char *) SERVER_ADDRESS;
    char *port = (char *) SERVER_PORT;
    
    serverSocket = tcpClientSetup(host, port);
    
    partnersSocket = serverSocket;
    
    while (1) {
        receiveData(serverSocket);
    }
    
    close(serverSocket);
}

void sendData(char *data) {
    send(partnersSocket, data, strlen(data) + 1, 0);
}

/**
 * Pack the ghost's position into 3 ints and send it
 *  over to the client.
 */
void sendGhostPosition(float x, float y, float z) {
    char dataString[256];
    sprintf(dataString, "%d %f %f %f %f", GHOST_POSITION_UPDATE_FLAG, x, y, z, 0.0);
    
    sendData(dataString);
}

/**
 * Pack the client's camera angle into a bootiful string and
 *  zap it over to the ghost
 */
void sendPlayerLook(float pitch, float yaw, float FOV, float health) {
    char dataString[256];
    sprintf(dataString, "%d %f %f %f %f", USER_LOOK_UPDATE_FLAG, pitch, yaw, FOV, health);

    sendData(dataString);
}

/**
 * Send one of the predefined constants over to the player.
 *  That'll spook em!
 */
void sendGhostAction(int action) {
    char dataString[256];
    sprintf(dataString, "%d %f %f %f %f", action, 0.0, 0.0, 0.0, 0.0);
    
    sendData(dataString);
}

void receiveData(int serverSocket) {
    char buf[BUFF_SIZE]; // Buffer for receiving data from the other guy
    
    long messageLen = recv(serverSocket, buf, BUFF_SIZE, 0);
    if (messageLen < 0) {
        perror("recv call FAILED.  MISERABLY. ");
    }
    else if (messageLen == 0) {
        printf("Partner disconnected!!\n");
        exit(0);
    }
    else {
        processIncomingPacket(buf, messageLen, serverSocket);
    }
}

int currAction = 0;

void processIncomingPacket(char *entirePacket, long dataLen, int clientSocket) {
//    cout << "Received packet: " << entirePacket << endl;
    int flag;
    float x, y, z, w;
    sscanf(entirePacket, "%d %f %f %f %f", &flag, &x, &y, &z, &w);
    
    if (flag == GHOST_ACTION_FLICKER_LAMP) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_CREAK_DOOR) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_SLAM_DOOR) {
        currAction = flag;
    }

    if (flag == GHOST_ACTION_POSSESS_CLOCK) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_TV_STATIC) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_BOO) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_EXPLODE_LAMP) {
        currAction = flag;    }
    
    if (flag == GHOST_ACTION_LOST_HORRIBLY) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_GLOW_DOLL) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_MOVE_DOLL) {
        currAction = flag;
    }
    
    if (flag == GHOST_ACTION_SPIN_FAN) {
        currAction = flag;
    }
    
    if (flag == GHOST_POSITION_UPDATE_FLAG) {
        ghostPosX = x;
        ghostPosY = y;
        ghostPosZ = z;
    }
    
    if (flag == USER_LOOK_UPDATE_FLAG) {
        playerLookPitch = x;
        playerLookYaw = y;
		playerLookFOV = z;
        recvPlayerHealth = w;
    }
}

int actionReady() {
    if (currAction) {
        int result = currAction;
        currAction = 0;
        return result;
    }
    return currAction;
}


Position getGhostPosition() {
    Position result = {ghostPosX, ghostPosY, ghostPosZ};
    return result;
}

Position getPlayerLook() {
	Position result = {playerLookPitch, playerLookYaw, playerLookFOV};
	return result;
}

float getPlayerHealth() {
    return recvPlayerHealth;
}

