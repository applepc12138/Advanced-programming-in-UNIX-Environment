src = $(wildcard *.c)
#src = main.c
obj = $(src:%.c=%.o)

chmod : $(obj) 
	gcc $< -o $@

%.o:%.c
	gcc -c $< -o $@

.PHONY:clean test

clean:
	-rm -rf ./*.o chmod ./*.txt

test:
	echo $(src)
	echo $(obj)
	
