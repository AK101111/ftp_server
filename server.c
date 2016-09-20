/* File for server side routine v0.1 August 10 2016
 * Author Arnav Kansal 2016
 * This library is free software.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
// fix close
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define DEBUG
#define FAIL_ERR 1
#define PARSE_INT(a) atoi(a)
         
#ifdef DEBUG
void cleanup_error(char *error)
{	
	//use something thread safe, strerror likely to crash
	//-> fix to strerror_s
	if(error)
		fprintf(stderr, "%s: %s\n", error, strerror(errno));
	else
		fprintf(stderr, "%s\n",strerror(errno));
	exit(FAIL_ERR);
}
#endif
void tokenize(char arr[], char *brr[])
{
    const char delim[2] = " ";
    char *token;
    token = strtok(arr, delim);
    short i = 0;
    while( token != NULL )
    {
        brr[i++] = token;
        token = strtok(NULL, delim);
    }
}

int exec_command(char message[], int outstream,char *argv[])
{
    //pid_t pid;
    //printf("first is %s\n",argv[0]);
    //char path1[100] = "/bin/";
    //char path2[100] = "/usr/bin/";
    //if ((pid = fork()) < 0)
       // cleanup_error("SERVER ERROR\n");
    //if(pid == 0){
        dup2(outstream, 1);
        dup2(outstream, 2);
        if(!strcmp(argv[0],"cd")){
            char current_dir[1024];
            getcwd(current_dir, sizeof(current_dir));
            if(argv[1])
                chdir(argv[1]);
            char new_dir[1024];
            getcwd(new_dir, sizeof(new_dir));
            if(strcmp(current_dir, new_dir) == 0){
                printf("No Such Directory\n");
            }
            else{
                system("pwd");
            }
            //if(argv)
                //free(argv);
            return 0;
        }else{
            system(message);
            //if(argv)
              //  free(argv);
            //printf("heh\n");
            //execv(argv[0], argv);
            //execv(strcat(path1, argv[0]), argv);
            //execv(strcat(path2, argv[0]), argv);
        }
        //fprintf(stderr, "exec %s failed\n", argv[0]);
    //}
    return FAIL_ERR;
}

int server_init(char* port_number, int *accept_socket_fd, char * ips)
{
	int socket_fd, client_sock, read_size;
	struct sockaddr_in server, client;
	if(!port_number)
		cleanup_error("NO PORT PROVIDED\n");

	socklen_t clilen;
	int port_num = PARSE_INT(port_number);
	/*AF_UNIX for unix, AF_INET for net.*/
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		cleanup_error("CANNOT OPEN SOCKET\n");
	
	//Get IP adderss of connection
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ips);
	server.sin_port = htons(port_num);

	if(bind(socket_fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
		cleanup_error("BINDING ERROR\n");

	listen(socket_fd, 4);
	
	clilen = sizeof(struct sockaddr_in);
	client_sock = accept(socket_fd, (struct sockaddr *)&client, (socklen_t*)&clilen);
	char client_msg[1024];
  	if(client_sock < 0)
 		cleanup_error("ERROR in accepting\n");
 	else{
 		bzero(client_msg, 1024);
 		while((read_size = read(client_sock, client_msg, 1024)) > 0){
            client_msg[strlen(client_msg)-1] = '\0';
            //if(client_msg[strlen(client_msg)-1] == '\n')
            char *argr[100];
            tokenize(client_msg,argr);
            exec_command(client_msg,client_sock,argr);
 		}
 	}
	return 0;
}


int start_server(int accept_fd)
{
	char buffer[256];
	char *argv[10];
	memset(buffer, 0, 256);
	int read_code;
	while(1){
		printf("hello\n");
		if((read_code = read(accept_fd,buffer,255)) < 0)
     		cleanup_error("ERROR Reading\n");
	    tokenize(buffer, argv);
	   	//exec_command(argv, accept_fd);
  	}
  	return FAIL_ERR;
}

int main(int argc, char *argv[])
{	
	if(argc < 2){
		printf("usage: ./a.out port_number ip max_threads\n");
		return 0;
	}
	int accept_fd;
	char * connection_ip = argv[2];
	if(server_init(argv[1], &accept_fd, connection_ip) < 0)
		printf("SERVER CANNOT START\n");
	//start_server(accept_fd);
	return 0;
}


/*References: http://www.binarytides.com/server-client-example-c-sockets-linux/*/