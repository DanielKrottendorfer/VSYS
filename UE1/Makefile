all: server testclient

server: mailserver.cpp
	g++ -Wall mailserver.cpp ServerSocket.cpp ClientSocket.cpp filehelper.c sockethelper.c -o ./executables/mailserver

testclient: testclient.cpp
	g++ -Wall testclient.cpp ClientSocket.cpp -o ./executables/testclient

clean:
	rm ./executables/mailserver
	rm ./executables/testclient
