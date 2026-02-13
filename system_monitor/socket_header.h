#ifndef SOCKET_HEADER_H
#define SOCKET_HEADER_H

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SOCKET_PATH "/tmp/proc_mon.sock"
#define BUFFER_SIZE 65536 // 65kb for our snapshot

int init_socket_server();
int wait_for_client(int server_fd);
void send_data(int client_fd,const char *data);

#endif
