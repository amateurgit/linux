all:client server

client:udp_client.o
	gcc -o client udp_client.o

server:udp_server.o
	gcc -o server udp_server.o

clean:
	rm -f client server *.o
