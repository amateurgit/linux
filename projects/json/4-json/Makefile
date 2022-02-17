CC=gcc
INC=/usr/local/include/json-c/
LIB=json-c
LIB_PATH=/usr/local/lib/

all:client server

client: tcp_client.c
	$(CC) -o $@ $< -I$(INC) -L$(LIB_PATH) -l$(LIB)

server: tcp_server.c
	$(CC) -o $@ $< -I$(INC) -L$(LIB_PATH) -l$(LIB)

clean:
	rm -f client server *.o
