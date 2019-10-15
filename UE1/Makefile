all: directories server client echoserver

server: mailserver.cpp
	g++ -Wall mailserver.cpp ServerSocket.cpp ClientSocket.cpp messagehandler.cpp filehelper.c sockethelper.c -o ./executables/mailserver

client: mailclient.cpp
	g++ -Wall mailclient.cpp ClientSocket.cpp -o ./executables/mailclient

echoserver: echoserver.cpp
	g++ -Wall echoserver.cpp ServerSocket.cpp ClientSocket.cpp sockethelper.c -o ./executables/echoserver

OUT_DIR := ./executables

MKDIR_P = mkdir -p

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

clean:
	rm -rf $(OUT_DIR)
