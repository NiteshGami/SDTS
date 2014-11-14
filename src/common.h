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
typedef enum {
	NONE,
	SEND,
} action_e;
typedef void(*usb_detected_cb)(action_e, void *);

#endif /* COMMON_H_ */
