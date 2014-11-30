/*
 * sock_client.h
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */

#ifndef SOCK_CLIENT_H_
#define SOCK_CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

struct client_t {
	int (*send)(const char *, int, const char *);
};
extern struct client_t client;
#endif /* SOCK_CLIENT_H_ */
