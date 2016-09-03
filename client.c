//FTP Client

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN 512

int tokenize(char *cmd, char tokens[10][256]){
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

void start_interface(int sock){
	char arg[1024];
	char tokens[10][256];
	int num_tokens;
	while (1){
		printf("Command My lord : ");
		bzero(arg, sizeof(arg));
		fgets(arg, sizeof(arg), stdin);
		num_tokens = tokenize(arg, tokens);
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
		else if (strcmp(tokens[0], "lchmod") == 0){
			char s[1024] = "chmod";
			int iter = 1;
			char *tkns;
			tkns = tokens[iter];
			//collecting all arguments to one string s n then running system command
			while(iter < num_tokens){
				strcat(s, " ");
				strcat(s, tkns);
				iter++;
				tkns = tokens[iter];
			}
			system(s);
		}
		else if (strcmp(tokens[0], "ls") == 0 || strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "chmod") == 0){
			//Send the command to control port
			if( send(sock , arg , strlen(arg) , 0) < 0)
	        {
	            puts("Send failed");
	            return;
	        }
	        //Receive reply from the server
	        char reply[1024];
	        if( recv(sock , reply , 1024 , 0) < 0)
	        {
	            puts("recv failed");
	            break;
	        }
	        else{
	        	printf("%s\n", reply);
	        }
		}
		else if (strcmp(tokens[0], "get")){
			
		}
		else if (strcmp(tokens[0], "put")){

		}
		else if (strcmp(tokens[0], "close")){
			close(sock);
			printf("Socket closed. Connection terminated\n");
		}
		else{
			system(arg);
		}
	}
}

void start_control_socket(int socket_port, char * ip){
	int sock;
	struct sockaddr_in servaddr;
	//Socket Creation
	sock= socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printf("Socket Creation Failed\n");
		exit(0);
	}
	else{
		printf("Socket Created\n");
	}
	servaddr.sin_family = AF_INET;
	printf("%s\n", ip);
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(socket_port);

	//Attempting to connect The socket
	if(connect(sock,(struct sockaddr *)&servaddr, sizeof(servaddr))<0){
		printf("Connection Error\n");
		exit(1);
	}
	else{
		printf("Connected\n");
		start_interface(sock);
	}
}

int main(int argc, char** argv){
	
	if(argc < 2){
		printf("provide port no. followed by ip\n");
		return -1;
	}
	int data_socket_port;
	int control_socket_port;
	char * con_ip = argv[2];
	//Port No.
	control_socket_port = atoi(argv[1]);

	/*printf("Type the control socket port number : ");
	scanf("%d", &control_socket_port);*/
	/*printf("Type the data socket port number : ");
	scanf("%d", &data_socket_port);*/
	start_control_socket(control_socket_port, con_ip);
}

//Refrence : http://mcalabprogram.blogspot.in/2012/01/ftp-sockets-server-client-using-c.html
//http://stackoverflow.com/questions/16841365/why-is-chdir-not-successful