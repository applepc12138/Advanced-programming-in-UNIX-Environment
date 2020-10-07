src = $(wildcard *.c)

pipe:$(src:.c=.o)
	gcc $< -o $@

%.o:%.c
	gcc -c $< -o $@

.PHONY:clean

clean:
	-rm -rf pipe *.o
