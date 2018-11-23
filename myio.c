//calder birdsey
//cs315 systems programming 
//assignment 2: myio 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"

struct file_struct *
myopen(const char *pathname, int flags) {
    //initialize struct
    struct file_struct *bufdata = malloc(sizeof(*bufdata));  
    if(bufdata == NULL) {
        return NULL; 
    }
    //initialize rd/wr buffers and mode flag 
    int MODE = 0; 
    bufdata->wr_bytes = 0;
    bufdata->rd_bytes = 0;
    bufdata->buf_count = 0; 

    if(flags == (O_CREAT | O_TRUNC)) {
        MODE = 1; 
    }

    //call open(2)
    if(MODE == 1) {
        bufdata->fd = open(pathname, flags, 0666);
    } else {
        bufdata->fd = open(pathname, flags);
    }
    
    //check for open(2) error
    if(bufdata->fd == -1) {
        free(bufdata);
        return NULL; 
    }
    return bufdata;
}

int
myclose(struct file_struct *bufdata) {
    //flush file if necessary 
    if (bufdata->wr_bytes != 0) {
        myflush(bufdata); 
    }

    //close file 
    int close_status = close(bufdata->fd);  

    //check for close(2) error
    if(close_status == -1) { 
        return close_status; 
    }
    free(bufdata);
    return close_status; 
}

ssize_t
myread(struct file_struct *bufdata, void *trg_buf, size_t count) {
    int rd_bytes = bufdata->rd_bytes; 
    int buf_count = bufdata->buf_count; 
    int offset = buf_count-rd_bytes; 
    if(rd_bytes < count) {
        if(rd_bytes != 0) {
            memcpy(bufdata->rd_buf, bufdata->rd_buf + offset, rd_bytes); 
        }
        buf_count = read(bufdata->fd, bufdata->rd_buf + rd_bytes, (BLOCK_SIZE-rd_bytes)); 

        //check for read(2) error
        if(buf_count == -1) {
            return buf_count; 
        } 
        buf_count += rd_bytes; 
        rd_bytes = buf_count;
    }
    offset = buf_count-rd_bytes;
    memcpy(trg_buf, bufdata->rd_buf + offset, count); 
    if (rd_bytes >= count) {
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
mywrite(struct file_struct *bufdata, void *source_buf, size_t count) {
    int bytes_loaded = bufdata->wr_bytes; 
    int null_bytes = BLOCK_SIZE-bytes_loaded; 
    int WRITTEN = 0; //write flag to determine what is returned 
    int bytes_written; 
    if(null_bytes < count) {
        bytes_written = write(bufdata->fd, bufdata->wr_buf, bytes_loaded);
        WRITTEN = 1; 
    
        //check for write(2) error
        if(bytes_written == -1) {
            return bytes_written;  
        }  
        bytes_loaded = 0; 
    }
    memcpy((bufdata->wr_buf) + bytes_loaded, source_buf, count); 
    bytes_loaded += count; 
    bufdata->wr_bytes = bytes_loaded; 
    //return bytes_loaded;    
    
    if(WRITTEN == 1) {
        return bytes_written; 
    } else {
        return 0;
    }
}

off_t
myseek(struct file_struct *bufdata_in, struct file_struct *bufdata_out, off_t offset, int whence) {
    int bytes_unread = bufdata_in->rd_bytes; 
    if((offset < bytes_unread) && (whence == SEEK_CUR)) {
        bytes_unread -= offset;
    } else {
        if(whence == SEEK_SET) {
            lseek(bufdata_in-> fd, offset, whence); 
        } else {
            lseek(bufdata_in-> fd, offset - bytes_unread, whence); 
        }
        bytes_unread = 0; 
    }
    bufdata_in->rd_bytes = bytes_unread; 
    return offset; 
}

void
myflush(struct file_struct *bufdata) {
    write(bufdata->fd, bufdata->wr_buf, bufdata->wr_bytes);
    bufdata->wr_bytes = 0; 
}