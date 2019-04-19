#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXLINE 1024

int board[9];
int turn;	//can take values from 1-9
char *strResult;
char *strBoard;

void board_init()
{
	int i = 0;
	for (i = 0; i < 9; i++) {
		board[i] = 2;
	}
}

void update_board(char* str)
{
	int i = 0;
	char *p = strtok(str, ",");
	while (p != NULL) {
		board[i] = atoi(p);
		i++;
		p = strtok(NULL, ",");
	}
}

int parse_packet(char* str)
{
	char* p = strtok(str, "|");
	if (p == NULL)
	{
		printf("Cannot parse the received packet\n");
		return 0;
	}
	strBoard = strdup(p);

	p = strtok(NULL, "|");
	if (p == NULL)
	{
		printf("Cannot parse the received packet\n");
		return 0;
	}
	turn = atoi(p);

	p = strtok(NULL, "|");
	if (p == NULL)
	{
		printf("Cannot parse the received packet\n");
		return 0;
	}
	strResult = strdup(p);
	return 1;
}

void print_board()
{
	printf("O : Computer  X : You\n");
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		if (board[3 * i + 0] == 3)
			printf(" %s ","X");
		else if (board[3 * i + 0] == 5)
			printf(" %s ","O");
		else 
			printf(" %d ",i*3+1);
		if (board[3 * i + 1] == 3)
			printf(" | %s ","X");
		else if (board[3 * i + 1] == 5)
			printf(" | %s ","O");
		else 
			printf(" | %d ",i*3+2);

		if (board[3 * i + 2] == 3)
			printf(" | %s ","X");
		else if (board[3 * i + 2] == 5)
			printf(" | %s ","O");
		else 
			printf(" | %d ",i*3+3);

		printf("   \n");
		if (i < 2) {
			printf("---------------\n");
		}
	}
	printf("\n");
}

int main(int argc, char **argv) {
	struct sockaddr_in serveraddr;
	int server_sockfd;
	int client_len;
	
	int iResult;
	int recvbuflen = 512;
	char inNo[MAXLINE];
	char sendStr[MAXLINE];
	char buf[MAXLINE];
	
	if (argc != 3){
		printf("Invalid start\n usage of eg: %s 127.0.0.1 4000\n",argv[0]);
		return 0;
	}
	
	board_init();
	turn = 1;
	print_board();
	
	while (1){
		if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("error : ");
			return 1;
		}
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
		serveraddr.sin_port = htons(atoi(argv[2]));

		client_len = sizeof(serveraddr);

		if(connect(server_sockfd, (struct sockaddr*)&serveraddr, client_len) == -1) {
			perror("connect error : ");
			return 1;
		}
		
		printf("Enter position to bet:");
		memset(inNo, 0x00, MAXLINE);
		read(0, inNo, MAXLINE);
		printf("\n");
		
		memset(sendStr, 0x00, MAXLINE);
		sprintf(sendStr, "%d,%d", atoi(inNo), turn);
						
		if(write(server_sockfd, sendStr, MAXLINE) <= 0) {
			perror("write error : ");
			return 1;
		}
		
		memset(buf, 0x00, MAXLINE);
		if(read(server_sockfd, buf, MAXLINE) <= 0) {
			perror("read error: ");
			return 1;
		}
		
		if (parse_packet(buf) == 0) {
			printf("Parse error\n");
			return 1;
		}

		update_board(strBoard);
		print_board();

		if (strcmp(strResult, "continue") != 0) {
			printf("%s\n", strResult);
			break;
		}
		
		close(server_sockfd);
	}
	return 0;
}
