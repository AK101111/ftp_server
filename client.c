//FTP Client

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int tokenize(char *cmd, char tokens[2][256]){
     char *token;
     int NUM_TOKENS = 0;
     token = strtok(cmd, " ");

    while(token != NULL){
         strcpy(tokens[NUM_TOKENS],token);
         token = strtok(NULL, " ");
         //Removing the last \n from arguments
         if (token == NULL){
         	strtok(tokens[NUM_TOKENS], "\n");
         }
         NUM_TOKENS++;
     }
     return NUM_TOKENS;
}

void start_interface(){
	char arg[1024];
	char tokens[2][256];
	while (1){
		printf("Command My lord : ");
		bzero(arg, sizeof(arg));
		fgets(arg, sizeof(arg), stdin);
		tokenize(arg, tokens);
		if (strcmp(tokens[0], "lls") == 0){
			system("ls");
		}
		else if (strcmp(tokens[0], "lcd") == 0){
			if (tokens[1] == NULL){
				printf("Not enough arguments\n");
			}
			else{
				char current_dir[1024];
				getcwd(current_dir, sizeof(current_dir));
				chdir(tokens[1]);
				char new_dir[1024];
				getcwd(new_dir, sizeof(new_dir));
				if(strcmp(current_dir, new_dir) == 0){
					printf("No Such Directory\n");
				}
				else{
					system("pwd");
				}
			}
		}
		else if (strcmp(arg, "lchmod")){
			system()
		}
		else if (strcmp(arg, "ls")){

		}
		else if (strcmp(arg, "cd")){

		}
		else if (strcmp(arg, "chmod")){

		}
		else if (strcmp(arg, "get")){

		}
		else if (strcmp(arg, "put")){

		}
		else if (strcmp(arg, "close")){

		}
		else{

		}
	}
}

void start_socket(int sktp){
	int sock, socket_port = sktp;
	struct sockaddr_in serveraddr;
	//Socket Creation
	sock= socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printf("Socket Creation Failed");
		exit(0);
	}
	else{
		printf("Socket Created");
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(socket_port);
	//Attempting to connect The socket
	if(connect(sock,(struct sockaddr *)&servaddr, sizeof(servaddr))<0){
		printf("Connection Error\n");
		exit(1);
	}
	else{
		printf("Connected\n");
		start_interface();
	}
}

int main(int argc, char** argv){
	
	int socket_port;
	//Port No.
	printf("Type the socket port : ");
	scanf("%d", &socket_port);
	start_socket(socket_port);
}

//Refrence : http://mcalabprogram.blogspot.in/2012/01/ftp-sockets-server-client-using-c.html
//http://stackoverflow.com/questions/16841365/why-is-chdir-not-successful