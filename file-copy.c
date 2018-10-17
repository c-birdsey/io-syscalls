//file-copy.c
//test program for assignment2 
//tests myopen, myclose, myread, mywrite, myflush

#include "myio.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

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

        //copy file
        while((bytes_read = myread(buf, bufdata_in, BUFFER_SIZE)) > 0){
            mywrite(buf, bufdata_out, bytes_read); 
        }
        myflush(bufdata_out);

        //close files
        myclose(bufdata_in);
        myclose(bufdata_out);  
    }
}



         
