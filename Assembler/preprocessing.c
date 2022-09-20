#include "preprocessing.h"

/*
decription: this function check if the token is saved token
args:
char * tok: the token to check if its saved token
return:
int: TRUE token is save token else FALSE
*/
int is_saved_token(char* tok)
{
	/*check if the word saved oparend*/
	if (!strcmp(tok, "mov") || !strcmp(tok, "cmp") ||
		!strcmp(tok, "sub") || !strcmp(tok, "lea") ||
		!strcmp(tok, "clr") || !strcmp(tok, "not") ||
		!strcmp(tok, "inc") || !strcmp(tok, "dec") ||
		!strcmp(tok, "jmp") || !strcmp(tok, "bne") ||
		!strcmp(tok, "jsr") || !strcmp(tok, "red") ||
		!strcmp(tok, "prn") || !strcmp(tok, "rts") ||
		!strcmp(tok, "stop") || !strcmp(tok, "add") ||
		!strcmp(tok, ".extern") || !strcmp(tok, ".entry") ||
		!strcmp(tok, ".string") || !strcmp(tok, ".data"))
		return TRUE;
	/*check if the word is r0-r15*/
	if ((strlen(tok) == 2 && tok[0] == 'r' && tok[1] >= '0' && tok[1] <= '9') || (strlen(tok) == 3 && tok[0] == 'r' && tok[1] == '1' && tok[2] >= '0' && tok[2] <= '5'))
		return TRUE;
	return FALSE;
}

/*
decription: this function doing the preprossing to the orginal file with the marco definiton
args:
char* file_orginal_name: the file_orginal_name for exmaple "code.as"
char* file_without_macro_name: the file_without_macro_name for exmaple "code.am"
return:
int TRUE if createing the new file was sccssufuly otherwise FALSE
*/
int create_file_without_macro(char* file_orginal_name, char* file_without_macro_name)
{
	FILE* fp_orignal = 0, *fp_without_macro = 0;
	MACRO_TABLE *m_table = 0;
	LINES* lines = 0;
	char line[MAX_LINE];
	char temp_copy_line_for_strtok[MAX_LINE];
	char temp_macro_name[MAX_MACRO_NAME];
	char* token;
	char macro_found = FALSE;
	int found = FALSE;

	fp_orignal = fopen(file_orginal_name, "r");
	fp_without_macro = fopen(file_without_macro_name, "w");
	RETURN_IF_FAILED_TO_OPEN_FILE_AND_PRINT_ERROR(fp_orignal, file_orginal_name);
	if (fp_without_macro == 0)
	{
		fprintf(stdout, "cannot create file named %s\n", file_without_macro_name);
		fclose(fp_orignal);
		return FALSE;
	}

	/*get a line each iterate till there is no more lines in the file*/
	while ((fgets(line, MAX_LINE, fp_orignal)) != NULL)
	{
		strcpy(temp_copy_line_for_strtok, line);
		token = strtok(temp_copy_line_for_strtok, " \t\r\n");
		if (token[0] == ';') /*comment we dont care we move the next line after append to the new file*/
		{
			fprintf(fp_without_macro, "%s", line);
			continue;
		}
		else if (!strcmp(token, "macro")) 
		{
			token = strtok(0, " \r\n\t");
			macro_found = TRUE;
			strcpy(temp_macro_name, token);
		}
		else if (!strcmp( token, "endm"))
		{
			if (!is_saved_token(temp_macro_name))
			{
				if (!m_table) /*if its the first time we create macro_table with the first marco info*/
					m_table = create_macro_table(create_macro_info(temp_macro_name, lines));
				else /*otherwise we append to next macro*/
					append_to_macro_table(m_table,create_macro_info(temp_macro_name, lines));
			}
			macro_found = FALSE;
		}
		else if (macro_found)
		{
			if (!lines)
				lines = create_lines(line);  /*first line of the marco to save*/
			else
				append_to_lines(lines, line); /*other lines*/
		}
		else
		{
			if (m_table)
			{
				found = FALSE;
				while (token)
				{
					lines = get_lines_by_macro_name(m_table, token); /*get all the lines by the marco name*/
					if (lines)
					{
						found = TRUE;
						append_lines_to_file(fp_without_macro, lines);/*append the lines to the file without macro*/
						token = strtok(0, " ");
						while (token)
						{
							fprintf(fp_without_macro, "%s", token);
							token = strtok(0, " ");
						} 
						break;
					}
					else
						token = strtok(0, " ");
				}
				if(!token && !found)
					fprintf(fp_without_macro, "%s", line);
			}
			else
				fprintf(fp_without_macro, "%s", line);
		}
	}
	fclose(fp_orignal);
	fclose(fp_without_macro);
	clean_up_macro_table(m_table);
	return TRUE;
}

