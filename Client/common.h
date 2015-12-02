/*
 * common.h
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * All type of actions from the client goes here
 */

typedef enum {
	NONE,
	SEND,
} action_e;

typedef void(*usb_detected_cb)(action_e, void *);

#endif /* COMMON_H_ */
