//calder birdsey
//cs315 systems programming 
//assignment 2 header file 
//9.27.18 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

extern struct myopen(const char *pathname, int flags); 
extern int myclose(int fd); 
extern ssize_t myread(int fd, void *buf, size_t count); 
extern ssize_t mywrite(int fd, void *buf, size_t count); 
extern off_t myseek(int fd, off_t offset, int whence); 
extern void myflush(int fd, void *buf, size_t count);
