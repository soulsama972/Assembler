#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_RAM_SIZE 8192 /*size of ram for this pc*/
#define MAX_SYMBOL_NAME 31 /*max char allow to symbol name*/
#define MAX_MACRO_NAME 31  /*max char allow to marco name*/
#define MAX_LINE 81 /*max char allow to line */
#define FALSE 0
#define TRUE 1

/*
the define used to check if the alloaction was failed or not if the alloaction was failed print to the user a message error and exit the program
*/
#define EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(x) \
		if (!x){\
		fprintf(stdout, "cannot allocate memory exiting\n");\
		exit(-1);\
		}

/*
the define check if the file was open or not if it didnt open print to the user an error message about it and reutrn FALSE about the check
*/
#define RETURN_IF_FAILED_TO_OPEN_FILE_AND_PRINT_ERROR(x,file_name) \
		if (!x){\
		fprintf(stdout, "failed to open file %s\n", file_name);\
		return FALSE;\
		}

#endif 
