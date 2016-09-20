#ifndef DATASOCK_
#define DATASOCK_

int send_file(char filename[], int socket);
int receive_file(char filename[], int sock);
int start_data_socket(int socket_port, char * ip);

#endif