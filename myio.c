//calder birdsey
//cs315 systems programming 
//assignment 2 
//9.27.18 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 10240

struct
myopen(const char *pathname, int flags){
    //initialize struct
    struct open_struct{
        int fd; 
        void *rd_buf;
        ssize_t rd_buf_bytes;  
        void *wr_buf;
        ssize_t wr_buf_bytes;   
    }

    //create rd/wr buffers
    if(flags == O_RDONLY | O_RDWR){
        rd_buf = malloc(BLOCK_SIZE);
        rd_buf_bytes = 0;
    } 
    if(flags == O_WRONLY | O_RDWR){
        wr_buf = malloc(BLOCK_SIZE);
        wr_buf_bytes = 0; 
    }

    //call open(2) and return fd
    if(flags == O_CREAT | O_TRUNC){
        fd = open(pathname, flags, 0666); 
    else
        fd = open(pathname, flags); 
    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE); 
    }
    return open_struct;
}

int
myclose(struct open_struct){
    int close_status = close(open_struct.fd);  
    if(close_status == -1){
        perror("close");
        exit(EXIT_FAILURE);
    }
    free(open_struct.rd_buf);
    free(open_struct.wr_buf); 
    return close_status; 
}
 
ssize_t
myread(int fd, void *trg_buf, struct open_struct, size_t count){
    int bytes_read = open_struct.rd_buf_bytes; 
    if(bytes_read < count){ //how to check if buffer is full or empty?
        bytes_read += read(open_struct.fd, open_struct.rd_buf, (BLOCK_SIZE-bytes_read));
        if(bytes_read == -1){
            perror("read"); 
            exit(EXIT_FAILURE);
        } 
    }
    memcpy(trg_buf, open_struct.rd_buf, count);
    bytes_read = bytes_read - count; 
    return bytes_read; 
}

ssize_t
mywrite(int fd, void *trg_buf, struct open_struct, size_t count){
    if((BLOCK_SIZE-bytes_written) < count ){//how to check if buffer isnt full
        bytes_written += write(open_struct.fd, open_struct.wr_buf, bytes_written);
        if(bytes_written == -1){
            perror("write"); 
            exit(EXIT_FAILURE); 
        } 
    }                

}

off_t
myseek(int fd, off_t offset, int whence){ 
    printf("foo"); 
    //repositions file offset of fd to offset, according to option whence 
    //implement SEEK_SET (file offset is set to offset bytes) and SEEK_CUR (file offset is set to current + offset bytes)
}

void
myflush(int fd, void *buf, size_t count){
    printf("foo"); 
    //write(fd, buf, count); 
    //flushes buffer and writes whatever is loaded (before its full) 
}

