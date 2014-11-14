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
	errno = 0;

	printf("****************(%s,%s)**********\n",command, file_path);
	/*while (args[index] != NULL) {
		printf("%s", args[index]);
		index++;
	}*/

	if (!(pid = fork())) {
		printf("pid(%d), ppid (%d)", getpid(), getppid());
		printf("Inside child, exec (%s) command\n", file_path);

		errno = 0;
		if (execlp(command, command, file_path, NULL) == -1) {
			printf("Fail to execute command (%s)\n", strerror(errno));
			exit(-1);
		}
		exit(0);
	} else if (pid > 0) {
		if (waitpid(pid, &status, 0) == -1) {
			printf("wait pid (%u) status (%d)\n", pid, status);
		}
		printf("Waiting for child\n");
		if(status == -1) {
			printf("execlp failed\n");
		} else if(status == 0) {
			printf("execlp success status: %d\n",status);
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

	execute_script("perl", "get_info.pl");
	sleep(3);
	file = fopen("final.info", "r");
	if (file <= 0) {
		printf("Error openning");
	}
	printf("opened file\n");
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
	if(ret) {
		printf("not able to delete\n");
	} else {
		printf("file deleted");
	}
	return head;
}

static void take_action(action_e act, void *data) {
	node_t *head = NULL;
	node_t *trav = NULL, *t;
	printf("Take Action called\n");
	sleep(3);
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
	//printf("%s", mac);
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
