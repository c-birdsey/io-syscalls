CFLAGS = -Wall -pedantic

.PHONY: all
all: test

test1000: test-1000buf.o myio.o
	gcc -o $@ $^ 

test50: test-50buf.o myio.o
	gcc -o $@ $^

test3: test-file3.o myio.o 
	gcc -o $@ $^

test: file-copy.o myio.o 
	gcc -o $@ $^

%.o: %.c 
	gcc $(CFLAGS) -c -o $@ $^ 

.PHONY: clean 
clean: 
	rm -f myio.o file-copy.o test-file2.o test-file3.o test-1000buf.o test-50buf.o test test2 test3 test50 test1000

