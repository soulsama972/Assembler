#include"parser.h"

int line_number; /*which line we current are*/
int found_error; /*flag to tell if we found error in the file for knowing if to save the files or not*/
int first_run;/*flag to tell if we in the first run of the analyze file*/
char* pattern = " ,\r\n"; /*pattern to split the lines to tokens*/

extern int is_saved_token(char* tok); /*declear in preprocessing.h*/

/*
decription: this function check if the string is a command token
args:
char* command: the string to compeare to command string
return:
TRUE if the token is string is a command else FALSE
*/
int is_command(char* command)
{
	if (!strcmp(command, "mov") || !strcmp(command, "cmp") ||
		!strcmp(command, "sub") || !strcmp(command, "lea") ||
		!strcmp(command, "clr") || !strcmp(command, "not") ||
		!strcmp(command, "inc") || !strcmp(command, "dec") ||
		!strcmp(command, "jmp") || !strcmp(command, "bne") ||
		!strcmp(command, "jsr") || !strcmp(command, "red") ||
		!strcmp(command, "prn") || !strcmp(command, "rts") ||
		!strcmp(command, "stop") || !strcmp(command, "add"))
		return TRUE;
	return FALSE;
}

/*
decription: this function check if the string is a directive token
args:
char* directive: the string to compeare to directive string
return:
TRUE if the token is string is a directive else FALSE
*/
int is_directive(char* directive)
{
	if (!strcmp(directive, ".extern") || !strcmp(directive, ".entry") ||
		!strcmp(directive, ".string") || !strcmp(directive, ".data"))
		return TRUE;
	return FALSE;
}

/*
decription: this function return directive_enum  according to the input
args:
char* directive the string to compeare and get the enum type
return: int
*/
int get_directive(char* directive)
{
	if (!strcmp(directive, ".extern"))
		return EXTERN;
	if (!strcmp(directive, ".entry"))
		return ENTRY;
	if (!strcmp(directive, ".string"))
		return STRING;
	if (!strcmp(directive, ".data"))
		return DATA;
	return NOT_DIRECTIVE;
}

/*
decription: this function check if the symbol is valid according to the given task
args:
SYMBOL_TABLE* symbol_table:used for valiation (if the smybol already exist)
char* symbol_name: the symbol to check if valid
int check_if_exsit: used as boolean used for ignore spical case 
return: TRUE if valid other wise FALSE
*/
int is_valid_symbol(SYMBOL_TABLE* symbol_table,char* symbol_name, int check_if_exsit)
{
	unsigned int symbol_length = 0;
	unsigned int i;
	/*if the symbol is empty*/
	if (!symbol_name)
	{
		fprintf(stdout, "not found symbol name line:%d\n", line_number);
		found_error = TRUE;
		return FALSE;
	}
	symbol_length = strlen(symbol_name);
	/*if the smybol is saved token*/
	if (is_saved_token(symbol_name))
	{
		fprintf(stdout, "symbol name used saved token line:%d\n", line_number);
		found_error = TRUE;
		return FALSE;
	}
	if (symbol_length > MAX_SYMBOL_NAME)
	{
		fprintf(stdout, "symbol name size longer than 31 line:%d\n", line_number);
		found_error = TRUE;
	}
	/*if didnt start with alphabet */
	if(symbol_length > 1 && !isalpha(symbol_name[0]))
	{
		fprintf(stdout, "symbol name not start in alphabetical line:%d\n", line_number);
		found_error = TRUE;
		return FALSE;
	}
	/*if symbol already exist */
	if (check_if_exsit && is_symbol_already_exist(symbol_table, symbol_name))
	{
		fprintf(stdout, "symbol name already exist  line:%d\n", line_number);
		found_error = TRUE;
		return FALSE;
	}
	/*if symbol contain valid charactar */
	for (i = 1; i < symbol_length; i++)
	{
		if (!isalpha(symbol_name[i]) && !isdigit(symbol_name[i]))
		{
			fprintf(stdout, "symbol name can only contain alphabet and numerical number line:%d\n", line_number);
			found_error = TRUE;
			return FALSE;
		}
	}
	/*vaild symbol*/
	return TRUE;
}

