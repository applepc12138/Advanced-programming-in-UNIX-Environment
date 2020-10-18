CC = g++

all: server client

server:main.cpp timer_list.o 
	$(CC) $^ -o $@

timer_list.o:timer_list.cpp
	$(CC) -c $<

client:client.c
	$(CC) $< -o $@

.PHONY:clean

clean:
	-rm -rf server timer_list.o client
