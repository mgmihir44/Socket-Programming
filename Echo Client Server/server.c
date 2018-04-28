#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char * argv[]){


	if(argc != 2){
		printf("Use: %s port number\n", argv[0]);
		exit(1);
	}

	/* Store the data received from the client */
	char data[1024];
	struct sockaddr_in server, client;
	socklen_t client_len;

	/* Create a TCP Server Socket */
	int s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(s_socket == -1){
		printf("Cannot create socket.\n");
		exit(1);
	}

	/* Initialize the parameters required for the connection */
	server.sin_family = AF_INET;				/* Working with IPv4 Addresses */
	server.sin_port = htons(atoi(argv[1]));			/* Initialize server on port provided by client */
	server.sin_addr.s_addr = INADDR_ANY;			/* Listen on al interfaces on our machine -> 0.0.0.0 */

	/* Make the socket reuseable */
	if(setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1){
		printf("setsockopt(SO_REUSEADDR) failed");
		exit(1);
	}
	
	/* Bind the server to the IP address and the port initialized */
	if(bind(s_socket, (struct sockaddr *)&server, sizeof(server)) == -1){
		printf("Bind failed");
		exit(1);
	}

	/* Start listening for incoming connections */
	if(listen(s_socket, 10)){
		printf("Cannot listen");
		exit(1);
	}

	while(1){
		int c_socket;
		/* Accept Connections from clients */
		if((c_socket = accept(s_socket, (struct sockaddr *)&client, &client_len)) == -1){
			printf("Cannot accept connections");
			exit(1);
		}

		/* Display client deitals */
		printf("Client connected with IP Address: %s\n", inet_ntoa(client.sin_addr));

		/* receive parameters */
		int input = 1;

		/* Keep receiving while client has data to send */
		while(input){
			input = recv(c_socket, &data, sizeof(data), 0);
			if(input == -1){
				printf("Cannot receive data");
				exit(1);
			}

			data[input] = '\0';				/* NULL terminate the data before printing */
			printf("Received from Client: %s\n", data);

			/* send/echo received data back to client */
			if(send(c_socket, data, input, 0) == -1){
				printf("Cannot send data to client");
				exit(1);
			}
		}
		printf("Client Disconnected\n");
		close(c_socket);
	}
	
	close(s_socket);
	return 0;
}
