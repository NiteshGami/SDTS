/*
 ============================================================================
 Name        : targetinfo.c
 Author      : m3ghd007
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <net/if.h>
#include "common.h"
#include "usb_monitor.h"
#include "sock_client.h"
typedef struct n {
	char *data;
	struct n *next;
} node_t;
static char address[32] = "127.0.0.1";
static int port = 10006;
static char mac[32] = {'\0'};
void get_mac() {
	FILE *file;
	char line[256];
	file = fopen("/sys/class/net/eth0/address", "r");
	if (file <= 0) {
		printf("Error openning");
		return;
	}
	fscanf(file, "%s", line);
	strcpy(mac, line);
	fclose(file);
}

int execute_script(const char * command, const char *file_path)
{
	pid_t pid = 0;
	int status = -1;

	if (!(pid = fork())) {

		if (execlp(command, command, file_path, NULL) == -1) {
			printf("Fail to execute command (%s)\n", strerror(errno));
			exit(-1);
		}
		exit(0);
	} else if (pid > 0) {
		if (waitpid(pid, &status, 0) == -1) {
			printf("wait pid (%u) status (%d)\n", pid, status);
		}

		if(status == -1) {
			printf("execlp failed\n");
		}
		return status;
	}

	printf("failed to fork(%s)\n", strerror(errno));
	return -EIO;
}

node_t *get_target_info() {
	FILE *file;
	char line[256];
	char *data;
	node_t *head = NULL, *temp = NULL;
	int ret = -1;

	execute_script("perl", "dev.pl");
	sleep(3);
	file = fopen("final.info", "r");
	if (file <= 0) {
		printf("Error openning");
	}
	bzero(line, sizeof(line));
	while(fscanf(file, "%s", line) > 0) {
		printf("line = %s\n", line);
		data = (char *)malloc(strlen(line) * sizeof(char));
		bzero(data, strlen(line) * sizeof(char));
		strcpy(data, line);
		temp = (node_t *) malloc(sizeof(node_t));
		temp->data = data;
		temp->next = head;
		head = temp;
	};
	fclose(file);
	ret = remove("final.info");
	return head;
}

static void take_action(action_e act, void *data) {
	node_t *head = NULL;
	node_t *trav = NULL, *t;
	head = get_target_info();
	trav = head;
	while(trav) {
		char messge[256];
		bzero(messge, sizeof(messge));
		sprintf(messge, "%s;%s", trav->data, mac);
		printf("Sending %s\n", messge);
		client.send(address, port, messge);
		t = trav;
		trav = trav->next;
		free(t->data);
		free(t);
		t = NULL;
	}
}

int main(int argc, char *argv[]) {
	get_mac();

	if (argc == 3) {
		bzero(address, sizeof(address));
		strncpy(address, argv[1], 31 * sizeof(char));
		port = atoi(argv[2]);
	} else {
		printf("Setting server address as [%s] and port as [%d]", address, port);
	}

	usb_monitor_init(take_action, "Hi Folks");
	return EXIT_SUCCESS;
}