/*
decription: this function fix code table and the symbol table (used in the secound run)
args:
CODE_TABLE* code_table: the code table used to fix the code section
SYMBOL_TABLE* symbol_table: the symbol_table to fix the entry lines
SYMBOL_TABLE* ext_symbol_table: the extranal_symbol_table used for saving to file all the ext lines
char* tok: the token string
return: None
*/
void fix_code_and_symbol_table(CODE_TABLE* code_table,SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table,char* tok)
{
	SYMBOL* pointer_symbol = get_symbol(symbol_table, tok);
	unsigned int address = 0;
	WORD word = { 0 };
	if (pointer_symbol)
	{
		if (pointer_symbol->is_external)
		{
			word.index_address.E = 1;
			address = code_table->IC;
			append_word_to_code_table(code_table, word);
			append_word_to_code_table(code_table, word);
			append_to_symbol_table(ext_symbol_table, pointer_symbol->symbol_name, address, TRUE, FALSE, pointer_symbol->is_code);
		}
		else
		{
			word.index_address.R = 1;
			word.index_address.address = pointer_symbol->address - pointer_symbol->address % 16;
			append_word_to_code_table(code_table, word);
			word.index_address.address = pointer_symbol->address % 16;
			append_word_to_code_table(code_table, word);
		}
	}
	else
	{
		found_error = TRUE;
		fprintf(stdout, "symbol was not found at symbol table line:%d\n", line_number);
	}
}

