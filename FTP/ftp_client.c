/*
 * Author: Mihir Garude
 *
 * Date: 08/16/2018
 *
 * Description: This is an FTP Client. Sends a text file to the server.
 */

/**************************************************************** Includes Section ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/*************************************************************** Globals Section ************************************************/

#define BUFFER 2048

//Function Declarations
void error(const char * message);

/*************************************************************** Main ***********************************************************/
int main(int argc, char * argv[]){

	if(argc < 3){
		fprintf(stdout, "Usage: %s <IP Address> <Port No>", argv[0]);
		exit(1);
	}
	
	char buffer[BUFFER];
	struct sockaddr_in client;
	int client_soc, server_soc, w_fd;
	int words = 0;
	char ch, ch_one;

	/* Create a socket for communication */
	if((client_soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		error("Cannot create a socket");
	}

	/* Initialize the parameters required to establish the connection */
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[2]));
	client.sin_addr.s_addr = inet_addr(argv[1]);

	/* Establish a connection to the server */
	if((connect(client_soc, (struct sockaddr*) & client, sizeof(client))) < 0){
		error("Cannot establish connection");
	}

	fprintf(stdout, "Connected to remote machine with IP %s successful\n", argv[1]);

	/* Open a file to send over the connection */
	FILE * f = fopen("dummy.txt", "r");
	if(f == NULL){
		error("Cannot read file");
	}

	/* Read the file character by character */
	while((ch = fgetc(f)) != EOF){
		fscanf(f, "%s", buffer);
		if(isspace(ch) || ch == '\t'){
			words++;
		}
	}

	/* Send word count to the remote machine */
	w_fd = write(client_soc, &words, sizeof(int));
	if(w_fd < 0){
		error("Cannot send word count to remote machine");
	}

	/* Return to the beginning of the file */
	rewind(f);

	/* Send the entire file to the remote machine*/
	while(ch_one != EOF){
		fscanf(f, "%s", buffer);
		w_fd = write(client_soc, buffer, 1024);
		if(w_fd < 0){
			error("Cannot send file to remote machine");
		}
		ch = fgetc(f);
	}
	
	fprintf(stdout, "File transfer successful\n");
	
	return 0;
}

/*
 * Description: Print a descriptive error message
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