/*
decription: this function create MACRO_INFO OJBECT and return it
args:
char macro_name[MAX_MACRO_NAME]: the macro name
LINES* lines: the lines that represent body of the marco
return:
MACRO_INFO*: MARCO_INFO POINTER
*/
MACRO_INFO* create_macro_info(char macro_name[MAX_MACRO_NAME], LINES* lines)
{
	MACRO_INFO* m_info;
	m_info = malloc(sizeof(MACRO_INFO));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(m_info);
	strcpy(m_info->macro_name, macro_name);
	m_info->lines = lines;
	return m_info;
}

/*
decription: this function create by malloc MACRO_TABLE object and init it and return the pointer object
args:
MACRO_INFO* m_info: the macro info that we set the the first macro table
return: pointer to MACRO_TABLE object
*/
MACRO_TABLE* create_macro_table(MACRO_INFO* m_info)
{
	MACRO_TABLE* m_table;
	m_table = malloc(sizeof(MACRO_TABLE));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(m_table);
	m_table->m_info = m_info;
	m_table->next = 0;
	return m_table;
}

/*
decription: this function create by malloc LINES object and init it and return the pointer object
args:
char line[MAX_LINE]: the lines to copy to object
return: pointer to LINES object
*/
LINES* create_lines(char line[MAX_LINE])
{
	LINES* lines;
	lines = malloc(sizeof(LINES));
	EXIT_IF_FAILED_ALLOC_AND_PRINT_ERROR(lines);
	strcpy(lines->line, line);
	lines->next = 0;
	return lines;
}

/*
decription: this function append LINES object new line
args:
LINES* lines: the LINES object that the new line will be append to
char line[MAX_LINE]: the new line to copy
return: None
*/
void append_to_lines(LINES* lines, char line[MAX_LINE])
{
	LINES* lines_to_be_append;
	LINES* temp;
	lines_to_be_append = create_lines(line);
	temp = lines;
	while (temp->next != 0)
		temp = temp->next;
	temp->next = lines_to_be_append;
}

/*
decription: this function append MACRO_INFO object to MACRO_TABLE object
args:
MACRO_TABLE* m_table: the table that the new macro info will be appened
MACRO_INFO* m_info: the new data that will be copyed
return: None
*/
void append_to_macro_table(MACRO_TABLE* m_table, MACRO_INFO* m_info)
{
	MACRO_TABLE* table_to_be_append;
	MACRO_TABLE* temp;
	table_to_be_append =create_macro_table(m_info);
	temp = m_table;
	while (temp->next != 0)
		temp = temp->next;
	temp->next = table_to_be_append;
}

/*
decription: this function get the LINES OJBECT by macro name
args:
MACRO_TABLE* m_table: the table have all the macro decleartion
char macro_name[MAX_MACRO_NAME]: the macro name to compare
return:
LINES*: zero if didnt find the macro (should not happen) otherwise the LINES OBJECT
*/
LINES* get_lines_by_macro_name(MACRO_TABLE* m_table, char macro_name[MAX_MACRO_NAME])
{
	MACRO_TABLE* temp = m_table;
	while (temp)
	{
		if (!strcmp(temp->m_info->macro_name, macro_name))
			return temp->m_info->lines;
		temp = temp->next;
	}
	return 0;
}

/*
decription: this function used when we found a macro usage and we wantto append all the lines body of the macro
args:
FILE* fp: the file to append the lines
LINES *lines: the lines to copy to the file
return: None
*/
void append_lines_to_file(FILE* fp, LINES *lines)
{
	LINES* temp = lines;
	while (temp)
	{
		fprintf(fp, "%s", temp->line);
		temp = temp->next;
	}
}


/*
decription: this function free all of the memory inside the LINES
args:
LINES* lines: the ojbect that need to be free from memory
return: None
*/
void clean_up_lines(LINES* lines)
{
	LINES* temp = lines;
	while (lines)
	{
		lines = lines->next;
		free(temp);
		temp = lines;
	}
}


/*
decription: this function free all of the memory inside the MACRO_TABLE
args:
MACRO_TABLE* m_table: the ojbect that need to be free from memory
return: None
*/
void clean_up_macro_table(MACRO_TABLE* m_table)
{
	MACRO_TABLE* temp_table;
	while (m_table)
	{
		temp_table = m_table;
		clean_up_lines(temp_table->m_info->lines);
		free(temp_table->m_info);
		m_table = m_table->next;
	}
}