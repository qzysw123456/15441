#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "parse.h"

#define ECHO_PORT 9999
#define BUF_SIZE 4096

int close_socket(int sock)
{
	if (close(sock)) {
		fprintf(stderr, "Failed closing socket.\n");
		return 1;
	}
	return 0;
}
int open_socket()
{
	int sock;
	struct sockaddr_in addr;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Failed creating socket.\n");
		return EXIT_FAILURE;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ECHO_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *) &addr, sizeof(addr))) {
		close_socket(sock);
		fprintf(stderr, "Failed binding socket.\n");
		return EXIT_FAILURE;
	}
	
	if (listen(sock, 10)) {
		close_socket(sock);
		fprintf(stderr, "Error listening on socket.\n");
		return EXIT_FAILURE;
	}
	
	return sock;
}
const char* BAD_REQUEST_RESPONSE = "HTTP/1.1 400 Bad Request\r\n";
int main(int argc, char* argv[])
{
	int client_sock;
	ssize_t readret;
	socklen_t cli_size;
	struct sockaddr_in cli_addr;
	char buf[BUF_SIZE];

	fprintf(stdout, "----- Echo -----\n");
	
	int sock = open_socket();
	
	fd_set master;
	fd_set read_fds;
	int fdmax;
	
	fdmax = sock;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(sock, &master);	
	
	while("server forever")
	{
		read_fds = master;
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)	{
			fprintf(stderr, "fail to select");
			exit(4);
		}
		for(int i = 0; i <= fdmax; i++) {
			if(FD_ISSET(i, &read_fds)) {
				if(i == sock) {
					cli_size = sizeof cli_addr;
					client_sock = accept(sock, (struct sockaddr* )&cli_addr, &cli_size);	
					FD_SET(client_sock, &master);
					if(client_sock > fdmax)
						fdmax = client_sock;
				}
				else {
					readret = recv(i, buf, sizeof buf, 0);
					Request* t = parse(buf, readret, i);
					if(t != NULL)
						send(i, buf, readret, 0);
					else
						send(i, BAD_REQUEST_RESPONSE, strlen(BAD_REQUEST_RESPONSE), 0);
				}
			}
		}	
	}
	close_socket(sock);

	return 0;
}
