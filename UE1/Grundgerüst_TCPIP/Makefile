all: client server

client: myclient.cpp
	g++ -Wall myclient.cpp filehelper.c sockethelper.cpp -o myclient

server: myserver.cpp
	g++ -Wall myserver.cpp filehelper.c sockethelper.cpp -o myserver

clean:
	rm myclient
	rm myserver
