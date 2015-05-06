//
//  network.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/6/15.
//
//

#include "network.h"

#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <algorithm>

#define BUFF_SIZE 100

using namespace std;

void doClientNetworking() {
    int serverSocket = 0;

    char *host = (char *)"localhost";
    char *port = (char *)"4444";
    
    char *data = (char *)"client sez hi";
    
    serverSocket = tcpClientSetup(host, port);
    
//    while (1) {
    clientListen(serverSocket);
    send(serverSocket, data, strlen(data) + 1, 0);
//    }
    
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
    
//    while (1) {
    send(clientSocket, data, strlen(data) + 1, 0);
    recvFromClient(clientSocket);
//    }
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

int tcpSetup() {
    int server_socket = 0;
    struct sockaddr_in local;      /* socket address for local side  */
    socklen_t len = sizeof(local);  /* length of local address        */
    
    /* create the socket  */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        perror("socket call");
        exit(1);
    }
    
    local.sin_family = AF_INET;         //internet family
    local.sin_addr.s_addr = INADDR_ANY; //wild card machine address
    local.sin_port = htons(4444);          //use port 4444
    
    // Uncomment to use a specific port forever
    int yeah = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yeah, sizeof(int));
    
    /* bind the name (address) to a port */
    if (bind(server_socket, (struct sockaddr *) &local, sizeof(local)) < 0) {
        perror("bind call");
        exit(-1);
    }
    
    //get the port name and print it out
    if (getsockname(server_socket, (struct sockaddr*) &local, &len) < 0) {
        perror("getsockname call");
        exit(-1);
    }
    
    printf("Server is using port %d\n", ntohs(local.sin_port));
    
    return server_socket;
}

int tcpClientSetup(char *hostName, char *port) {
    int socketNum;
    struct sockaddr_in remote;       // socket address for remote side
    struct hostent *hp;              // address of remote host
    
    // create the socket
    if ((socketNum = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call");
        exit(-1);
    }
    
    // designate the addressing family
    remote.sin_family= AF_INET;
    
    // get the address of the remote host and store
    if ((hp = gethostbyname(hostName)) == NULL) {
        printf("Error getting hostname: %s\n", hostName);
        exit(-1);
    }
    
    memcpy((char*)&remote.sin_addr, (char*)hp->h_addr, hp->h_length);
    
    // get the port used on the remote side and store
    remote.sin_port= htons(atoi(port));
    
    if (connect(socketNum, (struct sockaddr*)&remote, sizeof(struct sockaddr_in)) < 0) {
        perror("connect call");
        exit(-1);
    }
    
    return socketNum;
}