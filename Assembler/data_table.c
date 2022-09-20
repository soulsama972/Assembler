#include "data_table.h"


/*
decription: this function create by malloc DATA_TABLE object and init it and return the pointer object
args: None
return: pointer to DATA_TABLE object
*/
DATA_TABLE* create_data_table()
{
	DATA_TABLE* data_table = calloc(1,sizeof(DATA_TABLE));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(data_table); /*if we dont cant malloc we exit the program*/
	data_table->words = calloc(MAX_RAM_SIZE, sizeof(WORD));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(data_table->words); /*if we dont cant malloc we exit the program*/
	data_table->DC = 0;
	return data_table;
}

/*
decription:
this function append word (20bits of data we called them word) to the table 
args:
DATA_TABLE *: pointer to the object (we assume that the table was alloc before)
WORD word: word to be appended to the table
return:
None
*/
void append_word_to_data_table(DATA_TABLE* data_table, WORD word)
{
	memcpy(&data_table->words[data_table->DC++], &word, sizeof(WORD));
}

/*
decription:
this function free the memory that not long in used
args:
DATA_TABLE* data_table: the object that we free from memory
return:
None
*/
void free_data_table(DATA_TABLE* data_table)
{
	free(data_table->words);
	free(data_table);
	data_table = 0;
}