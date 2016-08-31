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

int server_init(char* port_number, int *accept_socket_fd)
{
	int socket_fd;
	struct sockaddr_in *server, *client;
	if(!port_number)
		cleanup_error("NO PORT PROVIDED\n");

	if(!socket_fd)
		cleanup_error("NO SOCKET AVAILABLE\n");

	socklen_t clilen;
	int port_num = PARSE_INT(port_number);
	/*AF_UNIX for unix, AF_INET for net.*/
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		cleanup_error("CANNOT OPEN SOCKET\n");
	
	server = (struct sockaddr_in*)calloc(1, sizeof(struct sockaddr_in));
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	/* random c stuff, we actually have to change b/w network and 
		 host byte order, was so much easier in JAVA !!*/
	server->sin_port = htons(port_num);
	
	if(bind(socket_fd, server, sizeof(struct sockaddr_in)) < 0)
		cleanup_error("BINDING ERROR\n");

	listen(socket_fd,4);
	client = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	clilen = sizeof(client);

  if((accept_socket_fd = accept(socket_fd, 
                 								client, 
                 								&clilen)) < 0)
 		cleanup_error("ERROR in accepting\n");
	return 0;
}

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

int start_server(int accept_fd)
{
	char buffer[256];
	char *argv[10];
	memset(buffer, 0, 256);
	int read_code;
	while(1){
		if((read_code = read(accept_fd,buffer,255)) < 0)
     	cleanup_error("ERROR Reading\n");
    tokenize(buffer, argv);
   	exec_command(argv, accept_fd);
  }
  return FAIL_ERR;
}

int exec_command(char *argv[], int outstream)
{
	pid_t pid;
	char path1[100] = "/bin/";
	char path2[100] = "/usr/bin/";
	if ((pid = fork()) < 0)
		cleanup_error("SERVER ERROR\n");
	if(pid == 0){
		dup2(outstream, 1);
		dup2(outstream, 2);
		execv(argv[0], argv);
    execv(strcat(path1, argv[0]), argv);
   	execv(strcat(path2, argv[0]), argv);
    fprintf(stderr, "exec %s failed\n", argv[0]);
	}
	return FAIL_ERR;
}

int main(int argc, char *argv[])
{	
	if(argc < 2){
		printf("usage: ./a.out port_number max_threads\n");
		return 0;
	}
	int accept_fd;
	if(server_init(argv[1], &accept_fd) < 0)
		printf("%SERVER CANNOT START\n");
	start_server(accept_fd);
	return 0;
}