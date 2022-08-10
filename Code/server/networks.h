#ifndef NETWORKS_H
#define NETWORKS_H

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

#define TRUE 1
#define FALSE 0
#define MAX_PLAYER 2
#define THREE_MAX_PLAYER 3
#define FOUR_MAX_PLAYER 4
#define BOARD_SIZE 9
#define MAX_TRIES 200
#define BUFFER_SIZE ((BOARD_SIZE)*(BOARD_SIZE)*2)

typedef char bool;

char board[BOARD_SIZE * BOARD_SIZE];
char* invalid_input;

int points_A;
int points_B;

int ports;


struct client
{
    int sockfd;
    bool is_connected;
    char id;
    double time;
};

struct client_set
{
    struct client* client;
    unsigned nb;
};

void error(char* msg);
int init_server(const char* portno);
int send_verif(int sockfd, char * msg);
int recv_verif(int sockfd, char * buffer);
int socket_ready(int sockfd, unsigned timeout_ms);
void send_char(int sockfd, char msg);
int wait_client(int sockfd);
struct client_set client_set_init();
void client_set_add(struct client_set* set, int sockfd, char id);
void send_complete(struct client_set* set);
void client_set_send(struct client_set * set, char* msg);
void client_set_close(struct client_set set);
char* convert(int input);

#endif