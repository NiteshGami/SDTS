configure:
	sudo apt-get install libusb-dev libudev-dev
all:
	gcc *.c -o target -ludev
install:
	cp target /usr/bin/
	cp dev.pl /usr/bin/
	echo "up eth0 target &" >> /etc/network/interfaces
	echo "down eth0 killall target" >> /etc/network/interfaces
package:
	tar -zc Makefile target dev.pl -f sdts.tar.gz

