client: client.o networks.o
	gcc client.o networks.o -o client

networks.o: networks.c networks.h
	gcc -c networks.c
	
client.o : client.c client.h networks.h
	gcc -c client.c

clean:
	rm *.o client
