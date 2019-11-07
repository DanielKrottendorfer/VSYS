all: directories server client 

server: mailserver.cpp
	g++ -pthread -std=c++11 -Wall mailserver.cpp ServerSocket.cpp ClientSocket.cpp messagehandler.cpp ldap.cpp utils.cpp filehelper.c sockethelper.c -o ./executables/mailserver -lldap -llber

client: mailclient.cpp
	g++ -std=c++11 -Wall mailclient.cpp ClientSocket.cpp -o ./executables/mailclient

OUT_DIR := ./executables

MKDIR_P = mkdir -p

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

clean:
	rm -rf $(OUT_DIR)
