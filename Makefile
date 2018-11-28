CFLAGS = -Wall -pedantic

.PHONY: all
all: test 

test2: test-file2.o myio.o
	gcc -o $@ $^

test3: test-file3.o myio.o
	gcc -o $@ $^

test: file-copy.o myio.o 
	gcc -o $@ $^

%.o: %.c 
	gcc $(CFLAGS) -c -o $@ $^ 

.PHONY: clean 
clean: 
	rm -f myio.o file-copy.o test-file2.o test-file3.o test test2 test3 

