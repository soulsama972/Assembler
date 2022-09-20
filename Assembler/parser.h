#include"Header.h"
#include"code_table.h"
#include"data_table.h"
#include"symbol_table.h"
#include"io.h"

#ifndef PARSER_H
#define PARSER_H


enum { NOT_DIRECTIVE, EXTERN, ENTRY, DATA, STRING };
enum command_enum { MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, JSR, RED, PRN, RTS, STOP, INVALID_COMMAND };
enum addressing_mode { NO_ADDRESSING_MODE = 0, IMMEDIATE_ADDRESSING_MODE = 1, DIRECT_ADDRESSING_MODE = 2, INDEX_ADDRESSING_MODE = 4, REGISTER_ADDRESING_MODE = 8 };

/*
decription: this function check if the string is a command token
args:
char* command: the string to compeare to command string
return:
TRUE if the token is string is a command
*/
int is_command(char* command);

/*
decription: this function check if the string is a directive token
args:
char* directive: the string to compeare to directive string
return:
TRUE if the token is string is a directive else FALSE
*/
int is_directive(char* directive);

/*
decription: this function return directive_enum  according to the input
args:
char* directive the string to compeare and get the enum type
return: directive_enum
*/
int get_directive(char* directive);

/*
decription: this function check if the symbol is valid according to the given task
args:
SYMBOL_TABLE* symbol_table:used for valiation (if the smybol already exist)
char* symbol_name: the symbol to check if valid
int check_if_exsit: used as boolean used for ignore spical case
return: TRUE if valid other wise FALSE
*/
int is_valid_symbol(SYMBOL_TABLE* symbol_table, char* symbol_name, int check_if_exsit);

/*
decription: this function fix code table and the symbol table (used in the secound run)
args:
CODE_TABLE* code_table: the code table used to fix the code section
SYMBOL_TABLE* symbol_table: the symbol_table to fix the entry lines
SYMBOL_TABLE* ext_symbol_table: the extranal_symbol_table used for saving to file all the ext lines
char* tok: the token string
return: None
*/
void fix_code_and_symbol_table(CODE_TABLE* code_table, SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table, char* tok);

/*
decription: this function convert froms sring to int if valid
args:
char* tok: the string token to be converted
int * valid: used to check if the string was valid for convertion 1 yes 0 no
return:
int: the number that converted from string to int
*/
int get_number_from_token(char* tok, int* valid);

/*
decription: this function get the command inforamtion and update the pointers pass to the function
args:
char* command: the command to search and update the pointers accoding
int* how_many_operand:pointer used to update the number of operand
int* opcode: pointer used to update the opocde
int* funct: pointer used to update the funct
int* src_addressing_mode: pointer used to update the src addressing mode
int* dst_addressing_mode: pointer used to update the dst addressing mode
return: None
*/
void get_command_info(char* command, int* how_many_operand, int* opcode, int* funct, int* src_addressing_mode, int* dst_addressing_mode);

/*
decription: this function used to update the tables according to he operand state
args:
CODE_TABLE* code_table: used to append or fix the table
SYMBOL_TABLE* symbol_table: used to append or fix the table
SYMBOL_TABLE* ext_symbol_table: used to append or fix the table
char* tok: the string token
WORD* word: used to get the next token from the word
int addressing_mode:
int first_operand: flag used totell if the opearnd is first or not
return: None
*/
void operand_state(CODE_TABLE* code_table, SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table, char* tok, WORD* word, int addressing_mode, int first_operand);

/*
decription: this function used to update the tables according directive state
args:
DATA_TABLE* data_table: the table to fix or append new info
SYMBOL_TABLE* symbol_table: the table to fix or append new info
char* tok: the string token
char* symbol_name the symbol name
return: None
*/
void directive_state(DATA_TABLE* data_table, SYMBOL_TABLE* symbol_table, char* tok, char* symbol_name);

/*
decription: this function used to update the tables according command state
args:
CODE_TABLE* code_table: the table to fix or append new info
SYMBOL_TABLE* symbol_table: the table to fix or append new info
SYMBOL_TABLE* ext_symbol_table: the table for file ext info
char* tok: the string token
char* symbol_name: the symbol name
return: None
*/
void command_state(CODE_TABLE* code_table, SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table, char* tok, char* symbol_name);

/*
decription: this function act as the main body of state machine will go the function accodding to his state
args:
CODE_TABLE* code_table: the table to fix or append new info
DATA_TABLE* data_table: the table to fix or append new info
SYMBOL_TABLE* symbol_table: the table to fix or append new info
SYMBOL_TABLE* ext_symbol_table:  the table for file ext info
char *tokens: the  string tokens
char* symbol_name: the symbol name
return: None
*/
void analyze_tokens(CODE_TABLE* code_table, DATA_TABLE* data_table, SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table, char* tokens, char* symbol_name);

/*
decription: this function parse file according to the task
args:
char* file_name_without_marco: the file to prase
char* file_name: the file_name given by the user (used to save other files .ent .ext etc)
return:
int FALSE if the file didnt open or any otherr file error else TRUE
*/
int prase_file(char* file_name_without_marco, char* file_name);

/*
decription: this function save to file entry code as instructed in the task
args:
SYMBOL_TABLE* ent_symbol_table: the table info will be save to the file
char* file_name:the file name path
return: None
*/
void save_entry_file(SYMBOL_TABLE* ent_symbol_table, char* file_name);

/*
decription: this function save to file extrnal code as instructed in the task
args:
SYMBOL_TABLE* ext_symbol_table: the table info will be save to the file
char* file_name:the file name path
return: None
*/
void save_external_file(SYMBOL_TABLE* ext_symbol_table, char* file_name);

/*
decription: this function save to file the object format as instructed in the task
args:
CODE_TABLE* code_table:in code table there is a union call object format we append it to the file
DATA_TABLE* data_table:used for te count of how many data there is
char* file_name:the file name path
return: None
*/
void save_object_file(CODE_TABLE* code_table, DATA_TABLE* data_table, char* file_name);

#endif 

