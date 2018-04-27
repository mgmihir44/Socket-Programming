#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char * argv[]){

	/* Data to send to any client that connect to us */
	char s_data[512] = "You have successfully connected to the serever. Welcome!";

	/* Create a TCP socket fpr IPv4 Addresses */
	int s_socket = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize parameters to set the connection */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;			/* Working with IPv4 Addresses */
	server_addr.sin_port = htons(9002);			/* Initialize server on port 9002 */
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* Adress in -> 0.0.0.0 */

	if(setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0){
    		printf("setsockopt(SO_REUSEADDR) failed");	
	}

	/* Bind the server to the IP address and port initialized*/
	bind(s_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

	/* Start Listening for connections from clients */
	listen(s_socket, 10);				/* Keep 10 clients backlogged */

	/* Accept connections from clients */
	int c_socket = accept(s_socket, NULL, NULL);		/* No need to verify client info as it runs on our local machine */
	
	/* Send data to Client */
	send(c_socket, s_data, sizeof(s_data), 0);

	/* Close the connection */
	close(s_socket);

	return 0;
}
