src = $(wildcard *.c)
truncate:$(src:.c=.o)
	gcc $< -o $@

%.o:%.c
	gcc -c $< -o $@

.PHONY:clean

clean:
	-rm ./*.txt truncate ./*.o 
