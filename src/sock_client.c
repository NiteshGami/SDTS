/*
 * sock_client.c
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */
#include "sock_client.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int sock_send(const char *address, int port, const char *message) {
	int sock_fd = 0, connected = 0, written = 0;
	int true = 1;
	struct sockaddr_in server;
	char buff[256];
	int len_buf = -1;

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "Could not open socket");
		return -EOPNOTSUPP;
	}

	if (setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &true, sizeof(int)) == -1) {
		fprintf(stderr, "Error reuse\n");
	}

	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(address);
	server.sin_port = htons(port);

	connected = connect(sock_fd, (struct sockaddr *)&server, sizeof(server));

	if (connected < 0) {
		fprintf(stderr, "Could not connect");
		return -ECONNREFUSED;
	}

	bzero(buff, sizeof(buff));
	snprintf(buff, sizeof(buff), "%s", message);
	len_buf = strlen(buff);
	printf("*************%s(%d)*****************\n",buff,len_buf);
	while(len_buf != written) {
		written += write(connected, buff+written, strlen(buff));
		printf("writeen :%d\n");
	}

	close(connected);
	close(sock_fd);
	return 0;
}

struct client_t client = {
	.send = sock_send,
};
