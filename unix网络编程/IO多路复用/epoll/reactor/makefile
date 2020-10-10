CC = gcc

ALL:out client

out:server.o main.c
	$(CC) $^ -o  $@ -lpthread -g

server.o:server.c
	$(CC) -c $<

client:client.c
	$(CC) $< -o $@

.PHONY:clean

clean:
	-rm -rf out server.o client

