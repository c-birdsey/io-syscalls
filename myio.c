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

//define struct file_struct
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

    //create rd/wr buffers- ERROR WHILE INITIALIZING BUFFERS
    if(flags == (O_RDONLY | O_RDWR)){
        //bufdata->rd_buf = malloc(BLOCK_SIZE);
        bufdata->rd_buf_bytes = 0;
    } 
    if(flags == (O_WRONLY | O_RDWR)){
        //bufdata->wr_buf = malloc(BLOCK_SIZE);
        bufdata->wr_buf_bytes = 0; 
    }

    //call open(2) and return fd, with proper mode
    if(flags == (O_CREAT | O_TRUNC)){
        bufdata->fd = open(pathname, flags, 0666); 
    }else{
        bufdata->fd = open(pathname, flags); 
    }
    
    //check for open(2) error
    if(bufdata->fd == -1){
        perror("open");
        exit(EXIT_FAILURE); 
    }
    return bufdata;
}

int
myclose(struct file_struct *bufdata){
    int close_status = close(bufdata->fd);  
    
    //check for close(2) error
    if(close_status == -1){ 
        perror("close");
        exit(EXIT_FAILURE);
    }
    free(bufdata->rd_buf); //free memory allocation for read buf
    free(bufdata->wr_buf); //free memory allocation for write buf
    free(bufdata); //free memory allocation for file_struct  
    return close_status; 
}

ssize_t
myread(void *trg_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->rd_buf_bytes; //bytes_loaded represents bytes in read buf
    if(bytes_loaded < count){ 
        int bytes_read = read(bufdata->fd, bufdata->rd_buf, (BLOCK_SIZE-bytes_loaded));
        bytes_loaded += bytes_read;

        //check for read(2) error
        if(bytes_read == -1){
            perror("read"); 
            exit(EXIT_FAILURE);
        } 
    }
    memcpy(trg_buf, bufdata->rd_buf, count);
    if(bytes_loaded > count){ //determining what to return for bytes read into target buf
        bytes_loaded -= count; 
        bufdata->rd_buf_bytes = bytes_loaded;  
        return count; 
    }
    return bytes_loaded; //if bytes_loaded <= count, bytes_loaded is also the number of bytes read into target buf 
}

ssize_t
mywrite(void *source_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->wr_buf_bytes; 
    if(bytes_loaded < BLOCK_SIZE){ 
        memcpy(bufdata->wr_buf, source_buf, count); 
        bytes_loaded += count;         
    }else{
        int bytes_written = write(bufdata->fd, bufdata->wr_buf, BLOCK_SIZE);

        //check for write(2) error
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
}

void
myflush(struct file_struct *bufdata){
    if(bufdata->wr_buf_bytes > 0){
        write(bufdata->fd, bufdata->wr_buf, bufdata->wr_buf_bytes); //writes all bytes in wr_buf to file fd 
    }
}

