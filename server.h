#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void * connection_handler(void *);
int open_listner_socket();
void bind_to_port(int, int);
int response(int , char*);
