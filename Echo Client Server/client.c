#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char * argv[]){

	if(argc != 3){
		printf("Use: %s ip port\n", argv[0]);
		exit(1);
	}
	
	char send_data[512];					/* Buffer to send data to echo server */
	char recv_data[512];					/* Buffer to store received data from echo server */

	/* Create a TCP socket */
	int c_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(c_socket == -1){
		printf("Cannot create socket.");
		exit(1);
	}
	
	/* Set Parameters to make the connections */
	struct sockaddr_in echo_client;
	echo_client.sin_family = AF_INET;			/* Working with IPv4 */
	echo_client.sin_port = htons(atoi(argv[2]));		/* Connect to server on port specified by user */
	echo_client.sin_addr.s_addr = inet_addr(argv[1]);	/* Connect to server ip specified by user */

	if(connect(c_socket, (struct sockaddr*)&echo_client, sizeof(echo_client))){
		printf("Cannot establish connection");
		exit(1);
	}

	while(1){
		
		fgets(send_data, 512, stdin);			/* Get input from user */

		/* Send data to server */
		if(send(c_socket, send_data, strlen(send_data), 0) == -1){
			printf("Cannot send data to server");
			exit(1);
		}

		/* Receive data from server */
		int recv_sock = recv(c_socket, recv_data, 512, 0);
		if(recv_sock == -1){
			printf("Cannot receive data");
			exit(1);
		}

		recv_data[recv_sock] = '\0';			/* Null terminate data before printing */

		fprintf(stdout, "Message from Server: %s\n", recv_data);
		
	}

	close(c_socket);
	return 0;
}
