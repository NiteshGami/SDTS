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
#include<sqlite3.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<semaphore.h>

#define NAME "SDTSPS22RG"

sem_t *mutex;
static int port = 10006;
static char *DB_Path = "./SDTS.db";

int main(int argc, char **argv) {
	int true = 1;
	int skfd, conn;
	int ret_val = -1;
	sqlite3 *DB = NULL;
	char query[512] = {0};

	if((mutex = sem_open(NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
		printf("sem_open failed\n");
	}
	ret_val = sqlite3_open(DB_Path, &DB);

	if(ret_val != SQLITE_OK) {
		printf("Not able to open database :%s\n", sqlite3_errmsg(DB));
	}
	strcpy(query, "CREATE TABLE IF NOT EXISTS device_detected_table (serial_no VARCHAR(100), mac_address VARCHAR(100)," \
					"date_time TIMESTAMP DEFAULT(datetime(current_timestamp,'localtime')), PRIMARY KEY(serial_no, mac_address));");

	ret_val = sqlite3_exec(DB, query, NULL, 0, NULL);

	if(ret_val != SQLITE_OK)
		printf("Table Creation Failed\n");

	sqlite3_close(DB);

	skfd = socket(AF_INET, SOCK_STREAM, 0);
	if (skfd < 0) {
		fprintf(stderr, "Error creating socket\n");
		return -ENOMEM;
	}

	if (setsockopt(skfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1)
		fprintf(stderr, "Error reuse\n");

	if (argc == 2) {
		port = atoi(argv[1]);
	}

	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = (in_port_t) htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(skfd, (struct sockaddr *) &name, sizeof(name)) < 0) {
		fprintf(stderr, "Error binding socket : [%s]\n", strerror(errno));
		return -EINVAL;
	}

	if (listen(skfd, 10) < 0) {
		fprintf(stderr, "Error listening socket\n");
		return -EFAULT;
	}
	printf("Waiting for connection...\n");
	while (1) {
		struct sockaddr_in client;
		unsigned int client_len = sizeof(client);

		conn = accept(skfd, (struct sockaddr *) &client, &client_len);

		if (conn < 0) {
			fprintf(stderr, "Error binding socket\n");
			return -ECONNREFUSED;
		}
		pid_t pid = -1;
		if (!(pid = fork())) {
			char *p = NULL, *mac = NULL;
			char buf[256];
			char serial[64];
			bzero(buf, sizeof(buf));
			memset(serial, 0, sizeof(serial));
			read(conn, buf, sizeof(buf));
			mac = buf;
			p = serial;
			while((*p++ =*mac++) != ';');
			*(--p) = '\0';
			ret_val = sqlite3_open(DB_Path, &DB);

			if(ret_val != SQLITE_OK) {
				printf("Not able to open database :%s\n", sqlite3_errmsg(DB));
				exit(-101);
			}
			printf("Received: %s--->%s\n",serial, mac);
			memset(query, 0, sizeof(query));
			snprintf(query, sizeof(query),"REPLACE INTO device_detected_table(serial_no, mac_address) VALUES(\"%s\",\"%s\");",serial,mac);

			if((mutex = sem_open(NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
				printf("sem_open failed\n");
			}

			if(sem_wait(mutex) < 0) {
				printf("sem_wait failed\n");
			}

			ret_val = sqlite3_exec(DB, query, NULL, 0, NULL);

			if(ret_val != SQLITE_OK) {
				printf("Insertion Failed\n");
				exit(-202);
			} else {
				printf("Insertion Successful\n");
			}

			if(sem_post(mutex) < 0) {
				printf("sem_post failed\n");
			}

			if(sem_close(mutex) < 0) {
				printf("sem_close failed\n");
			}
			sqlite3_close(DB);
			close(conn);
			close(skfd);
			exit(0);
		}  else {
			close(conn);
		}
	}

	if(sem_close(mutex) < 0) {
		printf("sem_close failed\n");
	}

	if(sem_unlink(NAME) < 0) {
		printf("sem_unlink failed\n");
	}
	close(skfd);
	return 0;
}

