#include"io.h"


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
FILE* open_file(char* file_name, char* to_concat,char * mode)
{
	FILE* fp;
	int total_length;
	char* obj_file_name = 0;
	if (to_concat)
	{
		total_length = strlen(file_name) + strlen(to_concat) + 1;
		obj_file_name = calloc(total_length, sizeof(char));
		EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(obj_file_name);
		strcpy(obj_file_name, file_name);
		strcat(obj_file_name, to_concat);

		fp = fopen(obj_file_name, mode);
		free(obj_file_name);
		return fp;
	}
	return fopen(file_name, mode);
}