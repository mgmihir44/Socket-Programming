#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER 1024

int main(int argc, char * argv[]){

	if(argc != 2){
		printf("Use: %s port", argv[0]);
		exit(1);
	}

	char data[BUFFER];							/* Store the data received from client */
	char s_data[BUFFER];							/* Send data received from user to client */
	pid_t multi_client;
	struct sockaddr_in server, client;					/* Store Client and Server details */
	socklen_t client_len;
	int c_socket;

	/* Create a TCP Server Socket */
	int s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(s_socket == -1){
		printf("Cannot create socket\n");
		exit(1);
	}

	/* Initialize parameters required to establish the connection */
	server.sin_family = AF_INET; 						/* Working with IPv4 */
	server.sin_port = htons(atoi(argv[1]));					/* Port number provided by user */
	server.sin_addr.s_addr = INADDR_ANY;					/* Listen on all local interfaces of the host machine */

	/* Make the socket reuseable */
	if(setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1){
		printf("Cannot make socket reuseable\n");
		exit(1);
	}

	/* Bind the server the to IP address and the defined port */
	if(bind(s_socket, (struct sockaddr *)&server, sizeof(server)) == -1){
		printf("Cannot bind server\n");
		exit(1);
	}

	/* Start listening for connections */
	if(listen(s_socket, 10)){
		printf("Cannot listen for connections\n");
		exit(1);
	}

	while(1){
		/* Accept conncetions from clients */
		c_socket = accept(s_socket, (struct sockaddr*)&client, &client_len);
		if(c_socket == -1){
			printf("Cannot accept connections from clients\n");
			exit(1);
		}
		
		/* Display Client Details */
		printf("Client connected with IP Address: %s\n", inet_ntoa(client.sin_addr));
		printf("Client port number: %d\n", ntohs(client.sin_port));

		/* Create a new process to handle the connection */
		multi_client = fork();
		if(multi_client == 0){
			/* Close the server socket */
			close(s_socket);

			while(1){
				/* Receive data from clients */
				int input;
				if((input = recv(c_socket, data, BUFFER, 0)) == -1){
					printf("Cannot receive data from client\n");
					exit(1);
				}

				data[input] = '\0';				/* Null terminate data received from client */
				printf("Client: %s\n", data);

				/* Get data from user to send to client */	
				printf("Server:");
				fgets(s_data, BUFFER, stdin);		/*Get input from user */
					
				/* Send Data to client */
				if(send(c_socket, s_data, strlen(s_data), 0) == -1){
					printf("Cannot send data to client\n");
					exit(1);
				}
				
			}
		}
	}
	close(c_socket);

	return 0;
}
