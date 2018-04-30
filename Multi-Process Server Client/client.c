#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER 1024


int main(int argc, char * argv[]){


	if(argc != 3){
		printf("Use: %s ip port\n", argv[0]);
		exit(1);
	}

	char send_data[BUFFER];					/* Stores data which is to be sent to server */
	char recv_data[BUFFER];					/* Stores data received from the server */

	/* Create a TCP socket for the client */
	int c_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(c_socket == -1){
		printf("Cannot create client socket\n");
		exit(1);
	}

	/* Initialize parameters required for the connection */
	struct sockaddr_in remote_server;
	remote_server.sin_family = AF_INET;			/* Working with IPv4 */
	remote_server.sin_port = htons(atoi(argv[2]));		/* Port is provided by the user */
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);	/* IP address is provided by the user */

	/* Establish a connection to the server */
	if(connect(c_socket, (struct sockaddr *)& remote_server, sizeof(remote_server)) == -1){
		printf("Cannot connect to server\n");
		exit(1);
	}

	while(1){
		printf("Client: ");
		fgets(send_data, BUFFER, stdin);		/* Get data from user */

		/* Send data to server */
		if(send(c_socket, send_data, strlen(send_data), 0) == -1){
			printf("Cannot send data to server\n");
			exit(1);
		}
		
		/* Receive data from server */
		int recv_sock = recv(c_socket, recv_data, BUFFER, 0);
		if(recv_sock == -1){
			printf("Cannot receive data from server\n");
			exit(1);
		}

		recv_data[recv_sock] = '\0';			/* Null terminate the string received from server */

		fprintf(stdout, "Server: %s", recv_data);
	}

	return 0;
}
