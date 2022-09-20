#include"Header.h"
#include"word_structrue.h"
#ifndef DATA_TABLE_H
#define DATA_TABLE_H

typedef struct
{
	int DC; /*data counter*/
	WORD *words; /*array of words[size of ram]*/
}DATA_TABLE;


/*
decription: this function create by malloc DATA_TABLE object and init it and return the pointer object
args: None
return: pointer to DATA_TABLE object
*/
DATA_TABLE* create_data_table();

/*
decription:
this function append word (20bits of data we called them word) to the table
args:
DATA_TABLE *: pointer to the object (we assume that the table was alloc before)
WORD word: word to be appended to the table
return:
None
*/
void append_word_to_data_table(DATA_TABLE* data_table, WORD word);

/*
decription:
this function free the memory that not long in used
args:
DATA_TABLE* data_table: the object that we free from memory
return:
None
*/
void free_data_table(DATA_TABLE* data_table);
#endif