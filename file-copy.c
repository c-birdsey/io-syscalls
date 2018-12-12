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
        printf("  file2: destinationm file\n");
        printf("  file3: optional destination file to test myseek\n"); 
        exit(EXIT_FAILURE); 
    } else {
        //open files  
        in_filename = argv[1];
        out_filename = argv[2];  
        bufdata_in = myopen(in_filename, O_RDONLY);
        bufdata_out = myopen(out_filename, O_WRONLY); 

        //copy inFile to outFile
        while((bytes_read = myread(bufdata_in, buf, BUFFER_SIZE)) > 0) {
            printf("bytes read: %ld\n", bytes_read);    
            ssize_t wr_bytes = mywrite(bufdata_out, buf, bytes_read);  
            printf("bytes written: %ld\n", wr_bytes);    
        }

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

            int seek_count = 0; 
            //copy inFile to seekFile to test myseek implementation-- NOT WORKING
            while((bytes_read = myread(bufdata_in, buf, BUFFER_SIZE)) > 0) {
                mywrite(bufdata_seek, buf, bytes_read);  
                myseek(bufdata_in, bufdata_seek, 10, SEEK_CUR); 
                seek_count += 1; 
            }
            //printf("seek count: %d\n", seek_count); 
            
            //close files
            myclose(bufdata_in); 
            myclose(bufdata_seek); 
        }
        /*
        printf("REAL SYS CALLS:\n");

        int out = open(out_filename, O_WRONLY); 
        int in = open(in_filename, O_RDONLY);

        while((bytes_read = read(in, buf, BUFFER_SIZE)) > 0) {
            printf("bytes read: %ld\n", bytes_read);    
            ssize_t wr_bytes = write(out, buf, bytes_read);  
            printf("bytes written: %ld\n", wr_bytes);    
        } 

        close(in); 
        close(out); 

        exit(EXIT_SUCCESS);
        */
    }
}