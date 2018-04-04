CC = gcc
CFLAGS = -Wall -g 

all: server client

server.o: server.c server.h helpers_server.c
	$(CC) $(CFLAGS) -c $^

helpers_server.o: server.h helpers_server.c
	$(CC) $(CFLAGS) -c $^

server: server.o helpers_server.o
	$(CC) $^ $(CFLAGS) -o $@ -lpthread

client.o: client.c client.h helpers_client.c
	$(CC) $(CFLAGS) -c $^

helpers_client.o: client.h helpers_client.c
	$(CC) $(CFLAGS) -c $^

client: client.o helpers_client.o
	$(CC) $^ $(CFLAGS) -o $@	
