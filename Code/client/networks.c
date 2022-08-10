#include "networks.h"

int broadcasting = false;

char* say_hello = "Hi your message recieved!";

void error(char* msg)
{
	printf("\nError : %s\n", msg);
	exit(1);
}

int init_client(const char* portno, char type)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_addr;

	if (sockfd < 0)
		error("creating socket");

	// Configure serv_addr
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST_ADDR);
	serv_addr.sin_port = htons(atoi(portno));

	//connecting
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error(" connecting ");

	send_char(sockfd, type);

	return sockfd;
}

int send_verif(int sockfd, char * msg)
{
	int sent = 0;
	int n = 0;
	int tries = 0;
	while(sent < BUFFER_SIZE && tries < MAX_TRIES)
	{
		tries += 1;
		n = send(sockfd, msg, BUFFER_SIZE, 0);
		sent += n;
	}
	if (tries >= MAX_TRIES)
	{
		write(1, "Data not sent\n", 15);
		return -1;
	}
	return 0;
}

int recv_verif(int sockfd, char * buffer)
{
	int received = 0;
	int n = 0;
	int tries = 0;
	while(received < BUFFER_SIZE && tries < MAX_TRIES)
	{
		tries += 1;
		n = recv(sockfd, buffer, BUFFER_SIZE, 0);
		received += n;
	}
	if (tries >= MAX_TRIES)
	{
		write(1, "Data not received\n", 19);
		return -1;
	}

	return 0;
}

int socket_ready(int sockfd, unsigned timeout_ms)
{
	fd_set readfs;
	struct timeval timeout;

	timeout.tv_sec = 0; // 0s
	timeout.tv_usec = timeout_ms * 1000; // to µs

	FD_ZERO(&readfs);
	FD_SET(sockfd, &readfs);

	if (select(sockfd + 1, &readfs, NULL, NULL, &timeout) > 0)
		if(FD_ISSET(sockfd, &readfs))
			return 1;
	return 0;
}

void send_char(int sockfd, char msg)
{
	char buffer[BUFFER_SIZE];

	memset(buffer, 0 , BUFFER_SIZE);
	buffer[0] = msg;

	send_verif(sockfd, buffer);
}

void display_board()
{
	printf("  ");

	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (i % 2 == 0)
		{
			printf("%d", i);
		}
		else 
		{
			printf("     ");
		}
	}

	printf("\n");
	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (i % 2 == 0)
		{
			printf("%d ", i);
			for (int j = 0; j < BOARD_SIZE; j++) 
			{
				if (j % 2 == 0) 
				{
					 printf("%c", board[i*BOARD_SIZE + j]);
				}

				else 
				{
					if (board[i*BOARD_SIZE + j] == '-')
					{
						printf("-----");
					}
					if (board[i*BOARD_SIZE + j] == '0')
					{
						printf("     ");
					}
				}
			}
			printf("\n");
		}
	
		if (i % 2 == 1)
		{
			printf("  ");
			for (int j = 0; j < BOARD_SIZE; j++) 
			{
				if (j % 2 == 0) 
				{
					if (board[i*BOARD_SIZE + j] == '|')
					{
						printf("|");
					}
					if (board[i*BOARD_SIZE + j] == '0')
					{
						printf(" ");
					}
				}

				else 
				{
					if (board[i*BOARD_SIZE + j] != ' ')
					{
						printf("  %c  ", board[i*BOARD_SIZE + j]);
					}
					if (board[i*BOARD_SIZE + j] == ' ')
					{
						printf("     ");
					}
				}
			}
			printf("\n");
		}        
	}    
}

void set_variable()
{
	points_A = 0;
	points_B = 0;
	points_C = 0;
	points_D = 0;

	if (player_number == 2)
	{
		BOARD_SIZE = 5;
	}
	else if (player_number == 3)
	{
		BOARD_SIZE = 7;
	}
	else if (player_number == 4)
	{
		BOARD_SIZE = 9;
	}
	else
	{
		error("inavalid player number!!");
	}
}

void set_sym_board()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (i % 2 == 0)
		{
			for (int j = 0; j < BOARD_SIZE; j++) 
			{
				if (j % 2 == 0)
				{
					board[i*BOARD_SIZE + j] = '+';
				}

				else 
				{
					board[i*BOARD_SIZE + j] = '0';
				}
			}
		}
	
		if (i % 2 == 1)
		{
			for (int j = 0; j < BOARD_SIZE; j++) 
			{
				if (j % 2 == 0) 
				{
					board[i*BOARD_SIZE + j] = '0';
				}

				else 
				{
					board[i*BOARD_SIZE + j] = ' ';
				}
			}
		}        
	}
}

int validate(int first_x, int first_y, int sec_x, int sec_y)
{
    int cord_x, cord_y, dist_x, dist_y;

    dist_x = sec_x - first_x;
    dist_y = sec_y - first_y;
    
    cord_x = (first_x + sec_x) / 2;
    cord_y = (first_y + sec_y) / 2;

    if (board[first_x*BOARD_SIZE + first_y] == '+' && board[sec_x*BOARD_SIZE + sec_y] == '+')
    {
        if ((dist_x == 2 || dist_x == -2 || dist_x == 0) && (dist_y == 2 || dist_y == -2 || dist_y == 0))
        {
            if (board[cord_x*BOARD_SIZE + cord_y] == '0')
            {
                return 1;
            }
        }
    }

    return 0;
} 

