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

#define BLOCK_SIZE 20480 //size of buffer block

struct file_struct{
    int fd; 
    void *rd_buf;
    ssize_t rd_buf_bytes;  
    void *wr_buf;
    ssize_t wr_buf_bytes;   
};
 
struct file_struct *
myopen(const char *pathname, int flags){
    //initialize struct
    struct file_struct *bufdata = malloc(sizeof(*bufdata));  

    //create rd/wr buffers
    if(flags == (O_RDONLY | O_RDWR)){
        //bufdata->rd_buf = malloc(BLOCK_SIZE);
        bufdata->rd_buf_bytes = 0;
    } 
    if(flags == (O_WRONLY | O_RDWR)){
        //bufdata->wr_buf = malloc(BLOCK_SIZE);
        bufdata->wr_buf_bytes = 0; 
    }

    /*check for malloc() error 
    if((bufdata->wr_buf == NULL) | (bufdata->rd_buf == NULL)){
        perror("malloc"); 
        exit(EXIT_FAILURE); 
    } */  
        
    //call open(2) and return fd
    if(flags == (O_CREAT | O_TRUNC)){
        bufdata->fd = open(pathname, flags, 0666); 
    }else{
        bufdata->fd = open(pathname, flags); 
    }
    if(bufdata->fd == -1){
        perror("open");
        exit(EXIT_FAILURE); 
    }
    return bufdata;
}

int
myclose(struct file_struct *bufdata){
    int close_status = close(bufdata->fd);  
    if(close_status == -1){
        perror("close");
        exit(EXIT_FAILURE);
    }
    free(bufdata->rd_buf);
    free(bufdata->wr_buf);
    free(bufdata); //free memory allocation for file_struct  
    return close_status; 
}

ssize_t
myread(void *trg_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->rd_buf_bytes; 
    if(bytes_loaded < count){ 
        int bytes_read = read(bufdata->fd, bufdata->rd_buf, (BLOCK_SIZE-bytes_loaded));
        bytes_loaded += bytes_read; 
        if(bytes_read == -1){
            perror("read"); 
            exit(EXIT_FAILURE);
        } 
    }
    memcpy(trg_buf, bufdata->rd_buf, count);
    if(bytes_loaded > count){
        bytes_loaded -= count;  
    }
    return bytes_loaded; 
}

ssize_t
mywrite(void *source_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->wr_buf_bytes; 
    if(bytes_loaded < BLOCK_SIZE){
        memcpy(bufdata->wr_buf, source_buf, count); 
        bytes_loaded += count;         
    }else{
        int bytes_written = write(bufdata->fd, bufdata->wr_buf, BLOCK_SIZE);
        if(bytes_written == -1){
            perror("write"); 
            exit(EXIT_FAILURE); 
        }  
        bufdata->wr_buf_bytes = 0; 
    }
    bufdata->wr_buf_bytes = bytes_loaded; 
    return bytes_loaded;                 
}

off_t
myseek(int fd, off_t offset, int whence){ 
    int byte_offset = -1; 
    if(whence == SEEK_SET){
        byte_offset = lseek(fd, offset, whence);
    }
    if(whence == SEEK_CUR){
        byte_offset = lseek(fd, offset, whence); 
    }
    return byte_offset; 
    //repositions file offset of fd to offset, according to option whence 
    //implement SEEK_SET (file offset is set to offset bytes) and SEEK_CUR (file offset is set to current + offset bytes)
}

void
myflush(struct file_struct *bufdata){
   /* if(bufdata->rd_buf_bytes > 0){
        memcpy(trg_buf, bufdata->rd_buf, bufdata->rd_buf_bytes); 
    }*/
    if(bufdata->wr_buf_bytes > 0){
        write(bufdata->fd, bufdata->wr_buf, bufdata->wr_buf_bytes); //writes all bytes in wr_buf to file fd 
    }
}

