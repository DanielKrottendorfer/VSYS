all: clientTest server

clientTest: clientTest.cpp
	g++ -Wall clientTest.cpp filehelper.c sockethelper.cpp -o clientTest

server: myserver.cpp
	g++ -Wall myserver.cpp filehelper.c sockethelper.cpp -o myserver

clean:
	rm clientTest
	rm myserver
