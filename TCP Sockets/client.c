#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char * argv[]){


	/* Create a TCP Socket for IPv4 addresses*/
	int c_socket = socket(AF_INET, SOCK_STREAM, 0);


	/* Set parameters to make the connection */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;			/* Working with IPv4 */
	server_addr.sin_port = htons(9002);			/* Using port no. 9002 */
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* Server Address -> 0.0.0.0 */

	/* Set up the connection */
	int status = connect(c_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(status == -1){
		fprintf(stdout, "Connection cannot be made. Please try again\n");
	}

	/* Receive Data from the server */
	char s_data[512];						/* Store data received from server */
	recv(c_socket, &s_data, sizeof(s_data), 0);
	

	/* Print the data received from the server*/
	printf("The server send: %s\n", s_data);

	/* Close the Connection */
	close(c_socket);

	return 0;
}
