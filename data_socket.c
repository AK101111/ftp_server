#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int send_file(char filename[], int socket){
	int client_fd = accept(socket, (struct sockaddr*)NULL, NULL);

	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL){
		printf("Unable to open your file\n");
		return -1;
	}
	char buff[256] = {0};
	int read;
	while (1){
		read = fread(buff, 1, 256, file);
		if (read > 0){
			send(client_fd, read, strlen(read), 0);
		}
		if (read < 256){
			if (feof(file)){
				printf("End of File\n");
			}
			return 1;
			break;
		}
	}
	close(client_fd);
	return 0;
}

int receive_file(char filename[], int sock){
	FILE *file;
	file = fopen(filename,"w+");
	if(file ==  NULL){
		printf("Unable to open your file\n");
		return -1;
	}
	char buff[256] = {0};
	//memset(buff,'0',sizeof(buff));
	while (1){
		int bytesreceived = read(sock, buff, 256);
		while(bytesreceived > 0){
			printf("Bytes received %d\n", bytesreceived);
			fwrite(buff, 1, bytesreceived, file);
		}
		if(bytesreceived < 0){
			printf("Read Error\n");
			break;
		}
	}
	return 1;
}

//flag = 0 : send file; = 1 : receive file
int start_data_socket(int socket_port, char filename[], char * ip, int flag){
		int sock, client_fd;
		struct sockaddr_in server_addr;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0){
			printf("Data Socket Creation Failed\n");
			exit(0);
		}
		else{
			printf("Data Socket Created\n");
		}
		server_addr.sin_family = AF_INET;
		printf("%s\n", ip);
		server_addr.sin_addr.s_addr = inet_addr(ip);
		server_addr.sin_port = htons(socket_port);

		//Attempting to connect The socket
		if(connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr))<0){
			printf("Connection Error\n");
			return -1;
		}
		else{
			printf("Connected\n");
			if(flag == 0)
				send_file(filename, sock);
			else
				receive_file(filename, sock);
		}
}

/*int main(){
	return 0;
}*/

/*References : http://codereview.stackexchange.com/questions/43914/client-server-implementation-in-c-sending-data-files*/