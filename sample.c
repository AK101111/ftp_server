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
	}
}

int main(){
	int pr;
	int i;
	start_interface();
	return 0;
}