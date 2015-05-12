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
int ghostPosX, ghostPosY, ghostPosZ;
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
    
    partnersSocket = clientSocket;
    
    while (1) {
        receiveData(clientSocket);
        printf(" from the poor guy I'm haunting!\n");
    }
}

void doClientNetworking() {
    int serverSocket = 0;
    
    char *host = (char *)"localhost";
    char *port = (char *)"4444";
    
    serverSocket = tcpClientSetup(host, port);
    
    partnersSocket = serverSocket;
    
    while (1) {
        receiveData(serverSocket);
        printf(" from A SPOOKY GHOST\n");
    }
    
    close(serverSocket);
}

void sendData(void *data) {
    send(partnersSocket, data, BUFF_SIZE, 0);
}

/**
 * Pack the ghost's position into 3 ints and send it
 *  over to the client.
 */
void sendGhostPosition(int16_t x, int16_t y, int16_t z) {
    InfoHeader posHeader = {
        GHOST_POSITION_UPDATE_FLAG,
        x,
        y,
        z
    };
    
    sendData(&posHeader);
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
        if (messageLen != BUFF_SIZE) {
            cout << "Cap'n, received packet of size " << messageLen << " instead of 56..." << endl;
        }
        processIncomingPacket(buf, messageLen, serverSocket);
    }
}

void processIncomingPacket(void *entirePacket, long dataLen, int clientSocket) {
    InfoHeader incoming;
    memcpy(&incoming, entirePacket, BUFF_SIZE);
    
    cout << "Received packet with flag " << incoming.flag << " value1 " << incoming.value1 << " value2 " << incoming.value2 << " value 3 " << incoming.value3 << endl;
}
