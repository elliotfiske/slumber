//
//  network_setup.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/6/15.
//
//

#include "network_setup.h"
#include <cstring>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>

/**
 * Set up for the spooky ghost, who acts as the server.
 */
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

/**
 * Set up for the person getting spooked.
 */
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

    printf("Looking for server at %s...\n", SERVER_ADDRESS);
    
    memcpy((char*)&remote.sin_addr, (char*)hp->h_addr, hp->h_length);
    
    // get the port used on the remote side and store
    remote.sin_port = htons(atoi(port));
    
    if (connect(socketNum, (struct sockaddr*)&remote, sizeof(struct sockaddr_in)) < 0) {
        perror("connect call");
        exit(-1);
    }
    
    printf("SUCCESSFULLY CONNECTED\n");
    
    return socketNum;
}