/*
decription: this function convert froms sring to int if valid
args:
char* tok: the string token to be converted
int * valid: used to check if the string was valid for convertion 1 yes 0 no
return:
int: the number that converted from string to int
*/
int get_number_from_token(char* tok, int * valid)
{
	int multipler = 0;
	unsigned int i = 0;
	unsigned int length = strlen(tok);
	int found_nagtive_or_addtion = FALSE;
	if (length > 0)
	{
		/*if we start with - or +*/
		if (tok[0] == '-')
		{
			i = 1;
			multipler = -1;
			found_nagtive_or_addtion = TRUE;
		}
		if (tok[0] == '+')
		{
			i = 1;
			multipler = 1;
			found_nagtive_or_addtion = TRUE;
		}
	}
	/*check if all the charcatars are numbers*/
	for (; i < length; i++)
	{
		if (!isalnum(tok[i]))
		{
			found_error = TRUE;
			*valid = 0;
			return 0;
		}
	}
	*valid = 1; /*valid number*/
	if (found_nagtive_or_addtion)
		return atoi(tok + 1) * multipler; /*if start with neagtive or addtion*/
	else
		return atoi(tok); /*if normal number*/
}

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
void get_command_info(char* command, int* how_many_operand, int* opcode, int* funct, int* src_addressing_mode, int* dst_addressing_mode)
{
	int all_mode = IMMEDIATE_ADDRESSING_MODE | DIRECT_ADDRESSING_MODE | INDEX_ADDRESSING_MODE | REGISTER_ADDRESING_MODE;
	int all_mode_without_immediate = DIRECT_ADDRESSING_MODE | INDEX_ADDRESSING_MODE | REGISTER_ADDRESING_MODE;
	int only_direct_and_index_addressing = DIRECT_ADDRESSING_MODE | INDEX_ADDRESSING_MODE;
	/*for our demo computer we have only 4 access mode so i declear them above for nice visualzition*/
	if (!strcmp(command, "mov"))
	{
		*opcode = 0;
		*funct = 0;
		*how_many_operand = 2;
		*src_addressing_mode = all_mode;
		*dst_addressing_mode = all_mode_without_immediate;

	}
	else if (!strcmp(command, "cmp"))
	{
		*opcode = 1 << 1;
		*funct = 0;
		*how_many_operand = 2;
		*src_addressing_mode = all_mode;
		*dst_addressing_mode = all_mode;
	}
	else if (!strcmp(command, "add"))
	{
		*funct = 10;
		*opcode = 1 << 2;
		*how_many_operand = 2;
		*src_addressing_mode = all_mode;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "sub"))
	{
		*opcode = 1 << 2;
		*funct = 10;
		*how_many_operand = 2;
		*src_addressing_mode = all_mode;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "lea"))
	{
		*opcode = 1 << 4;
		*funct = 0;
		*how_many_operand = 2;
		*src_addressing_mode = only_direct_and_index_addressing;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "clr"))
	{
		*opcode = 1 << 5;
		*funct = 10;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "not"))
	{
		*opcode = 1 << 5;
		*funct = 11;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "inc"))
	{
		*opcode = 1 << 5;
		*funct = 12;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "dec"))
	{
		*opcode = 1 << 5;
		*funct = 13;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode_without_immediate;

	}
	else if (!strcmp(command, "jmp"))
	{
		*opcode = 1 << 9;
		*funct = 10;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = only_direct_and_index_addressing;
	}
	else if (!strcmp(command, "bne"))
	{
		*opcode = 1 << 9;
		*funct = 11;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = only_direct_and_index_addressing;
	}
	else if (!strcmp(command, "jsr"))
	{
		*opcode = 1 << 9;
		*funct = 12;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = only_direct_and_index_addressing;
	}
	else if (!strcmp(command, "red"))
	{
		*opcode = 1 << 12;
		*funct = 0;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode_without_immediate;
	}
	else if (!strcmp(command, "prn"))
	{
		*opcode = 1 << 13;
		*funct = 0;
		*how_many_operand = 1;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = all_mode;
	}
	else if (!strcmp(command, "rts"))
	{
		*opcode = 1 << 14;
		*funct = 0;
		*how_many_operand = 0;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = NO_ADDRESSING_MODE;
	}
	else if (!strcmp(command, "stop"))
	{
		*opcode = 1 << 15;
		*funct = 0;
		*how_many_operand = 0;
		*src_addressing_mode = NO_ADDRESSING_MODE;
		*dst_addressing_mode = NO_ADDRESSING_MODE;
	}
}

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
void operand_state(CODE_TABLE* code_table,SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table,char* tok, WORD* word, int addressing_mode, int first_operand)
{
	int valid;
	int reg_number;
	int immediate_number;
	unsigned int length_of_token;
	long long index_of_open_bracket;
	char* pointer_to_open_bracket = 0;
	WORD word_to_be_append = { 0 };
	if (!tok)
	{
		if (first_operand)
			fprintf(stdout, "missing first operand line:%d\n", line_number);
		else
			fprintf(stdout, "missing second operand line:%d\n", line_number);
		found_error = TRUE;
		return;
	}
	
	length_of_token = strlen(tok);

	if (addressing_mode & IMMEDIATE_ADDRESSING_MODE && length_of_token > 1)
	{
		if (tok[0] == '#')
		{
			immediate_number = get_number_from_token(tok + 1, &valid);
			if (valid)
			{
				if (!first_run)
				{
					code_table->IC++;
					return;
				}
				if (first_operand)
					word->command_extra_info.src_reg = 0;
				else
					word->command_extra_info.dst_reg = 0;
				word_to_be_append.immediate_address.A = 1;
				word_to_be_append.immediate_address.value = immediate_number;
				append_word_to_code_table(code_table, word_to_be_append);
				return;
			}
		}
	}
	if (addressing_mode & REGISTER_ADDRESING_MODE)
	{
		/*check if the token is r0-r15*/
		if ((strlen(tok) == 2 && tok[0] == 'r' && tok[1] >= '0' && tok[1] <= '9') || (strlen(tok) == 3 && tok[0] == 'r' && tok[1] == '1' && tok[2] >= '0' && tok[2] <= '5'))
		{
			if (!first_run)
				return;
			reg_number = atoi(tok + 1);
			if (first_operand)
			{
				word->command_extra_info.src_reg = reg_number;
				word->command_extra_info.src_address = 3;
			}
			else
			{
				word->command_extra_info.dst_reg = reg_number;
				word->command_extra_info.dst_address = 3;
			}
			return;
		}
	}
	if (addressing_mode & INDEX_ADDRESSING_MODE)
	{
		pointer_to_open_bracket = strchr(tok, '['); 
		if (pointer_to_open_bracket && tok[length_of_token -1] == ']') /*check whatever or not we have open bracket and close bracket*/
		{
			index_of_open_bracket = pointer_to_open_bracket - tok; /*calcalute the index of the open bracket*/
			pointer_to_open_bracket++; /*move the pointer by one so we start at the char after the open barckt*/
			tok[index_of_open_bracket] = 0; /*we spilt the words into two tokens label and register*/
			tok[length_of_token - 1] = 0; /*we spilt the words into two tokens label and register*/
			/*check if the token is r10-r15*/
			if (strlen(pointer_to_open_bracket) == 3 && pointer_to_open_bracket[0] == 'r' 
				&& pointer_to_open_bracket[1] == '1' && pointer_to_open_bracket[2] >= '0' && pointer_to_open_bracket[2] <= '5') 
			{
				if(is_valid_symbol(symbol_table,tok,FALSE)) /*check whatever or not the symbol is valid*/
				{
					if (first_run)
					{
						reg_number = atoi(pointer_to_open_bracket + 1);
						if (first_operand)
						{
							word->command_extra_info.src_reg = reg_number;
							word->command_extra_info.src_address = 2;
						}
						else
						{
							word->command_extra_info.dst_reg = reg_number;
							word->command_extra_info.dst_address = 2;
						}
						code_table->IC += 2;
					}
					else
						fix_code_and_symbol_table(code_table,symbol_table,ext_symbol_table,tok);
					return;
				}
			}
			else
			{
				fprintf(stdout, "invalid opration can only use r10-15 line:%d\n", line_number);
				found_error = TRUE;
				return;
			}
		}
	}
	if (addressing_mode & DIRECT_ADDRESSING_MODE)
	{
		if (is_valid_symbol(symbol_table,tok, FALSE))
		{
			if (first_run)
			{
				if (first_operand)
					word->command_extra_info.src_address = 1;
				else
					word->command_extra_info.dst_address = 1;
				code_table->IC += 2;
			}
			else
				fix_code_and_symbol_table(code_table,symbol_table,ext_symbol_table,tok);
			return;
		}
	}
	fprintf(stdout, "invalid addressing mode line%d\n", line_number);
	found_error = TRUE;

}

