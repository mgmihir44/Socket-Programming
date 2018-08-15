/*
 * Author: Mihir Garude
 *
 * Date: 07/30/2018
 *
 * Date Modified: 08/14/2018
 *
 * Description: A simple server application. This application will start listening on the local host
 *		machine on the port number specified as an argument and ask the client for 
 * 		two number and the arithmetic operation to perform
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
		fprintf(stderr, "Usage: %s Port No\n", argv[0]);
		exit(1);
	}

	int value_one, value_two, result, operation;	/* Buffer to Store the operation, values and the result*/
	struct sockaddr_in server, client;		/* Store client and server details */
	int w_fd, r_fd;					/* File descriptors for read and write operations */
	socklen_t client_length;
	int server_soc, client_soc;
	pid_t multi_client;
	
	/* Create a server socket */
	if((server_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		error("Cannot create a socket");
	}

	/* Initialize the connection parameters */
	server.sin_family = AF_INET;			/* Working with IPv4 */
	server.sin_port = htons(atoi(argv[1]));	/* Initialize server on Port provided by user */
	server.sin_addr.s_addr = INADDR_ANY;		/* Listen on all local interfaces of the host machine */

	/* Make socket reusable */
	/* We are using sockets at API level, so second parameter to
	 * the function should be SOL_SOCKET
	 */
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

	fprintf(stdout, "Server started on Port No: %d\n", atoi(argv[1]));
	
	/* Accept connections from client */
	client_soc = accept(server_soc, (struct sockaddr *)&client, &client_length);
	if(client_soc < 0){
		error("Cannot accept connections");
	}

	/* Display client details */
	fprintf(stdout, "Client connected with IP Address: %s\n", inet_ntoa(client.sin_addr));
	fprintf(stdout, "Client port number: %d\n", ntohs(client.sin_port));

	/* Ask client for the first value */
Loop:	w_fd = write(client_soc, "Enter First Value: ", strlen("Enter First Value: "));
	if(w_fd < 0){
		error("Cannot write to socket");
		exit(1);
	}

	/* Read first value from client */
	r_fd = read(client_soc, &value_one, sizeof(int));
	if(r_fd < 0){
		error("Cannot read data from socket");
		exit(1);
	}	
	fprintf(stdout, "First Value: %d\n", value_one);
	
	/* Ask client for the second value */
	w_fd = write(client_soc, "Enter Second Value: ", strlen("Enter Second Value: "));
	if(w_fd < 0){
		error("Cannot write to socket");
		exit(1);
	}

	/* Read second value from client */
	r_fd = read(client_soc, &value_two, sizeof(int));
	if(r_fd < 0){
		error("Cannot read data from socket");
		exit(1);
	}
	fprintf(stdout, "Second Value: %d\n", value_two);
	
	/* Ask client for operation to perform */
	w_fd = write(client_soc, "Operation to perform:\n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit ", strlen("Operation to perform:\n 1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit "));
	if(w_fd < 0){
		error("Cannot write to socket");
		exit(1);
	}	
	
	/* Read Operation to perform from client */
	r_fd = read(client_soc, &operation, sizeof(int));
	if(r_fd < 0){
		error("Cannot read data from socket");
		exit(1);
	}
	fprintf(stdout, "Client Choice: %d\n", operation);	

	/* Perform the desired operation based on client choice */
	switch(operation){
		case 1:
			result = value_one + value_two;
			break;

		case 2:
			result = value_one - value_two;
			break;

		case 3:
			result = value_one * value_two;
			break;

		case 4:
			result = value_one / value_two;
			break;

		case 5:
			goto E;
			break;
	
	}

	/* Send result back to client */
	w_fd = write(client_soc, &result, sizeof(int));
	if(w_fd < 0){
		error("Cannot write to socket");
		exit(1);
	}

	if(operation != 5){
		goto Loop;
	}

E:	close(client_soc);
	close(server_soc);
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
