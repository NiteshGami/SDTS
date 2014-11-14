/*
 * server.c
 *
 *  Created on: Jul 28, 2014
 *      Author: s.sikder
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int true = 1;
	int skfd, conn;
	int bytes_read =  0;
	int ret_val = -1;
	pid_t c_pid = -1;
	skfd = socket(PF_INET, SOCK_STREAM, 0);
	if (skfd < 0) {
		fprintf(stderr, "Error creating socket\n");
		return -ENOMEM;
	}
	printf("Sock created\n");
	if (setsockopt(skfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1) {
		fprintf(stderr, "Error reuse\n");
	}

	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t) htons(10006);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("Bind Set\n");

	if (bind(skfd, (struct sockaddr *) &name, sizeof(name)) < 0) {
		fprintf(stderr, "Error binding socket : [%s]\n", strerror(errno));
		return -EINVAL;
	}

	printf("Bound\n");

	if (listen(skfd, 10) < 0) {
		fprintf(stderr, "Error listening socket\n");
		return -EFAULT;
	}

	printf("Listening\n");
	while (1) {
		struct sockaddr_in client;
		unsigned int client_len = sizeof(client);
		char buf[256];
		printf("Waiting for connection\n");
		conn = accept(skfd, (struct sockaddr *) &client, &client_len);

		if (conn < 0) {
			fprintf(stderr, "Error binding socket\n");
			return -ECONNREFUSED;
		} else {
			printf("Got connection from %s(%ud)\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		}
		if (1) {
			printf("inside\n");
			bzero(buf, sizeof(buf));
			while((ret_val = read(conn, buf+bytes_read, sizeof(buf)-bytes_read)) > 0) {
				bytes_read += ret_val;
				printf("inside\n");
			}
			//TODO enter DB;
			printf("Printing %s\n", buf);
			close(conn);
			close(skfd);
			return 0;
		} else {
			printf("parent\n");
			close(conn);
		}
	}

	close(skfd);
	return 0;
}

