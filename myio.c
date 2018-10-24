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
    ssize_t rd_bytes; //number of bytes read
    int buf_count;   //number of bytes in the buffer (normally = BLOCK_SIZE)
    char wr_buf[BLOCK_SIZE];
    ssize_t wr_bytes;  
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
        bufdata->rd_bytes = 0;
    } 
    if(flags == (O_WRONLY | O_RDWR)){
        bufdata->wr_bytes = 0; 
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
    int rd_bytes = bufdata->rd_bytes; 
    int buf_count = bufdata->buf_count; 
    int offset = buf_count-rd_bytes; 
    if(rd_bytes < count){
        if(rd_bytes != 0){
            memcpy(bufdata->rd_buf, bufdata->rd_buf + offset, rd_bytes); 
        }
        buf_count = read(bufdata->fd, bufdata->rd_buf + rd_bytes, (BLOCK_SIZE-rd_bytes)); 

        //check for read(2) error
        if(buf_count == -1){
            perror("read"); 
            return buf_count; 
        } 
        buf_count += rd_bytes; 
        rd_bytes = buf_count;
    }
    offset = buf_count-rd_bytes;
    memcpy(trg_buf, bufdata->rd_buf + offset, count); 
    if (rd_bytes >= count){
        rd_bytes -= count; 
        bufdata->rd_bytes = rd_bytes;  
        bufdata->buf_count = buf_count; 
        return count; 
    }
    bufdata->rd_bytes = 0;  
    bufdata->buf_count = buf_count;
    return buf_count; 
}

ssize_t
mywrite(void *source_buf, struct file_struct *bufdata, size_t count){
    int bytes_loaded = bufdata->wr_bytes; 
    int null_bytes = BLOCK_SIZE-bytes_loaded; 
    if(null_bytes < count){
        int bytes_written = write(bufdata->fd, bufdata->wr_buf, bytes_loaded);

        //check for write(2) error
        if(bytes_written == -1){
            perror("write"); 
            return bytes_written;  
        }  
        bytes_loaded = 0; 
    }
    memcpy((bufdata->wr_buf) + bytes_loaded, source_buf, count); 
    bytes_loaded += count; 
    bufdata->wr_bytes = bytes_loaded; 
    return bytes_loaded;                 
}

off_t
myseek(struct file_struct *bufdata, off_t offset, int whence){ 
    int byte_offset = -1; 
    if((whence == SEEK_SET) | (whence == SEEK_CUR)){
        byte_offset = lseek(bufdata->fd, offset, whence);
    }else{
        perror("myseek"); 
        return byte_offset; 
    }
    return byte_offset; 
}

void
myflush(struct file_struct *bufdata){
    write(bufdata->fd, bufdata->wr_buf, bufdata->wr_bytes);
}