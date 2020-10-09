CC = gcc
out:server.o main.c
	$(CC) $^ -o $@

server.o:server.c
	$(CC) -c $<

.PHONY:clean

clean:
	-rm -rf out server.o

