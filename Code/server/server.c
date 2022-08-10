#include "server.h"

int main(int argc, char * argv[])
{
	int sockfd;

	if (argc != 2)
		error("Usage : server <portno>");

	sockfd = init_server(argv[1]);

	listen(sockfd, 42);

	game_dots(sockfd);

	write(1, "\nServer shutdown!\n", 19);
	close(sockfd);
	return 0;
}

void game_dots(int sockfd)
{
	write(1, "Beginning of Dots and Boxes!\n", 30);
	
	int current_player;
	int sock;

	char buffer [BUFFER_SIZE];
	char symbols[] = {'A', 'B', 'C', 'D'};
	char* rec_ports[] = {"3000", "4000", "5000", "6000"};
	
	bool send_port = FALSE;
	bool send_port_3 = FALSE;
	bool send_port_4 = FALSE;
	
	char game_ended = '0';
	ports = 0;
	char* help = (char*)malloc(4 * sizeof(char));

	struct client_set player = client_set_init();
	struct client_set player_3 = client_set_init();
	struct client_set player_4 = client_set_init();

	while(TRUE)
	{
		if(socket_ready(sockfd, 50))
		{
			sock = wait_client(sockfd);
			memset(buffer, 0, BUFFER_SIZE);
			recv_verif(sock, buffer);

			switch (buffer[0])
			{
				case '1': // player
					write(1, "One new player tried to connect\n", 31);
					
					if (player.nb < MAX_PLAYER)
					{
						client_set_add(&player, sock, symbols[player.nb]);
						send_char(sock, player.client[player.nb - 1].id);

						write(1, "Player ", 8);
						write(1, &player.client[player.nb-1].id, 2);
						write(1, " successfully connected!\n", 26);

						if (player.nb == MAX_PLAYER)
						{
							write(1, "All the player are connected... Let's start the game!\n\n", 56);
							send_port = TRUE;
							break;
						}
						else
						{
							char help = (MAX_PLAYER - player.nb) + '0';
							write(1, &help, 2);
							write(1, " more players required!\n\n", 26);
						}
					}
					else
					{
						write(1, "Cannot accept any more client\n\n", 32);
					}

					break;

				case '2': // player
					write(1, "A new player tried to connect\n", 31);
					
					if (player_3.nb < THREE_MAX_PLAYER)
					{
						client_set_add(&player_3, sock, symbols[player_3.nb]);
						send_char(sock, player_3.client[player_3.nb-1].id);

						write(1, "Player ", 8);
						write(1, &player_3.client[player_3.nb-1].id, 2);
						write(1, " successfully connected!\n", 26);

						if (player_3.nb == THREE_MAX_PLAYER)
						{
							write(1, "All the player are connected... Let's start the game!\n\n", 56);
							send_port_3 = TRUE;
							break;
						}
						else
						{
							char help = (THREE_MAX_PLAYER - player_3.nb) + '0';
							write(1, &help, 2);
							write(1, " more players required!\n\n", 26);
						}
					}
					else
					{
						write(1, "Cannot accept any more client\n\n", 32);
					}

					break;

				case '3': // player
					write(1, "A new player tried to connect\n", 31);
					
					if (player_4.nb < FOUR_MAX_PLAYER)
					{
						client_set_add(&player_4, sock, symbols[player_4.nb]);
						send_char(sock, player_4.client[player_4.nb-1].id);

						write(1, "Player ", 8);
						write(1, &player_4.client[player_4.nb-1].id, 2);
						write(1, " successfully connected!\n", 26);

						if (player_4.nb == FOUR_MAX_PLAYER)
						{
							write(1, "All the player are connected... Let's start the game!\n\n", 56);
							send_port_4 = TRUE;
							break;
						}
						else
						{
							char help = (FOUR_MAX_PLAYER - player_4.nb) + '0';
							write(1, &help, 2);
							write(1, " more players required!\n\n", 26);
						}
					}
					else
					{
						write(1, "Cannot accept any more client\n\n", 32);
					}

					break;
				default:
					write(1, "An unknown client has been rejected\n\n", 38);
					close(sock);
					break;
			}
		}

		if (send_port)
		{
			memset(buffer, 0, BUFFER_SIZE);

			help = rec_ports[ports];
			ports++;

			buffer[0] = player.client[0].id;			
			buffer[1] = help[0];
			buffer[2] = help[1];
			buffer[3] = help[2];
			buffer[4] = help[3];
			
			client_set_send(&player, buffer);
			
			send_port = FALSE;
		} 

		if(send_port_3)
		{
			memset(buffer, 0, BUFFER_SIZE);

			help = rec_ports[ports];
			ports++;

			buffer[0] = player_3.client[0].id;			
			buffer[1] = help[0];
			buffer[2] = help[1];
			buffer[3] = help[2];
			buffer[4] = help[3];
			
			client_set_send(&player_3, buffer);

			send_port_3 = FALSE;
		}

		if (send_port_4)
		{
			memset(buffer, 0, BUFFER_SIZE);

			help = rec_ports[ports];
			ports++;

			buffer[0] = player_4.client[0].id;			
			buffer[1] = help[0];
			buffer[2] = help[1];
			buffer[3] = help[2];
			buffer[4] = help[3];
			
			client_set_send(&player_4, buffer);

			send_port_4 = FALSE;
		}    

	}

	client_set_close(player);
	client_set_close(player_3);
	client_set_close(player_4);		
}