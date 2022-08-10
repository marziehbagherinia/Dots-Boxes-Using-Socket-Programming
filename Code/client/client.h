#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <time.h>

#include "networks.h"

void game_dots(char you, int sockfd);

#endif
