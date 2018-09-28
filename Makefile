CFLAGS = -Wall -pedantic 

.PHONY: all
all: test

test: test1.o myio.o 
	gcc -o $@ $^

%.o: %.c 
	gcc $(CFLAGS) -c -o $@ $^ 

.PHONY: clean 
clean: 
	rm -f myio.o test1.o test 

