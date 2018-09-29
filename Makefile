CFLAGS = -Wall -pedantic 

.PHONY: all
all: test

test: file-copy.o myio.o 
	gcc -o $@ $^

%.o: %.c 
	gcc $(CFLAGS) -c -o $@ $^ 

.PHONY: clean 
clean: 
	rm -f myio.o file-copy.o test 