/*
decription: this function used to update the tables according directive state
args:
DATA_TABLE* data_table: the table to fix or append new info
SYMBOL_TABLE* symbol_table: the table to fix or append new info
char* tok: the string token
char* symbol_name the symbol name
return: None
*/
void directive_state(DATA_TABLE* data_table, SYMBOL_TABLE* symbol_table,char* tok, char* symbol_name)
{
	SYMBOL_TABLE* temp;
	WORD word;
	int temp_counter;
	int valid;
	unsigned int length_of_token;
	unsigned int i;
	switch (get_directive(tok))
	{
	case EXTERN:
	{
		if (!first_run)
			return;
		tok = strtok(NULL, pattern);
		if (is_valid_symbol(symbol_table,tok,TRUE))
			append_to_symbol_table(symbol_table, tok, 0, TRUE, FALSE, FALSE);
	}break;
	case ENTRY:
	{
		if (first_run)
			return;
		tok = strtok(0, pattern);
		temp = symbol_table;
		while (temp->next)
		{
			if (!strcmp(temp->symbol.symbol_name, tok))
			{
				temp->symbol.is_entry = TRUE;
				return;
			}
			temp = temp->next;
		}
		fprintf(stdout, ".entry was found without decleartion of its symbol line:%d\n", line_number);
		found_error = TRUE;
		return;
	}break;
	case DATA:
	{
		if (!first_run)
			return;
		temp_counter = data_table->DC;
		valid = TRUE;
		tok = strtok(NULL, pattern);
		if (!tok)
		{
			fprintf(stdout, ".data was found without data line:%d\n", line_number);
			found_error = TRUE;
			return;
		}
		while (tok)
		{
			memset(&word, 0, sizeof(WORD));
			word.immediate_address.value = get_number_from_token(tok, &valid);
			word.immediate_address.A = 1;
			if (valid)
				append_word_to_data_table(data_table, word);
			else
			{
				fprintf(stdout, "invalid number line:%d\n", line_number);
				found_error = TRUE;
				data_table->DC = temp_counter;
				return;
			}
			tok = strtok(0, pattern);
		}
		if (symbol_name)
			append_to_symbol_table(symbol_table, symbol_name, temp_counter, FALSE, FALSE, FALSE);

	}break;
	case STRING:
	{
		if (!first_run)
			return;
		tok = strtok(NULL, pattern);
		if (!tok)
		{
			fprintf(stdout, ".string was found without any string line:%d\n", line_number);
			found_error = TRUE;
			return;
		}
		temp_counter = data_table->DC;
		while (tok)
		{
			length_of_token = strlen(tok);
			if (length_of_token > 0 && tok[0] != '\"')
			{
				fprintf(stdout, "string didnt start with quote line:%d\n", line_number);
				found_error = TRUE;
				data_table->DC = temp_counter;
				return;
			}
			for (i = 1; i < length_of_token; i++)
			{
				memset(&word, 0, sizeof(WORD));
				if(i < length_of_token - 1)
					word.letter.ascii_value = tok[i];
				else
					word.letter.ascii_value = 0;
				word.immediate_address.A = 1;
				append_word_to_data_table(data_table, word);

			}
			if (length_of_token > 0 && tok[length_of_token-1] != '\"')
			{
				fprintf(stdout, "string didnt end with quote line:%d\n", line_number);
				found_error = TRUE;
				data_table->DC = temp_counter;
				return;
			}

			tok = strtok(0, pattern);
		}
		if(symbol_name)
			append_to_symbol_table(symbol_table, symbol_name, temp_counter, FALSE, FALSE, FALSE);
	}break;
	default:
		break;
	}
}

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
void command_state(CODE_TABLE* code_table,SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table, char* tok, char* symbol_name)
{
	WORD command = { 0 };
	WORD *extra_info_for_opcode = 0;
	int how_many_operand = INVALID_COMMAND;
	int opcode = 0;
	int funct = 0;
	int src_addressing_mode = 0;
	int dst_addressing_mode = 0;
	int temp_counter = code_table->IC;
	get_command_info(tok,&how_many_operand,&opcode,&funct,&src_addressing_mode,&dst_addressing_mode);
	if (how_many_operand == INVALID_COMMAND)
	{
		fprintf(stdout, "invalid command line:%d\n", line_number);
		found_error = TRUE;
		return;
	}
	command.command.A = 1;
	command.command.opcode = opcode;
	append_word_to_code_table(code_table, command);
	if (how_many_operand > 0)
	{
		extra_info_for_opcode = &code_table->words[code_table->IC];
		extra_info_for_opcode->command_extra_info.funct = funct;
		extra_info_for_opcode->command_extra_info.A = 1;
		code_table->IC++;
	}
	if (how_many_operand == 1)
	{
		if(src_addressing_mode)
			operand_state(code_table, symbol_table,ext_symbol_table,strtok(0, pattern), extra_info_for_opcode, src_addressing_mode, TRUE);
		else
			operand_state(code_table,symbol_table,ext_symbol_table,strtok(0, pattern), extra_info_for_opcode, dst_addressing_mode, FALSE);
	}
	else if (how_many_operand == 2)
	{
		operand_state(code_table, symbol_table,ext_symbol_table, strtok(0, pattern), extra_info_for_opcode, src_addressing_mode, TRUE);
		operand_state(code_table, symbol_table,ext_symbol_table, strtok(0, pattern), extra_info_for_opcode, dst_addressing_mode, FALSE);
	}
	if (strtok(0, pattern))
	{
		fprintf(stdout, "invalid syntex found extra operand line%d\n", line_number);
		found_error = TRUE;
	}
	if (first_run && symbol_name)
		append_to_symbol_table(symbol_table, symbol_name, temp_counter, FALSE, FALSE, TRUE);

}

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
void analyze_tokens(CODE_TABLE* code_table,DATA_TABLE* data_table,SYMBOL_TABLE* symbol_table, SYMBOL_TABLE* ext_symbol_table,char *tokens, char* symbol_name)
{
	unsigned int length_of_token;
	if (tokens)
	{
		length_of_token = strlen(tokens);
		if (is_saved_token(tokens))
		{
			if (is_directive(tokens))
				directive_state(data_table,symbol_table,tokens, symbol_name);
			else if (is_command(tokens))
				command_state(code_table,symbol_table,ext_symbol_table,tokens,symbol_name);
		}
		else if(!symbol_name)
		{
			if (length_of_token > 0 && tokens[length_of_token - 1] == ':')
			{
				tokens[length_of_token - 1] = 0;
				if (is_valid_symbol(symbol_table,tokens, first_run))
				{
					symbol_name = tokens;
					tokens = strtok(NULL, pattern);
					analyze_tokens(code_table,data_table,symbol_table,ext_symbol_table,tokens, symbol_name);
				}
			}
			else
			{
				fprintf(stdout, "symbol didnt contain ':' line:%d\n", line_number);
				found_error = TRUE;
			}
		}
		else
		{
			fprintf(stdout, "invalid syntex line:%d\n", line_number);
			found_error = TRUE;
		}
	}
}


