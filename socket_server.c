#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
 
#define BUFF_SIZE 1024
 
int board[9];	//vector to represent the board
				//The contents of the board - 2 if empty place, 3 if X has played a position, 5 if O has played

int turn;	//can take values from 1-9
bool round_finish_flag = false;

int mul(int a, int b, int c);
int posswin(int p)
{

	//check if next move is winning move and return that move for player p.
	//This is done by multiplying the elements of each row, column and diagonals
	//If the elements are 3,3,2, then playing a 3 (for X) will result in a win. (3*3*2 = 18)
	//If the elements are 5,5,2, then playing a 5 (for O) will result in a win. (5*5*2 = 50)

	int i;
	for (i = 0; i < 9; i++)
	{
		if (board[i] == 2)
		{
			//row1 - 012
			if (mul(board[0], board[1], board[2]) == p * p * 2)
			{
				for (i = 0; i < 3; i++)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//row2 - 345
			if (mul(board[3], board[4], board[5]) == p * p * 2)
			{
				for (i = 3; i < 6; i++)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//row3 - 678
			if (mul(board[6], board[7], board[8]) == p * p * 2)
			{
				for (i = 6; i < 9; i++)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//col1 - 036
			if (mul(board[0], board[3], board[6]) == p * p * 2)
			{
				for (i = 0; i < 7; i += 3)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//col2 - 147
			if (mul(board[1], board[4], board[7]) == p * p * 2)
			{
				for (i = 1; i < 8; i += 3)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//col3 - 258
			if (mul(board[2], board[5], board[8]) == p * p * 2)
			{
				for (i = 2; i < 9; i += 3)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//d1 - 048
			if (mul(board[0], board[4], board[8]) == p * p * 2)
			{
				for (i = 0; i < 9; i += 4)
				{
					if (board[i] == 2)
						return i;
				}
			}
			//d2 - 246
			if (mul(board[2], board[4], board[6]) == p * p * 2)
			{
				for (i = 2; i < 7; i += 4)
				{
					if (board[i] == 2)
						return i;
				}
			}
		}

	}

	return 0;
}

int checkwin(int p)
{
	//Self explanatory- checks if a players last move resulted in a win by examining
	//rows columns and diagonals for a set of 3 X's or O's depending on player p(3-X,5-O)

	if (board[0] == p && board[1] == p && board[2] == p)
	{
		return p;
	}
	//row2 - 345
	if (board[3] == p && board[4] == p && board[5] == p)
	{
		return p;
	}
	//row3 - 678
	if (board[6] == p && board[7] == p && board[8] == p)
	{
		return p;
	}
	//col1 - 036
	if (board[0] == p && board[3] == p && board[6] == p)
	{
		return p;
	}
	//col2 - 147
	if (board[1] == p && board[4] == p && board[7] == p)
	{
		return p;
	}
	//col3 - 258
	if (board[2] == p && board[5] == p && board[8] == p)
	{
		return p;
	}
	//d1 - 048
	if (board[0] == p && board[4] == p && board[8] == p)
	{
		return p;
	}
	//d2 - 246
	if (board[2] == p && board[4] == p && board[6] == p)
	{
		return p;
	}

	return 0;
}


int mul(int a, int b, int c)
{
	//multiply 3 integers
	return a * b* c;
}

int make2()
{
	//if center of board is free return it. Otherwise return any free position.
	if (board[4] == 2)
		return 4;
	else
	{
		for (int i = 1; i < 8; i += 2)
		{
			if (board[i] == 2)
				return i;
		}
		return 4;
	}
}

void go(int n)
{
	//Make the play. Put an X or O in the position n.
	//First player to begin plays odd turns(X) and second plays even ones(O)
	if (turn % 2 == 0)
	{
		board[n] = 5;
	}
	else
	{
		board[n] = 3;
	}
}

void incturn()
{
	turn++;
}



void refresh_board()
{
	//Show the board. If you want to clear the screen before this, please do so..
	cout << endl;

	if (turn % 2 == 0)
		cout << "Computer's move: " << endl;
	if (turn % 2 != 0)
		cout << "Player's move: " << endl;

	for (int i = 0; i < 3; i++)
	{
		//cout<<" "<<i*3+1<<" ";
		if (board[3 * i + 0] == 3)
			//cout<<"X";
			cout << " " << "X" << " ";
		else if (board[3 * i + 0] == 5)
			//cout<<"O";
			cout << " " << "0" << " ";
		else cout << " " << i * 3 + 1 << " ";

		//cout<<" | "<<i*3+2<<" ";
		if (board[3 * i + 1] == 3)
			//cout<<"X";
			cout << " | X ";
		else if (board[3 * i + 1] == 5)
			//cout<<"O";
			cout << " | O ";
		else cout << " | " << i * 3 + 2 << " ";

		//cout<<" | "<<i*3+3<<" ";
		if (board[3 * i + 2] == 3)
			cout << " | X ";
		else if (board[3 * i + 2] == 5)
			cout << " | O ";
		else cout << " | " << i * 3 + 3 << " ";

		cout << "   " << endl;
		if (i < 2) {
			cout << "---------------" << endl;
		}
	}

	cout << endl;
}

char* make_board_value()
{
	char res[256] = { 0 };
	memset(res, 0, sizeof(res));

	for (int i = 0; i < 9; i++) {
		if (i == 0)
			sprintf(res, "%d", board[i]);
		else
			sprintf(res, "%s,%d", res, board[i]);
	}
	return strdup(res);
}

char* check_result(int client_no, int turn_no)
{
	char res[256] = { 0 };
	memset(res, 0, sizeof(res));

	switch (turn_no)
	{
	case 1:
		client_no--;
		go(client_no);
		if (checkwin(3) == 3)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Client Win!");
			round_finish_flag = true;
			return strdup(res);
		}
		incturn();

		if (board[4] == 2)
			go(4);
		else
			go(0);

		if (checkwin(5) == 5)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Server Win!");
			round_finish_flag = true;
			return strdup(res);
		}
		incturn();

		sprintf(res, "%s|%d|%s", make_board_value(), turn, "continue");
		return strdup(res);
	case 3:
		client_no--;
		go(client_no);
		if (checkwin(3) == 3)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Client Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		incturn();
		if (posswin(3) != 0)
		{
			go(posswin(3));
		}
		else
			go(make2());

		if (checkwin(5) == 5)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Server Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		incturn();

		sprintf(res, "%s|%d|%s", make_board_value(), turn, "continue");
		return strdup(res);
		break;
	case 5:
		client_no--;
		go(client_no);
		if (checkwin(3) == 3)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Client Win!");
			round_finish_flag = true;
			return strdup(res);
		}
		incturn();//6

		if (posswin(5) != 0)
		{
			go(posswin(5));
		}
		else if (posswin(3) != 0)
		{
			go(posswin(3));
		}
		else
			go(make2());

		if (checkwin(5) == 5)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Server Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		incturn();//7
		sprintf(res, "%s|%d|%s", make_board_value(), turn, "continue");
		return strdup(res);

		break;
	case 7:
		client_no--;
		go(client_no);
		if (checkwin(3) == 3)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Client Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		incturn();//8
		if (posswin(5) != 0)
		{
			go(posswin(5));
		}
		else if (posswin(3) != 0)
		{
			go(posswin(3));
		}
		else
			go(make2());

		if (checkwin(5) == 5)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Server Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		incturn();//9
		sprintf(res, "%s|%d|%s", make_board_value(), turn, "continue");
		return strdup(res);

		break;
	case 9:
		client_no--;
		go(client_no);
		if (checkwin(3) == 3)
		{
			sprintf(res, "%s|%d|%s", make_board_value(), turn, "Client Win!");
			round_finish_flag = true;
			return strdup(res);
		}

		sprintf(res, "%s|%d|%s", make_board_value(), turn, "Game in draw!");
		round_finish_flag = true;
		return strdup(res);

		break;
	}
	return 0;
}

void initialize_for_newgame()
{
	for (int i = 0; i < 9; i++) {
		board[i] = 2;
	}
	round_finish_flag = false;
}

int main(void){
   int server_socket;
   int client_socket;
   int client_addr_size;
 
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
 
   char buff_rcv[BUFF_SIZE+5];
   char buff_snd[BUFF_SIZE+5];
 
   server_socket = socket(PF_INET, SOCK_STREAM, 0);
   if(-1 == server_socket){
      printf( "Failed to create server socket\n");
      exit( 1);
   }
 
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(4000);
   server_addr.sin_addr.s_addr= htonl(INADDR_ANY);
 
   if(-1 == bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
      printf( "Error on bind()\n");
      exit(1);
   }
 
   while(1){
      if(-1 == listen(server_socket, 5)){
         printf( "Failed to set on standby mode\n");
         exit(1);
      }
 
      client_addr_size  = sizeof( client_addr);
      client_socket = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
 
      if (-1 == client_socket){
         printf( "Failed to receive from client\n");
         exit(1);
      }
 
      read (client_socket, buff_rcv, BUFF_SIZE);
      printf("receive: %s\n", buff_rcv);
 
      sprintf(buff_snd, "%ld : %s", strlen(buff_rcv), buff_rcv);
      write(client_socket, buff_snd, strlen(buff_snd)+1);          // +1: transfer include NULL
      close(client_socket);
   }
}
