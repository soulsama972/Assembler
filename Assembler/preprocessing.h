#include"Header.h"
#ifndef PREPROCESSING_H
#define PREPROCESSING_H

struct LINES
{
	char line[MAX_LINE];
	struct LINES* next;
};
typedef struct LINES LINES;

typedef struct
{
	char macro_name[MAX_LINE];
	LINES* lines;
}MACRO_INFO;

struct MACRO_TABLE
{
	MACRO_INFO* m_info;
	struct MACRO_TABLE* next;
};
typedef struct MACRO_TABLE MACRO_TABLE;


/*
decription: this function check if the token is saved token
args:
char * tok: the token to check if its saved token
return:
int: TRUE token is save token else FALSE
*/
int is_saved_word(char* word);

/*
decription: this function doing the preprossing to the orginal file with the marco definiton
args:
char* file_orginal_name: the file_orginal_name for exmaple "code.as"
char* file_without_macro_name: the file_without_macro_name for exmaple "code.am"
return:
int TRUE if createing the new file was sccssufuly otherwise FALSE
*/
int create_file_without_macro(char* file_orginal_name, char* file_without_macro_name);

/*
decription: this function create MACRO_INFO OJBECT and return it
args:
char macro_name[MAX_MACRO_NAME]: the macro name
LINES* lines: the lines that represent body of the marco
return:
MACRO_INFO*: MARCO_INFO POINTER
*/
MACRO_INFO* create_macro_info(char macro_name[MAX_MACRO_NAME], LINES* lines);

/*
decription: this function create by malloc MACRO_TABLE object and init it and return the pointer object
args:
MACRO_INFO* m_info: the macro info that we set the the first macro table
return: pointer to MACRO_TABLE object
*/
MACRO_TABLE* create_macro_table(MACRO_INFO* m_info);

/*
decription: this function create by malloc LINES object and init it and return the pointer object
args:
char line[MAX_LINE]: the lines to copy to object
return: pointer to LINES object
*/
LINES* create_lines(char line[MAX_LINE]);

/*
decription: this function append LINES object new line
args:
LINES* lines: the LINES object that the new line will be append to
char line[MAX_LINE]: the new line to copy
return: None
*/
void append_to_lines(LINES* lines, char line[MAX_LINE]);


/*
decription: this function append MACRO_INFO object to MACRO_TABLE object
args:
MACRO_TABLE* m_table: the table that the new macro info will be appened
MACRO_INFO* m_info: the new data that will be copyed
return: None
*/
void append_to_macro_table(MACRO_TABLE* m_table, MACRO_INFO* m_info);


/*
decription: this function get the LINES OJBECT by macro name
args:
MACRO_TABLE* m_table: the table have all the macro decleartion
char macro_name[MAX_MACRO_NAME]: the macro name to compare
return:
LINES*: zero if didnt find the macro (should not happen) otherwise the LINES OBJECT
*/
LINES* get_lines_by_macro_name(MACRO_TABLE* m_table, char macro_name[MAX_MACRO_NAME]);


/*
decription: this function used when we found a macro usage and we wantto append all the lines body of the macro
args:
FILE* fp: the file to append the lines
LINES *lines: the lines to copy to the file
return: None
*/
void append_lines_to_file(FILE* fp, LINES* lines);

/*
decription: this function free all of the memory inside the LINES
args:
LINES* lines: the ojbect that need to be free from memory
return: None
*/
void clean_up_lines(LINES* lines);

/*
decription: this function free all of the memory inside the MACRO_TABLE
args:
MACRO_TABLE* m_table: the ojbect that need to be free from memory
return: None
*/
void clean_up_macro_table(MACRO_TABLE* m_table);


#endif 