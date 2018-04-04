#include "server.h"


//Create a socket
int open_listner_socket(){

	//Open TCP socket for communication
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1){
		perror("Socker Error: ");
		exit(1);
	}
	puts("Successfully opened socket");

	return s;

}


//Bind to a port
void bind_to_port(int socket, int port){

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	//Make port resuable
	int reuse = 1; 
	if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1){
		perror("Reuse Error");
		exit(1);
	}
	
	//Bind to port
	int b = bind(socket, (struct sockaddr *)&server, sizeof(server));
	if(b == -1){
		perror("Bind Error");
		exit(1);
	}
	puts("Bind done");

}

int response(int socket, char * s){

	//Send Data
	int result = send(socket, s, strlen(s), 0);
	if(result == -1){
		perror("Send Error:");
		exit(1);
	}

	return result;

}


//Connection handler thread
void * connection_handler(void * socket_desc){

	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char  client_message[2000];
	char  send_message[2000];

	while(1){
		read_size = recv(sock, client_message, strlen(client_message), 0);
		if(read_size >= 0){
			printf("Client message:%s\n", client_message);
			printf("To Client:");
			fgets(send_message, 2000, stdin);
			response(sock, send_message);	
		}
		else{
			perror("Receive Error:");
			exit(1);
			
		}
		sleep(5);

	}

	if(read_size == -1){
		perror("Recv Error:");
		exit(1);
	}

	return 0;
}
