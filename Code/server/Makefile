server: networks.o  server.o
	gcc networks.o server.o -o server

networks.o: networks.c networks.h
	gcc -c networks.c

server.o : server.c server.h networks.h
	gcc -c server.c

clean:
	rm *.o server
