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

#define BUFF_SIZE 100

using namespace std;

void doClientNetworking() {
    int serverSocket = 0;
    
    char *host = (char *)"localhost";
    char *port = (char *)"4444";
    
    char *data = (char *)"client sez hi";
    
    serverSocket = tcpClientSetup(host, port);
    
    while (1) {
        clientListen(serverSocket);
        send(serverSocket, data, strlen(data) + 1, 0);
    }
    
    close(serverSocket);
}

int clientSocket = 0;

void doGhostNetworking() {
    int serverSocket = 0;
    char *data = (char *)"big ol booty";
    
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
    
    while (1) {
        send(clientSocket, data, strlen(data) + 1, 0);
        recvFromClient(clientSocket);
    }
}

void recvFromClient(int serverSocket) {
    char buf[BUFF_SIZE]; // Buffer for receiving data from the player
    
    long messageLen = recv(serverSocket, buf, BUFF_SIZE, 0);
    if (messageLen < 0) {
        perror("recv call FAILED.  MISERABLY. ");
    }
    else if (messageLen == 0) {
        printf("Client disconnected!!\n");
    }
    else {
        printf("Server got packet: ");
        processIncomingPacket(buf, messageLen, serverSocket);
    }
}

void clientListen(int socket) {
    char data[BUFF_SIZE];
    
    long messageLen = recv(socket, data, BUFF_SIZE, 0);
    if (messageLen < 0) {
        perror("recv call MESSED UP: ");
    }
    else if (messageLen == 0) {
        printf("Server terminated.\n");
        exit(0);
    }
    else {
        printf("Client got packet: ");
        processIncomingPacket(data, messageLen, 0);
    }
}

void processIncomingPacket(char entirePacket[], long dataLen, int clientSocket) {
    entirePacket[dataLen] = '\0';
    printf("Packet: %s\n", entirePacket);
}
