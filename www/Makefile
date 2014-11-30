serverd.o: server.c
	gcc -c -g server.c -lsqlite3
serverd: server.o
	gcc -o serverd serverd.o -lsqlite3
server.o: server.c
	gcc -c server.c -lsqlite3
server: server.o
	gcc -o server server.o -lsqlite3
