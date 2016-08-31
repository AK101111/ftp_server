/* File for server side routine v0.1 August 10 2016
 * Author Arnav Kansal 2016
 * This library is free software.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include <errno.h>

#define DEBUG
#define FAIL_ERR 1

int init_server(char *port_num)
{
	int port_num, socket_fd;
	struct sockaddr_in *server, *client;
	port_num = parse_to_int(port_num);
	
	/*AF_UNIX for unix, AF_INET for net.*/
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		cleanup_error("CANT OPEN SOCKET\n");
	server = (sockaddr_in*)calloc(1, sizeof(sockaddr_in));

	server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	/* random c stuff, we actually have to change bw network and host byte order, was so much easier in JAVA !!*/
	server->sin_port = htons(port_num);
	
	if(bind(socket_fd, server, sizeof(sockaddr_in)) < 0)
		cleanup_error("ERROR in SERVER\n");
	return 0;
}

#ifdef DEBUG
void cleanup_error(char *error)
{	
	//use something thread safe, strerror likely to crash
	if(error)
		fprintf(stderr, "%s: %s\n", error, strerror(errno));
	else
		fprintf(stderr, "%s\n",strerror(errno));
	exit(FAIL_ERR);
}
#endif
