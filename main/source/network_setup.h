//
//  network_setup.h
//  slumber
//
//  Created by Elliot Fiske on 5/6/15.
//
//

#ifndef __slumber__network_setup__
#define __slumber__network_setup__

#include <stdio.h>

#define SERVER_ADDRESS "192.168.0.2"
#define SERVER_PORT "4444"
#define SERVER_PORT_NUM 4444

int tcpSetup();
int tcpClientSetup(char *host_name, char *port);

#endif /* defined(__slumber__netwt puork_setup__) */
