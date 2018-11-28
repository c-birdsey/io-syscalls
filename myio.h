
//calder birdsey
//cs315 systems programming 
//assignment 2 header file 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE 10

struct file_struct{
    int fd; 
    char rd_buf[BLOCK_SIZE];
    ssize_t rd_bytes;
    int rdbuf_count;   
    char wr_buf[BLOCK_SIZE];
    ssize_t wr_bytes;  
};

struct file_struct* myopen(const char *pathname, int flags); 
int myclose(struct file_struct *bufdata);  
ssize_t myread(struct file_struct *bufdata, void *trg_buf, size_t count); 
ssize_t mywrite(struct file_struct *bufdata, void *source_buf, size_t count);
off_t myseek(struct file_struct *bufdata_in, struct file_struct *bufdata_out, off_t offset, int whence); 
void myflush(struct file_struct *bufdata); 