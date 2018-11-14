//calder birdsey
//file-copy.c
//test program for assignment2 
//tests myopen, myclose, myread, mywrite, myflush

#include "myio.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 500

int
main(int argc, char *argv[]) {
    char *in_filename, *out_filename; 
    struct file_struct *bufdata_in;
    struct file_struct *bufdata_out; 
    char buf[BUFFER_SIZE]; 
    ssize_t bytes_read;

    if((argc < 3) || (argc > 4)) {
        printf("usage: %s file1 file2 [file3]\n", argv[0]); 
        printf("  file1: source file\n");
        printf("  file2: destination file\n");
        printf("  file3: optional destination file to test myseek\n"); 
        exit(EXIT_FAILURE); 
    } else {
        //open files  
        in_filename = argv[1];
        out_filename = argv[2];  
        bufdata_in = myopen(in_filename, O_RDONLY);
        bufdata_out = myopen(out_filename, O_WRONLY); 

        //copy inFile to outFile
        while((bytes_read = myread(buf, bufdata_in, BUFFER_SIZE)) > 0) {
            //printf("bytes read: %ld\n", bytes_read);    
            mywrite(buf, bufdata_out, bytes_read);  
        }
        myflush(bufdata_out); 

        //close files
        myclose(bufdata_in);
        myclose(bufdata_out); 
        
        //if optional seekFile parameter given
        if(argc == 4) {
            char *seek_filename; 
            struct file_struct *bufdata_seek; 

            //open files  
            seek_filename = argv[3]; 
            bufdata_seek = myopen(seek_filename, O_WRONLY);
            bufdata_in = myopen(in_filename, O_RDONLY);

            //copy inFile to seekFile to test myseek implementation-- NOT WORKING
            while((bytes_read = myread(buf, bufdata_in, BUFFER_SIZE)) > 0) {
                mywrite(buf, bufdata_seek, bytes_read);  
                myseek(bufdata_in, bufdata_seek, 10, SEEK_CUR); 
            }
            myflush(bufdata_seek); 
            
            //close files
            myclose(bufdata_in); 
            myclose(bufdata_seek); 
        }
        exit(EXIT_SUCCESS);
    }
}