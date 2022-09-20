#include"symbol_table.h"

/*
decription: this function create pointer SYMBOL_TABLE object and return it
args: None
return: SYMBOL_TABLE pointer
*/
SYMBOL_TABLE* create_symbol_table()
{
	SYMBOL_TABLE* symbol_table = calloc(1, sizeof(SYMBOL_TABLE));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(symbol_table); /*if we dont cant malloc we exit the program*/
	symbol_table->next = NULL;
	return symbol_table;

}

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
void append_to_symbol_table(SYMBOL_TABLE* symbol_table, char* symbol_name,int address, int is_external, int is_entry, int is_code)
{

	SYMBOL_TABLE* temp = symbol_table;
	SYMBOL_TABLE* s_table = calloc(1, sizeof(SYMBOL_TABLE));
	while (temp->next != NULL)
		temp = temp->next;
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(s_table); /*if we dont cant malloc we exit the program*/
	strcpy(temp->symbol.symbol_name, symbol_name);
	temp->symbol.address = address;
	temp->symbol.is_external = is_external;
	temp->symbol.is_entry = is_entry;
	temp->symbol.is_code = is_code;
	temp->next = 0;
	temp->next = s_table;
}


/*
decription: this function free the memory that not long in used
args:
SYMBOL_TABLE* symbol_table: the object that we free from memory
return:
None
*/
void free_symbol_table(SYMBOL_TABLE* symbol_table)
{
	SYMBOL_TABLE* temp;
	while (symbol_table)
	{
		temp = symbol_table;
		symbol_table = symbol_table->next;
		free(temp);
		temp = symbol_table;
	}
	symbol_table = 0;
}


/*
decription: this function check whatever or not the symbol already exist in the symbol table
args:
SYMBOL_TABLE* symbol_table: which symbol table to check in the symbol exsist
char* symbol_name: the symbol to compare with
return:
int : TRUE if exsit else FALSE
*/
int is_symbol_already_exist(SYMBOL_TABLE* symbol_table, char* symbol_name)
{
	SYMBOL_TABLE* temp = symbol_table;
	while (temp)
	{
		if (!strcmp(temp->symbol.symbol_name, symbol_name))
			return TRUE;
		temp = temp->next;
	}
	return FALSE;
}

/*
decription: this function get the SYMBOL object by symbol name
args:
SYMBOL_TABLE* symbol_table: the symbol table to search the name
char * symbol_name: which name to compare
return: 
SMYBOL*: if the symbol name exsit return SYMBOL OBJECT otherwise 0
*/
SYMBOL* get_symbol(SYMBOL_TABLE* symbol_table, char* symbol_name)
{
	SYMBOL_TABLE *temp = symbol_table;
	while (temp->next)
	{
		if (!strcmp(temp->symbol.symbol_name, symbol_name))
			return &temp->symbol;
		temp = temp->next;
	}
	return 0;
}
