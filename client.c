#include "client.h"

int main(int argc, char* argv[]){

	char message[2000];
	char server_response[2000];
	int s_mesg, r_mesg;

	//Client set up
	int sock = open_client_socket();
	connect_to_server(sock, 8888);

	while(1){

		printf("Enter Message: ");
		fgets(message, 2000, stdin);

		//send message
		s_mesg = send(sock, message, strlen(message), 0); 
		if(s_mesg == -1){
			perror("Send Error:");
			exit(1);
		}

		//Server reply
		r_mesg = recv(sock, server_response, strlen(server_response), 0);
		if(r_mesg == -1){
			perror("Receieve Error:");
			exit(1);
		}
		printf("Server Reply:%s\n", server_response);
	}

	close(sock);

	return 0;

}
