#include "client.h"

//Create client socket
int open_client_socket(){

	int c_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(c_sock == -1){
		perror("Client Socket Error:");
		exit(1);
	}
	puts("Client socket created");

	return c_sock;
}

//Connection to server
void connect_to_server(int socket, int port){

	//Initiate a connection
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	client.sin_port = htons(8888);

	if(connect(socket, (struct sockaddr *) &client, sizeof(client)) < 0){
		perror("Connection Error:");
		exit(1);
	}
	puts("Connected to server");

}
