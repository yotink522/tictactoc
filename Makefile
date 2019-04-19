CC=gcc
CFLAGS=-g -Wall
OBJS=server.o client.o
TARGET=app.out
 
$(TARGET): $(OBJS)
    $(CC) -o $@ $(OBJS)
 
server.o: tic_server.c
client.o: tic_client.c
