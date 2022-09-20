#include"Header.h"
#include"word_structrue.h"

#ifndef CODE_TABLE_H
#define CODE_TABLE_H

typedef struct
{
	int IC; /* inspection  counter */
	WORD *words; /*array of WORD [size of ram]*/
}CODE_TABLE;


/*
decription: this function create by malloc CODE_TABLE object and init it and return the pointer object
args: None
return: pointer to CODE_TABLE object
*/
CODE_TABLE* create_code_table();

/*
decription: this function append word (20bits of data we called them word) to the table
args:
CODE_TABLE *: pointer to the object (we assume that the table was alloc before)
WORD word: word to be appended to the table
return: None
*/
void append_word_to_code_table(CODE_TABLE* code_table, WORD word);

/*
decription:
this function free the memory that not long in used
args:
CODE_TABLE* code_table: the object that we free from memory
return:
None
*/
void free_code_table(CODE_TABLE* code_table);
#endif 