#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void start_interface(){
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
			//s = strcat(tokens[0], tokens);
			//printf("%s\n", s);
			int iter = 1;
			char *tkns;
			tkns = tokens[iter];
			while(iter < num_tokens){
				strcat(s, " ");
				strcat(s, tkns);
				iter++;
				tkns = tokens[iter];
			}
			system(s);
		}
		else{
			system(arg);
		}
	}
}

int main(){
	int pr;
	int i;
	start_interface();
	return 0;
}