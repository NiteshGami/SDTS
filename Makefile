serverd.o: server.c
	gcc -c -g server.c
serverd: server.o
	gcc -o serverd serverd.o
server.o: server.c
	gcc -c server.c
server: server.o
	gcc -o server server.o
