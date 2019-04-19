#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXLINE 1024

int board[9];
int turn;	//can take values from 1-9
char strResult[256];
char strBoard[256];

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
	free(p);
}

int parse_packet(char* str)
{
	char* p = strtok(str, "|");
	if (p == NULL)
	{
		printf("Cannot parse the received packet\n");
		return 0;
	}
	strcpy(strBoard, p);

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
	strdup(strResult, p);
	free(p);
	return 1;
}

void print_board()
{
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
			printf(" | %d ",i*3+2);

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
	char buf[MAXLINE];
	
	while (1){
		if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("error : ");
			return 1;
		}
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serveraddr.sin_port = htons(4000);

		client_len = sizeof(serveraddr);

		if(connect(server_sockfd, (struct sockaddr*)&serveraddr, client_len) == -1) {
			perror("connect error : ");
			return 1;
		}
		memset(buf, 0x00, MAXLINE);
		
		read(0, buf, MAXLINE);
		
		if(write(server_sockfd, buf, MAXLINE) <= 0) {
			perror("write error : ");
			return 1;
		}
		memset(buf, 0x00, MAXLINE);
		if(read(server_sockfd, buf, MAXLINE) <= 0) {
			perror("read error: ");
			return 1;
		}
		
		close(server_sockfd);
	}
	return 0;
}
