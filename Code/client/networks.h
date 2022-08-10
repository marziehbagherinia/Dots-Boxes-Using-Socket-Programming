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
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <assert.h>
#include <sys/time.h>
#include <netinet/in.h>

#define TRUE 1
#define FALSE 0

#define DEFAULT_BOARD_SIZE 9
#define MAX_TRIES 200
#define BUFFER_SIZE ((DEFAULT_BOARD_SIZE)*(DEFAULT_BOARD_SIZE)*2)
#define LOCAL_HOST_ADDR "127.0.0.1"
#define ever (;;)
#define PORT 9999

struct sockaddr_in broad_addr;
struct sockaddr_in main_addr;

char choice[BUFFER_SIZE];
char message[BUFFER_SIZE];
char board[DEFAULT_BOARD_SIZE * DEFAULT_BOARD_SIZE];
char winner;
int BOARD_SIZE;
int master_socket;
int selected;
int first_x, first_y, sec_x, sec_y;
int points_A, points_B, points_C, points_D;
int hit;
int player_number;

void error(char* msg);
int init_client(const char* portno, char type);
int send_verif(int sockfd, char * msg);
int recv_verif(int sockfd, char * buffer);
int socket_ready(int sockfd, unsigned timeout_ms);
void send_char(int sockfd, char msg);

void display_board();
void set_variable();
void set_sym_board();

int validate(int first_x, int first_y, int sec_x, int sec_y);
void atualize_board(int first_x, int first_y, int sec_x, int sec_y);
int square_ware (char player);
int game_end();
void points();
char changePlayer(char player);

int play(char player);

void send_message(char* port, char* message);
void recieve_message(char* port);
void prepare_message(char player);
char read_message();

#endif