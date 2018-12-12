CFLAGS = -Wall -pedantic

.PHONY: all
all: file-copy

test1000: test-1000buf.o myio.o
	gcc -o $@ $^ 

test50: test-50buf.o myio.o
	gcc -o $@ $^

test-out: test_outputs.o myio.o 
	gcc -o $@ $^ 

test-inter: test_inter.o myio.o 
	gcc -o $@ $^ 

file-copy: file-copy.o myio.o 
	gcc -o $@ $^

%.o: %.c 
	gcc $(CFLAGS) -c -o $@ $^ 

.PHONY: clean 
clean: 
	rm -f myio.o file-copy.o test_inter.o test_outputs.o test-1000buf.o test-50buf.o file-copy test50 test1000 test-inter test-out

