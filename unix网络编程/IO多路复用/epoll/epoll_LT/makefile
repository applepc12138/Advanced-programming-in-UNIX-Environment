all:server client

CC = gcc

server:server.c
	$(CC) $< -o $@

client:client.c
	$(CC) $< -o $@

.PHONY:clean all 


clean:
	-rm -rf server client
