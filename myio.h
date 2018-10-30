
//cs315 systems programming 
//assignment 2 header file 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE 100

struct file_struct{
    int fd; 
    char rd_buf[BLOCK_SIZE];
    ssize_t rd_bytes;
    int buf_count;   
    char wr_buf[BLOCK_SIZE];
    ssize_t wr_bytes;  
};

extern struct file_struct* myopen(const char *pathname, int flags); 
extern int myclose(struct file_struct *bufdata);  
extern ssize_t myread(void *trg_buf, struct file_struct *bufdata, size_t count); 
extern ssize_t mywrite(void *source_buf, struct file_struct *bufdata, size_t count);
extern off_t myseek(struct file_struct *bufdata_in, struct file_struct *bufdata_out, off_t offset, int whence); 
extern void myflush(struct file_struct *bufdata);
