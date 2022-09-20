#include"Header.h"
#include"preprocessing.h"
#include"parser.h"

extern int found_error;
/*
decription: the main function get from the user number of files and try to disasimbly it
args:
int argc: the number of argument pass to the main by the user
char* argv[]: the string contain the info about the file
return:
int: zero if everythime is okay else -1
*/
int main(int argc, char* argv[])
{
	char* file_name_buffer = 0;
	char* file_name_without_marco = 0;
	int length_of_file_name;
	int i;
	for (i = 1; i < argc; i++)
	{
		length_of_file_name = strlen(argv[i]) + 10;
		file_name_buffer = calloc(length_of_file_name, sizeof(char));
		EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(file_name_buffer);

		file_name_without_marco = calloc(length_of_file_name, sizeof(char));
		EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(file_name_without_marco);

		strcpy(file_name_buffer, argv[i]);
		strcat(file_name_buffer, ".as");

		strcpy(file_name_without_marco, argv[i]);
		strcat(file_name_without_marco, ".am");


		if (create_file_without_macro(file_name_buffer, file_name_without_marco))
		{
			fprintf(stdout,"FILE %s status:\n",file_name_buffer);
			prase_file(file_name_without_marco,argv[i]);
			if(!found_error) fprintf(stdout,"OK\n\n");
		}

		free(file_name_buffer);
		free(file_name_without_marco);
	}
	return 0;
}