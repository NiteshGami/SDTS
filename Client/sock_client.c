/*
 * sock_client.c
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */
#include "common.h"
#include "sock_client.h"
int sock_send(const char *address, int port, const char *message) {
	int sock_fd = 0, connected = 0;
	struct sockaddr_in server;
	char buff[256];

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		fprintf(stderr, "Could not open socket");
		return -EOPNOTSUPP;
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
	write(sock_fd, buff, strlen(buff));

	close(sock_fd);
	return 0;
}

struct client_t client = {
	.send = sock_send,
};
