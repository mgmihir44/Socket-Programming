/*
 * Author: Mihir Garude
 *
 * Date: 08/13/2018
 *
 * Description: A simple client application. This application will connect to the server
 *		and send simple messages for communication. The Server IP and Port number
 *		are passed as command line arguments.
 */

/******************************************************* Includes Section *****************************************************/
// Include all header files in this section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/******************************************************* Globals Section ******************************************************/
// All global variables, macros and function definations go here
#define BUFFER 1024

//Fucntion declarations
void error(char * message);

/******************************************************* Main *****************************************************************/
int main(int argc, char * argv[]){

	if(argc < 3){
		printf("Usage: %s ServerIP PortNo\n", argv[0]);
		exit(1);
	}

	int value_one, value_two, operation, result;	/* Holds the value to be sent, operation and result */
	char recv_buf[BUFFER];				/* Holds the received data */
	int client_soc, server_soc;
	struct sockaddr_in client;

	/* Create a TCP socket for communication */
	if((client_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		error("Cannot create socket");
		exit(1);
	}

	/* Initialize the parameters required for the connection */
	client.sin_family = AF_INET;			/* Working with IPv4*/
	client.sin_port = htons(atoi(argv[2]));	/* Connect on the Port provided by user */
	client.sin_addr.s_addr = inet_addr(argv[1]);	/* Connect to the IP provided by user */

	/* Establish a connection to the server */
	if((connect(client_soc, (struct sockaddr *) &client, sizeof(client))) < 0){
		error("Cannot establish connection");
		exit(1);
	}

	fprintf(stdout, "Connected to Server with IP: %s\n", argv[1]);
	fprintf(stdout, "Connected on Port No: %d\n", atoi(argv[2]));

	/* Receive data from the server */
Loop:	if((server_soc = recv(client_soc, recv_buf, BUFFER, 0)) <  0){
		error("Cannot receive data from client");
		exit(1);
	}
	recv_buf[server_soc] = '\0';
	fprintf(stdout, "%s", recv_buf);

	scanf("%d", &value_one);		/* First Value to send to server */
		
	/* Send data to the server */
	if((send(client_soc, &value_one, sizeof(int), 0)) < 0){
		error("Cannot send data to server");
		exit(1);
	}

	/* Receive data from the server */
	if((server_soc = recv(client_soc, recv_buf, BUFFER, 0)) <  0){
		error("Cannot receive data from client");
		exit(1);
	}
	recv_buf[server_soc] = '\0';
	fprintf(stdout, "%s", recv_buf);

	scanf("%d", &value_two);		/* Second Value to send to server */

	/* Send data to the server */
	if((send(client_soc, &value_two, sizeof(int), 0)) < 0){
		error("Cannot send data to server");
		exit(1);
	}

	/* Receive data from the server */
	if((server_soc = recv(client_soc, recv_buf, BUFFER, 0)) <  0){
		error("Cannot receive data from client");
		exit(1);
	}
	recv_buf[server_soc] = '\0';
	fprintf(stdout, "%s\n", recv_buf);

	scanf("%d", &operation);		/* Operation to be performed by the server */

	if(operation == 5){
		goto E;
	}

	/* Send it to the server */
	if((send(client_soc, &operation, sizeof(int), 0)) < 0){
		error("Cannot send data to server");
		exit(1);
	}
	
	/* Receive result from the server */
	if((server_soc = recv(client_soc, &result, sizeof(int), 0)) <  0){
		error("Cannot receive data from client");
		exit(1);
	}
	fprintf(stdout, "The result is : %d\n", result);

	if(operation != 5){
		goto Loop;
	}

	/* Exit if server wants to terminat */
E:	printf("Exiting the application\n");
	close(server_soc);
	close(client_soc);
	
	return 0;
}

/*
 * Description: Provide a descriptive message to the user and terminate the program
 *
 * Input: A string of characters specifing the message to display
 *
 * Output: None
 */
void error(char * message){

	perror(message);
	printf("\n");
	exit(1);

}