/*
decription: this function parse file according to the task
args:
char* file_name_without_marco: the file to prase
char* file_name: the file_name given by the user (used to save other files .ent .ext etc)
return:
int FALSE if the file didnt open or any otherr file error else TRUE
*/
int prase_file(char* file_name_without_marco, char* file_name)
{
	SYMBOL_TABLE* symbol_table, *ext_symbol_table;
	DATA_TABLE* data_table;
	CODE_TABLE* code_table;
	FILE* fp = NULL;
	char line[MAX_LINE];
	char *symbol_name  = NULL;
	char* tokens = NULL;
	int i, j;
	SYMBOL_TABLE* temp;
	
	/*alloc all needed tables*/
	symbol_table = create_symbol_table();
	ext_symbol_table = create_symbol_table();
	data_table = create_data_table();
	code_table = create_code_table();

	first_run = TRUE; /*set the global var to TRUE*/
	line_number = 1;/*set the global var to 1*/
	found_error = FALSE; /*set the global var to FALSE*/

	/*open the file for reading mode*/
	fp = fopen(file_name_without_marco, "r");
	RETURN_IF_FAILED_TO_OPEN_FILE_AND_PRINT_ERROR(fp, file_name_without_marco);

	/*loop twice the file*/
	for (i = 0; i < 2; i++)
	{
		while ((fgets(line, MAX_LINE, fp)) != NULL)
		{
			tokens = strtok(line, pattern);
			if (tokens)
			{
				if (tokens[0] == ';')
					continue;
				analyze_tokens(code_table,data_table,symbol_table,ext_symbol_table, tokens, symbol_name);
			}
			symbol_name = NULL;
			line_number++;/*increase line number*/
		}
		if (first_run)
		{
			/*reset for the second run and move fix all ent code*/
			temp = symbol_table;
			while (temp->next)
			{
				if (!temp->symbol.is_code && !temp->symbol.is_external)
					temp->symbol.address += code_table->IC;
				temp = temp->next;
			}

			code_table->IC = 100;
			line_number = 1;
			first_run = FALSE;
			rewind(fp);
		}

	}
	for (j = 0; j < data_table->DC; j++)
		append_word_to_code_table(code_table, data_table->words[j]);
	

	if (found_error == FALSE)
	{
		/*save the info if didnt found any error*/
		save_object_file(code_table, data_table, file_name);
		save_entry_file(symbol_table, file_name);
		save_external_file(ext_symbol_table, file_name);
	}
	
	/*free all uneeded tables*/
	free_code_table(code_table);
	free_data_table(data_table);
	free_symbol_table(symbol_table);
	free_symbol_table(ext_symbol_table);

	/*close the file*/
	fclose(fp);
	return TRUE;
}

