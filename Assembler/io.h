#include"Header.h"
#ifndef IO_H
#define IO_H

/*
decription:
this function trying to open a file with mode flag access and return pointer to file descriptor 
args:
char *file_name: the path the file to be opened.
char * to_concat: if we need to add extension or anything else to file name
char * mode which access to open file
return:
FILE*: pointer to file descriptor
*/
FILE* open_file(char* file_name, char* to_concat, char* mode);

#endif