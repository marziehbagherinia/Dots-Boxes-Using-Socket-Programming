#include "client.h" 

int bonus = 0;

int main(int argc, char * argv[])
{
	int sockfd, numBytes;

	char player, input;
	char buffer[BUFFER_SIZE];

	if (argc != 2)
		error("Usage : client <portno>");

	write(1, "Welcome to Dots % Boxes!!!! Choose your option:\n", 49);
	write(1, "1) Two-player game.\n", 21);
	write(1, "2) Three-player game.\n", 23);
	write(1, "3) Four-player game.\n", 22);

	memset(buffer, 0 , BUFFER_SIZE);
	numBytes = read(1, buffer, BUFFER_SIZE);
	buffer[numBytes] = '\0';

	sockfd = init_client(argv[1], buffer[0]);

	player_number = (buffer[0] - '0') + 1;

	write(1, "Successfully connected to the server.\n", 38);

	while(1)
	{
		if(socket_ready(sockfd, 50))
		{
			memset(buffer, 0 , BUFFER_SIZE);

			if(recv_verif(sockfd, buffer) != -1)
			{
				player = buffer[0];

				write(1, "You are player ", 16);
				write(1, &player, 2);
				write(1, "\n", 2);
				write(1, "Waiting for other player...\n", 29);
				break;
			}
			error("Server disconnected");
		}
	}

	game_dots(player, sockfd);

	close(sockfd);
	return 0;
}

void game_dots(char you, int sockfd)
{
	write(1, "Beginning of Dots and Boxes!\n", 30);
	write(1, "Waiting for other player...\n", 29);

	char buffer [BUFFER_SIZE];
	char player = '0';
	winner = '0';
	char* port = (char*)malloc(4 * sizeof(char)); ;

	int numBytes, broadfd;
    int hit;
	int addr_len = addr_len = sizeof(struct sockaddr_in);
	int br;
	hit = 0;

	while(1)
	{
		if(socket_ready(sockfd, 50))
		{
			memset(buffer, 0 , BUFFER_SIZE);

			if(recv_verif(sockfd, buffer) == -1)
			{
				write(1, "Server disconnected...\n\n", 25);
				winner = '0';
				break;
			}

			else
			{
				player = buffer[0];

				for (int i = 0; i < 4; ++i)
				{
					port[i] = buffer[i + 1];
				}

				break;
			}
		}
	}

	set_variable();

	set_sym_board();

	while (winner == '0')
	{
		display_board();

		if (player == you)
		{
			if (hit)
			{
				write(1, "It's your bonus:) move again!\n", 31);
			}
			else
			{
				write(1, "Hey ", 5);
				write(1, &you, 2);
				write(1, ", It is your turn! valid move is: (x,y),(j,v)\n", 47);				
			}

			hit = play(player);

			if (game_end() == 1)
			{
				points();

				if (points_A > points_B)
				{
					winner = 'A';
				}
				else if (points_B > points_A)
				{
					winner = 'B';
				}
				else if (points_A == points_B)
				{
					winner = 'N';
				}
			}
			prepare_message(player);
			send_message(port, message);
			
			if (hit == 0)
			{
				player = changePlayer(player);
			}

		}
		else
		{
			write(1, "Your opponent is playing, please wait...\n", 42);
			recieve_message(port);
			player = read_message();
			if (hit)
			{
				write(1, "Player ", 8);
				write(1, &player, 1);
				write(1, " win the bonus!!!\n", 19);
			}
			else
			{
				write(1, "Player ", 8);
				write(1, &player, 1);
				write(1, " opponent choise is: ", 22);
				write(1, &choice, 12);
				write(1, "\n", 2);
				player = changePlayer(player);
			}
		}		
	}

	display_board();

	// end of game
	if (winner == '0' || winner == 'N')
		write(1, "Draw!\n", 7);
	else if (winner == you)
		write(1, "You Won!\n", 10);
	else
		write(1, "You Lose!\n", 11);
}