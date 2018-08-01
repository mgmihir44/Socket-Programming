/*
 * Author: Mihir Garude
 *
 * Date: 07/30/2018
 *
 * Description: A simple server application. This application will start listening on the local host
 *		machine on the port number specified as an argument.
 */

/*********************************** Includes Section **********************************************/
// Include all header files in this section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/********************************** Globals Section ************************************************/
// All global variables, macros and function definations go here
#define BUFFER 1024

// Function declarations
void error(const char * message);	/* Print error message and terminate */


/********************************** Main **********************************************************/
int main(int argc, char * argv[]){

	if(argc != 2){
		fprintf(stderr, "Usage: %s Port No", argv[1]);
		exit(1);
	}

	char recv_buf[BUFFER];				/* Buffer to store data received from client */
	char send_buf[BUFFER];				/* Holds data to send to client */
	struct sockaddr_in server, client;		/* Store client and server details */
	socklen_t client_length;
	int server_soc, client_soc;
	pid_t multi_client;
	
	/* Create a server socket */
	if((server_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		error("Cannot create a socket");
	}

	/* Initialize the connection parameters */
	server.sin_family = AF_INET;			/* Working with IPv4 */
	server.sin_family = htons(atoi(argv[1]));	/* Initialize server on Port provided by user */
	server.sin_addr.s_addr = INADDR_ANY;		/* Listen on all local interfaces of the host machine */

	/* Make socket reusable */
	if(setsockopt(server_soc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
		error("Cannot make socket reusable");
	}

	/* Bind the server to the IP address and the defined port number */
	if(bind(server_soc, (struct sockaddr * )&server, sizeof(server)) < 0){
		error("Cannot bind socket");
	}

	/* Start listening for connections from clients */
	if(listen(server_soc, 5) < 0){
		error("Cannot listen for connections");
	}

	while(1){
		/* Accept connections from client */
		client_soc = accept(server_soc, (struct sockaddr *)&client, &client_length);
		if(client_soc < 0){
			error("Cannot accept connections");
		}

		/* Display client details */
		fprintf(stdout, "Client connected with IP Address: %s\n", inet_ntoa(client.sin_addr));
		fprintf(stdout, "Client port number: %d\n", ntohs(client.sin_port));

		/* Create a seperate process for each client connection */
		multi_client = fork();
		if(multi_client = 0){
			/* close the socket */
			close(server_soc);

			while(1){
				int input;
				/* Receive data from clients */
				if((input = recv(client_soc, recv_buf, BUFFER, 0)) < 0){
					error("Error is receiving data from client");
				}

				recv_buf[input] = '\0';		/* Null terminate the string */
				fprintf(stdout, "Received from Client: %s\n", recv_buf);

				/* Response to send to clinet */
				fprintf(stdout, "Message for Client: ");
				fgets(send_buf, BUFFER, stdin);

				/* Send response to client */
				if(send(client_soc, send_buf, strlen(send_buf), 0) < 0){
					error("Send Error");
				}
			}
		}
		else if(multi_client < 0){
			error("fork error");
		}
	}
	close(client_soc);

	return 0;
}


/*
 * Description: Provide a descriptive message to the user and terminate.
 *
 * Input: Message to display
 *
 * Output: None
 */
void error(const char * message){

	perror(message);
	printf("\n");
	exit(1);

}
