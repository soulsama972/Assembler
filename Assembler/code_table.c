#include "code_table.h"

/*
decription: this function create by malloc CODE_TABLE object and init it and return the pointer object
args: None
return: pointer to CODE_TABLE object
*/
CODE_TABLE* create_code_table()
{
	CODE_TABLE* code_table = calloc(1,sizeof(CODE_TABLE));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(code_table); /*if we dont cant malloc we exit the program*/
	code_table->words = calloc(MAX_RAM_SIZE, sizeof(WORD));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(code_table->words); /*if we dont cant malloc we exit the program*/
	code_table->IC = 100;
	return code_table;
}

/*
decription: this function append word (20bits of data we called them word) to the table
args:
CODE_TABLE *: pointer to the object (we assume that the table was alloc before)
WORD word: word to be appended to the table
return: None
*/
void append_word_to_code_table(CODE_TABLE* code_table, WORD word)
{
	memcpy(&code_table->words[code_table->IC++], &word, sizeof(WORD));
}

/*
decription:
this function free the memory that not long in used
args:
CODE_TABLE* code_table: the object that we free from memory
return:
None
*/
void free_code_table(CODE_TABLE* code_table)
{
	free(code_table->words);
	free(code_table);
	code_table = 0;
}