void atualize_board(int first_x, int first_y, int sec_x, int sec_y)
{
    int cord_x, cord_y;

    cord_x = (first_x + sec_x) / 2;
    cord_y = (first_y + sec_y) / 2;

    if (cord_x % 2 == 0)
    {
        board[cord_x*BOARD_SIZE + cord_y] = '-';

    }
    else
    {
        board[cord_x*BOARD_SIZE + cord_y] = '|';
    }

}

int square_ware (char player)
{
    int val = 0;
    for (int i = 0; i < BOARD_SIZE - 1; i += 2)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j += 2)
        {
            if (board[i*BOARD_SIZE + (j+1)] != '0' && board[(i+1)*BOARD_SIZE + j] != '0' && 
            	board[(i+1)*BOARD_SIZE + (j+2)] != '0' && board[(i+2)*BOARD_SIZE + (j+1)] != '0')
            {
                if (board[(i + 1)*BOARD_SIZE + j + 1] == ' ')
                {
                    board[(i + 1)*BOARD_SIZE + j + 1] = player;
                    val += 1;
                }
            }
        }
    }

    if (val > 0)
    {
        return 1;
    }

    return 0;
}

int game_end()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i*BOARD_SIZE + j] == '0')
            {
                return 0;
            }
        }
    }

    return 1;
}

void points()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i*BOARD_SIZE + j] == 'A')
            {
                points_A += 1;
            }

            else if (board[i*BOARD_SIZE + j] == 'B')
            {
                points_B += 1;
            }
        }
    }   
}

char changePlayer(char player)
{
    if (player == 'A')
    {
        return 'B';
    }
    else if (player == 'B')
    {
    	if (player_number == 2)
    	{
    		return 'A';
    	}
    	else if (player_number == 3 || player_number == 4)
    	{
    		return 'C';
    	}
    }
    else if (player == 'C')
    {
    	if (player_number == 3)
    	{
    		return 'A';
    	}
    	else if (player_number == 4)
    	{
    		return 'D';
    	}
    }
    if (player == 'D')
    {
    	return 'A';
    }
}

int play(char player)
{
	int hit, numBytes;
 
	while(1)
	{
		memset(choice, 0 , BUFFER_SIZE);
		numBytes = read(1, choice, BUFFER_SIZE);
		choice[numBytes] = '\0';

		first_x = choice[1] - '0';
		first_y = choice[3] - '0';
		sec_x   = choice[7] - '0';
		sec_y   = choice[9] - '0';

		if (validate(first_x, first_y, sec_x, sec_y) == 0)
		{
			write(1, "Your choise is not valid! Please try again:\n", 45);
		}
		else
		{
			break;
		}
	}

    atualize_board(first_x, first_y, sec_x, sec_y);
    hit = square_ware(player);

    return hit;
}

void send_message(char* port, char* message)
{
	fd_set readfd;
	
	char buffer[BUFFER_SIZE];

	int sock, addr_len, count, ret;
	int yes = 1;

	struct sockaddr_in broadcast_addr;
	struct sockaddr_in server_addr;
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)
		error("sock error");
	
	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
	if (ret == -1)
		error("setsockopt error");

	addr_len = sizeof(struct sockaddr_in);

	memset((void*)&broadcast_addr, 0, addr_len);
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	broadcast_addr.sin_port = htons(PORT);
	
	ret = sendto(sock, message, strlen(message), 0, (struct sockaddr*) &broadcast_addr, addr_len);
}

void recieve_message(char* port)
{
	fd_set readfd;

	char buffer[BUFFER_SIZE];
	
	int sock, addr_len, count, ret;
	int opt = 1;

	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)
		error("sock error\n");
  
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
        error("setsockopt");

	addr_len = sizeof(struct sockaddr_in);

	memset((void*)&server_addr, 0, addr_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	ret = bind(sock, (struct sockaddr*)&server_addr, addr_len);
	if (ret < 0) 
		error("bind error\n");

	while (1) 
	{
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock+1, &readfd, NULL, NULL, 0);
		if (ret > 0) 
		{
			if (FD_ISSET(sock, &readfd)) 
			{
				memset(&message, '\0', sizeof(message));
				count = recvfrom(sock, message, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
				break;
			}
		}

	}
}

char read_message()
{
	char player;

	player = message[0];
	winner = message[1];
	hit = message[2] - '0';
	
	for (int i = 0; i < 11; ++i)
	{
		choice[i] = message[i + 3];
	}

	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i)
	{
		board[i] = message[i + 14];
	}

	return player;	
}

void prepare_message(char player)
{
	message[0] = player;
	message[1] = winner;
	message[2] = hit + '0';

	for (int i = 0; i < 11; ++i)
	{
		message[i + 3] = choice[i];
	}

	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; ++i)
	{
		message[i + 14] = board[i];
	}
}
