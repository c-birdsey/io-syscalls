//test file1 for assign02 

#include "myio.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10240

int
main(int argc, char *argv[]){
    if((argc) != 3){
        printf("usage: %s source dest\n", argv[0]); 
        exit(EXIT_FAILURE); 
    }else{
        const char *file = argv[1]; 
        int flag = argv[2]; 
        int fd = myopen(file, flag);
        char *buf[BUFFER_SIZE]; 
        ssize_t bytes_rd = myread(fd, buf, 1024);
        printf("%d\n", fd);
        printf("%d\n", myclose(fd)); 
        printf("%ld\n", bytes_rd);  
    }
}



         
