/*
 * usb_monitor.c
 *
 *  Created on: Nov 10, 2014
 *      Author: s.sikder
 */
#include "common.h"
#include "usb_monitor.h"
#include <stdio.h>
#include <errno.h>

int usb_monitor_init(usb_detected_cb detected, void *data) {
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;

	struct udev_monitor *mon;
	int fd;

	/* Create the udev object */
	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		return -ENOMEM;
	}

	/* Set up a monitor to monitor hidraw devices */
	mon = udev_monitor_new_from_netlink(udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", NULL);
	udev_monitor_enable_receiving(mon);
	/* Get the file descriptor (fd) for the monitor.
	   This fd will get passed to select() */
	fd = udev_monitor_get_fd(mon);


	/* Create a list of the devices in the 'hidraw' subsystem. */
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path;

		/* Get the filename of the /sys entry for the device
		   and create a udev_device object (dev) representing it */
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		/* usb_device_get_devnode() returns the path to the device node
		   itself in /dev. */
		//printf("Device Node Path: %s\n", udev_device_get_devnode(dev));

		/* The device pointed to by dev contains information about
		   the hidraw device. In order to get information about the
		   USB device, get the parent device with the
		   subsystem/devtype pair of "usb"/"usb_device". This will
		   be several levels up the tree, but the function will find
		   it.*/
		dev = udev_device_get_parent_with_subsystem_devtype(
			   dev,
			   "usb",
			   "usb_device");
		if (!dev) {
			printf("Unable to find parent usb device.");
			return -ENODEV;
		}

		udev_device_unref(dev);
	}
	/* Free the enumerator object */
	udev_enumerate_unref(enumerate);

	while (1) {
		fd_set fds;
		struct timeval tv;
		int ret;

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		ret = select(fd+1, &fds, NULL, NULL, &tv);

		/* Check if our file descriptor has received data. */
		if (ret > 0 && FD_ISSET(fd, &fds)) {
			//printf("\nselect() says there should be data\n");

			/* Make the call to receive the device.
			   select() ensured that this will not block. */
			dev = udev_monitor_receive_device(mon);
			if (dev) {
				if (udev_device_get_devnode(dev) == NULL) {
					//fprintf(stderr, "\nNode : NULL");
					goto cont;
				}
				if (strcmp(udev_device_get_devtype(dev), "usb_device")) {
					//fprintf(stderr, "Type : nousb");
					goto cont;
				}
				if (!strcmp(udev_device_get_action(dev), "remove")) {
					//fprintf(stderr, "Action : remove");
					goto cont;
				}
				printf("Got Device\n");
				//char *message = get_device_info();
				detected(SEND, "Old folk");
cont:
				udev_device_unref(dev);
			}
			else {
				printf("No Device from receive_device(). An error occured.\n");
			}
		}
		usleep(250*1000);
		fflush(stdout);
	}


	udev_unref(udev);

	return 0;
}
