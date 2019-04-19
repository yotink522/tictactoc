#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma warning(disable:4996)

using namespace std;
int board[9];
int turn;	//can take values from 1-9
char strResult[256];
char strBoard[256];

void board_init()
{
	for (int i = 0; i < 9; i++) {
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
	strcpy(strResult, p);
	return 1;
}

void print_board()
{
	for (int i = 0; i < 3; i++)
	{
		if (board[3 * i + 0] == 3)
			cout << " " << "X" << " ";
		else if (board[3 * i + 0] == 5)
			cout << " " << "O" << " ";
		else cout << " " << i * 3 + 1 << " ";

		if (board[3 * i + 1] == 3)
			cout << " | X ";
		else if (board[3 * i + 1] == 5)
			cout << " | O ";
		else cout << " | " << i * 3 + 2 << " ";

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

int main(int argc, char** argv)
{
	board_init();
	turn = 1;

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	int iResult;
	int recvbuflen = 512;

	// Validate the parameters
	if (argc != 3) {
		printf("usage: %s server-name portno\n eg)%s 127.0.0.1 10001", argv[0], argv[0]);
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	print_board();
	while (true)
	{
		printf("Enter position to bet:");
		int inNo = 0;
		cin >> inNo;
		
		char sendStr[100] = { 0 };
		memset(sendStr, 0, sizeof(sendStr));

		sprintf(sendStr, "%d,%d\0", inNo, turn);
		int len = strlen(sendStr);

		iResult = send(ConnectSocket, sendStr, len, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		// receive response from server
		char szMsg[512];
		memset(szMsg, 0, sizeof(szMsg));
		int nLet = recv(ConnectSocket, szMsg, sizeof(szMsg), 0);

		//printf("Received From Server:%s\n", szMsg);
		if (parse_packet(szMsg) == 0)
		{
			printf("Parse error\n");
			return 1;
		}

		update_board(strBoard);
		print_board();

		if (strcmp(strResult, "continue") != 0)
		{
			printf("%s\n", strResult);
			break;
		}
	}
	
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	// Close winsock
	WSACleanup();
	return 0;
}