/*
decription: this function save to file entry code as instructed in the task
args:
SYMBOL_TABLE* ent_symbol_table: the table info will be save to the file
char* file_name:the file name path
return: None
*/
void save_entry_file(SYMBOL_TABLE* ent_symbol_table,char* file_name)
{
	FILE* fp;
	SYMBOL_TABLE* temp = ent_symbol_table;
	fp = open_file(file_name,".ent", "w");
	if (fp == 0)
	{
		fprintf(stdout, "failed to open file %s.ent\n", file_name);
		return;
	}
	while (temp->next)
	{
		if (temp->symbol.is_entry)
			fprintf(fp, "%s,%d,%d\n", temp->symbol.symbol_name, temp->symbol.address - temp->symbol.address % 16, temp->symbol.address % 16);
		temp = temp->next;
	}

	fclose(fp);
}

/*
decription: this function save to file extrnal code as instructed in the task
args:
SYMBOL_TABLE* ext_symbol_table: the table info will be save to the file
char* file_name:the file name path
return: None
*/
void save_external_file(SYMBOL_TABLE* ext_symbol_table,char* file_name)
{
	FILE* fp;
	SYMBOL_TABLE* temp = ext_symbol_table;
	fp = open_file(file_name, ".ext", "w");
	if (fp == 0)
	{
		fprintf(stdout, "failed to open file %s.ext\n", file_name);
		return;
	}
	while (temp->next)
	{
		if (temp->symbol.is_external)
		{
			fprintf(fp, "%s BASE %d\n", temp->symbol.symbol_name, temp->symbol.address);
			fprintf(fp, "%s OFFSET %d\n\n", temp->symbol.symbol_name, temp->symbol.address + 1);
		}
		temp = temp->next;
	}

	fclose(fp);
}

/*
decription: this function save to file the object format as instructed in the task
args:
CODE_TABLE* code_table:in code table there is a union call object format we append it to the file
DATA_TABLE* data_table:used for te count of how many data there is
char* file_name:the file name path
return: None
*/
void save_object_file(CODE_TABLE* code_table,DATA_TABLE* data_table,char* file_name)
{
	FILE* fp;
	int i;
	fp = open_file(file_name,".ob", "w");
	if (fp == 0)
	{
		fprintf(stdout, "failed to open file %s.ob\n", file_name);
		return;
	}

	fprintf(fp,"%d %d\n",code_table->IC - data_table->DC - 100, data_table->DC);
	for (i = 100; i < code_table->IC; i++)
	{
		fprintf(fp, "%d	A%x-B%x-C%x-D%x-E%x\n",i, code_table->words[i].word_object_format.A, code_table->words[i].word_object_format.B,
			code_table->words[i].word_object_format.C, code_table->words[i].word_object_format.D, code_table->words[i].word_object_format.E);
	}
	fclose(fp);
}