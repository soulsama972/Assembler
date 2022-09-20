#include"Header.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct 
{
	char symbol_name[MAX_SYMBOL_NAME]; /*symbol name*/
	unsigned int address : 16; /*address in the ram*/
	unsigned int is_external : 1; /*boolen to tell if the symbol is ext section */
	unsigned int is_entry : 1; /*boolen to tell if the symbol is ent section */
	unsigned int is_code : 1; /*boolen to tell if the symbol is code section */
}SYMBOL;



struct SYMBOL_TABLE
{
	SYMBOL symbol;
	struct SYMBOL_TABLE* next;
};
typedef struct SYMBOL_TABLE SYMBOL_TABLE;

/*
decription: this function create pointer SYMBOL_TABLE object and return it
args: None
return: SYMBOL_TABLE pointer
*/
SYMBOL_TABLE* create_symbol_table();

/*
decription: this function free the memory that not long in used
args:
SYMBOL_TABLE* symbol_table: the object that we free from memory
return:
None
*/
void free_symbol_table(SYMBOL_TABLE* symbol_table);

/*
decription: this function append to symbol_table object a new symbol data
args:
SYMBOL_TABLE *: pointer to the object (we assume that the table was alloc before) that new data will be append to
char *symbol_name: the new symbol name to add
int address: where the code in the ram
int is_external: boolen to tell if symbol created outside of the current file
int is_entry: boolen to tell if symbol in created in current file
int is_code: boolen to tell if symbol in code section
return: None
*/
void append_to_symbol_table(SYMBOL_TABLE* symbol_table, char* symbol_name, int address, int is_external, int is_entry, int is_code);

/*
decription: this function check whatever or not the symbol already exist in the symbol table
args:
SYMBOL_TABLE* symbol_table: which symbol table to check in the symbol exsist
char* symbol_name: the symbol to compare with
return:
int : TRUE if exsit else FALSE
*/
int is_symbol_already_exist(SYMBOL_TABLE* symbol_table, char *symbol_name);

/*
decription: this function get the SYMBOL object by symbol name
args:
SYMBOL_TABLE* symbol_table: the symbol table to search the name
char * symbol_name: which name to compare
return:
SMYBOL*: if the symbol name exsit return SYMBOL OBJECT otherwise 0
*/
SYMBOL* get_symbol(SYMBOL_TABLE* symbol_table, char* symbol_name);
#endif 