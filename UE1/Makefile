all: directories server testclient testserver

server: mailserver.cpp
	g++ -Wall mailserver.cpp ServerSocket.cpp ClientSocket.cpp filehelper.c sockethelper.c -o ./executables/mailserver

testclient: testclient.cpp
	g++ -Wall testclient.cpp ClientSocket.cpp -o ./executables/testclient

testserver: testserver.cpp
	g++ -Wall testserver.cpp ServerSocket.cpp ClientSocket.cpp sockethelper.c -o ./executables/testserver

OUT_DIR := ./executables

MKDIR_P = mkdir -p

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

clean:
	rm -rf $(OUT_DIR)
