all: server client
CC = gcc

server: tic_server.c
		$(CC) -o $@ $<
client: tic_client.c
		$(CC) -o $@ $<
clean:
		rm -f client server
