all: client server

client: myclient.c
	gcc -Wall myclient.c filehelper.c sockethelper.c -o myclient

server: myserver.c
	gcc -Wall myserver.c filehelper.c sockethelper.c -o myserver

clean:
	rm myclient
	rm myserver
