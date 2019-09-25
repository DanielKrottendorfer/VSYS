all: client server

client: myclient.c
	gcc -Wall myclient.c filehandler.c -o myclient

server: myserver.c
	gcc -Wall myserver.c filehandler.c -o myserver

clean:
	rm myclient
	rm myserver
