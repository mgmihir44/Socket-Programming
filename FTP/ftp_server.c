/*
 * Author: Mihir Garude
 *
 * Date: 08/16/2018
 *
 * Description: This is an FTP server. Accepts a text file from the client.
 */

/************************************************************* Includes Section *******************************************************/
// All include files go in this section
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/************************************************************* Globals Section *********************************************************/
//All global variables, macros and function defination go here
#define BUFFER 1024

//Function Declarations
void error(const char * message);		/* A function to display an error message to the user */

/************************************************************* Main Section ************************************************************/
int main(int argc, char * argv[]){

	if(argc < 2){
		fprintf(stdout, "Usage: %s <Port No.>\n", argv[0]);
	}

	struct sockaddr_in server, client;
	socklen_t client_l;
	int server_soc, client_soc, w_fd;
	int ch = 0;
	int words;
	char buffer[BUFFER];

	/* Create a server socket */
	if((server_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		error("Cannot create socket");
	}

	/* Initialize the parameters requried to establish the connection */
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;

	/* Make the socket reuseable */
	if((setsockopt(server_soc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))) < 0){
		error("Cannot reuse socket");
	}

	/* Bind the server */
	if((bind(server_soc, (struct sockaddr *) &server, sizeof(server))) < 0){
		error("Cannot bind server");
	}

	/* Listen for connections */
	if(listen(server_soc, 5) < 0){
		error("Cannot listen for connections");
	}

	fprintf(stdout, "Server started on port no: %d\n", atoi(argv[1]));

	/* Accept connection from client */
	client_soc = accept(server_soc, (struct sockaddr *)&client, &client_l);
	if(client_soc < 0){
		error("Cannot accept connections");
	}

	/* Display connection details */
	fprintf(stdout, "Remote connection with IP Address %s\n", inet_ntoa(client.sin_addr));
	fprintf(stdout, "Remote connection on Port No. %d\n", ntohs(client.sin_port));

	/* Open the file for writing */
	FILE * f = fopen("received_file.txt", "a");

	/* Read the amount of words sent by the client */
	w_fd = read(client_soc, &words, sizeof(int));
	if(w_fd < 0){
		error("Cannot read amount of data from client");
	}

	/* Perform a write to the opened file */
	while(ch != words){
		w_fd = read(client_soc, buffer, 1024);
		if(w_fd < 0){
			error("Cannot read data from stream");
		}
		fprintf(f, "%s ", buffer);
		ch++;
	}

	fprintf(stdout, "File received successfully\n");
	return 0;
}

/*
 * Description: Provide a descriptive error message to the user
 *
 * Input: A message to display
 *
 * Output: None
 */
void error(const char * message){

	perror(message);
	printf("\n");
	exit(1);

}
