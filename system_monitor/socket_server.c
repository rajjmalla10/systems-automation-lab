#include "socket_header.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


int init_socket_server(){
	struct sockaddr_un sock_addr;
	
	int socket_fd = socket(AF_UNIX,SOCK_STREAM,0);
	if(socket_fd == -1){
		perror("socket creation failed");
		return -1;
		}
	unlink(SOCKET_PATH);
	// set up the address 
	
	memset(&sock_addr,0,sizeof(struct sockaddr_un));
	sock_addr.sun_family = AF_UNIX;
	strncpy(sock_addr.sun_path,SOCKET_PATH,sizeof(sock_addr.sun_path)-1);
	
	// bind the socket to the path 
	
	if(bind(socket_fd, (struct sockaddr*)&sock_addr,sizeof(struct sockaddr_un)) == -1){
		perror("Bind failed");
        close(socket_fd);
        return -1;
		}
	//5. start listening 
	
	if(listen(socket_fd,1) == -1){
		perror("Listen failed");
        close(socket_fd);
        return -1;
		}	
	return socket_fd;
	}

int wait_for_client(int socket_fd){
	int client_fd = accept(socket_fd,NULL,NULL);
	if(client_fd == -1){
		perror("Accept failed");
        return -1;
		}
	printf("Connection established!\n");	
	return client_fd;	
	}

void send_data(int client_fd,const char *data){
	size_t len = strlen(data);
	ssize_t bytes_send = send(client_fd,data,len,MSG_NOSIGNAL);
	
	if(bytes_send == -1){
		perror("send failed (python disconnected)!!\n");
		}
	}	
	

