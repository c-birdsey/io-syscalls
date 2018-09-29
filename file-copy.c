//file-copy.c
//test program for assignment2 
//tests myopen, myclose, myread, mywrite, myflush

#include "myio.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096 

struct file_struct{
    int fd; 
    void *rd_buf;
    ssize_t rd_buf_bytes;  
    void *wr_buf;
    ssize_t wr_buf_bytes;   
};

int
main(int argc, char *argv[]){
    char *in_filename, *out_filename;
    ssize_t bytes_read; 
    struct file_struct *bufdata_in;
    struct file_struct *bufdata_out; 
    char buf[BUFFER_SIZE]; 

    if((argc) != 3){
        printf("usage: %s source dest\n", argv[0]); 
        exit(EXIT_FAILURE); 
    }else{
        //open files  
        in_filename = argv[1];
        out_filename = argv[2];  
        bufdata_in = myopen(in_filename, O_RDONLY);
        bufdata_out = myopen(out_filename, O_WRONLY);
        bufdata_in->rd_buf = malloc(20480); 
        bufdata_out->wr_buf = malloc(20480); 

        //copy file
        while((bytes_read = myread(buf, bufdata_in, BUFFER_SIZE)) > 0){       
            printf("%ld\n", bytes_read); //for testing purposes
            mywrite(buf, bufdata_out, bytes_read); 
        }
        myflush(bufdata_out);

        //close files
        myclose(bufdata_in);
        myclose(bufdata_out);  
    }
}



         
