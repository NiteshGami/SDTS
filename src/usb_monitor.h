/*
 * usb_monitor.h
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */

#ifndef USB_MONITOR_H_
#define USB_MONITOR_H_

#include <libudev.h>
#include <locale.h>
#include <string.h>
#include "common.h"

int usb_monitor_init(usb_detected_cb detected, void *data);

#endif /* USB_MONITOR_H_ */
