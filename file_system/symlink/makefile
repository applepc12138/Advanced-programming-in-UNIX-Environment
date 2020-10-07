src = $(wildcard *.c)
symlink:$(src)
	gcc $< -o $@

.PHONY:clean

clean:
	-rm -rf symlink *.txt
