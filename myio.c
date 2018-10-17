//calder birdsey
//cs315 systems programming 
//assignment 2 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4096

//define struct file_struct
struct file_struct{
    int fd; 
    char rd_buf[BLOCK_SIZE];
    ssize_t rd_buf_bytes;  
    char wr_buf[BLOCK_SIZE];
    ssize_t wr_buf_bytes;   
};
 
struct file_struct *
myopen(const char *pathname, int flags){
    //initialize struct
    struct file_struct *bufdata = malloc(sizeof(*bufdata));  
    if(bufdata == NULL){
        perror("malloc"); 
        return NULL; 
    }

    //initialize rd/wr buffers
    if(flags == (O_RDONLY | O_RDWR)){
        bufdata->rd_buf_bytes = 0;
    } 
    if(flags == (O_WRONLY | O_RDWR)){
        bufdata->wr_buf_bytes = 0; 
    }

    //call open(2)
    bufdata->fd = open(pathname, flags);
    
    //check for open(2) error
    if(bufdata->fd == -1){
        perror("open");
        return NULL; 
    }
    return bufdata;
}

int
myclose(struct file_struct *bufdata){
    int close_status = close(bufdata->fd);  
    
    //check for close(2) error
    if(close_status == -1){ 
        perror("close");
        return close_status; 
    }
    free(bufdata);
    return close_status; 
}

ssize_t
myread(void *trg_buf, struct file_struct *bufdata, size_t count){
    int rd_buf_bytes = bufdata->rd_buf_bytes; 
    int bytes_read = 0; 
    //printf("rd bytes loaded:%d\n", rd_buf_bytes); 
    if(rd_buf_bytes < count){
        bytes_read = read(bufdata->fd, bufdata->rd_buf, (BLOCK_SIZE-rd_buf_bytes)); 
        rd_buf_bytes += bytes_read;
        //printf("bytes loaded after read sys call:%d\n", rd_buf_bytes); 
        //printf("bytes read:%d\n", bytes_read);  

        //check for read(2) error
        if(bytes_read == -1){
            perror("read"); 
            return bytes_read; 
        } 
    }
    memcpy(trg_buf, bufdata->rd_buf, count); 
    if (rd_buf_bytes >= count){
        rd_buf_bytes -= count; 
        bufdata->rd_buf_bytes = rd_buf_bytes;  
        //printf("count:%ld\n\n", count); 
        return count; 
    }
    rd_buf_bytes = 0; 
    bufdata->rd_buf_bytes = rd_buf_bytes;  
    return bytes_read; 
}

ssize_t
mywrite(void *source_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->wr_buf_bytes; 
    //printf("wr bytes loaded: %d\n", bytes_loaded); 
    if(bytes_loaded == BLOCK_SIZE){
        int bytes_written = write(bufdata->fd, bufdata->wr_buf, BLOCK_SIZE);
        //printf("bytes written: %d\n", bytes_written);
        //check for write(2) error
        if(bytes_written == -1){
            perror("write"); 
            return bytes_written;  
        }  
        bytes_loaded = 0; 
    }
    if(bytes_loaded < BLOCK_SIZE){
        //how to handle a count > BLOCK_SIZE-bytes_loaded?
        memcpy(bufdata->wr_buf, source_buf, count); 
        bytes_loaded += count; 
        //printf("wr buf isnt full, wr bytes loaded after copy: %d\n", bytes_loaded);         
    }
    bufdata->wr_buf_bytes = bytes_loaded;
    //printf("wr bytes loaded: %d\n\n", bytes_loaded);  
    return bytes_loaded;                 
}

off_t
myseek(int fd, off_t offset, int whence){ 
    int byte_offset = -1; 
    if((whence == SEEK_SET) | (whence == SEEK_CUR)){
        byte_offset = lseek(fd, offset, whence);
    }else{
        perror("myseek"); 
        return byte_offset; 
    }
    return byte_offset; 
}

void
myflush(struct file_struct *bufdata){
    write(bufdata->fd, bufdata->wr_buf, bufdata->wr_buf_bytes);
}