#include "server.h"

int main(int argc, char* argv[]){

	int connect_d, *new_sock;

	//Set up server
	int listner_d = open_listner_socket();
	bind_to_port(listner_d, 8888);
	if(listen(listner_d, 10) == -1){
		perror("Listener Error:");
		exit(1);
	}

	struct sockaddr_in client_addr;
	unsigned int addr_size = sizeof(client_addr);

	while(1){
		connect_d = accept(listner_d, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
		if(connect_d == -1){
			perror("Connection Error:");
			exit(1);
		}
		puts("Connection Accepted");

		//Create thread for handling communication
		pthread_t comm_thread;
		new_sock = malloc(1);
		*new_sock = connect_d;

		if(pthread_create(&comm_thread, NULL, connection_handler, (void *) new_sock)){
			perror("Thread Error:");
			exit(1);
		}

		//Wait for commuincation to complete
//		pthread_join(comm_thread, NULL);

	}

	if(connect_d == -1){
		perror("Accept Error:");
		exit(1);
	}
	
	free(new_sock);

	return 0;
}
