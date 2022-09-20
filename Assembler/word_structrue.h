#include"Header.h"
#ifndef WORD_STRUCTRUE_H
#define WORD_STRUCTRUE_H


typedef	union
{
	int unused : 20; /*for the union the max size should be 4 byte long*/

	struct
	{
		int opcode : 16; /* opcode for the command*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}command; /*used to tell which command was request to execute */

	struct
	{
		unsigned int dst_address : 2; /*destination address*/
		unsigned int dst_reg : 4; /*destination register*/
		unsigned int src_address : 2; /*source address*/
		unsigned int src_reg : 4; /*source register*/
		unsigned int funct : 4; /* used to tell which command to used with the same opcode*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}command_extra_info; /*used to access importend info*/

	struct
	{
		int value : 16; /* immediate value*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}immediate_address; /*used for storing the  immediate value and data table*/

	struct
	{
		unsigned int address : 16; /* for offset or base address*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}direct_address; /*used for storing the  direct_address*/

	struct
	{
		unsigned int address : 16; /* for offset or base address*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}index_address; /*used for storing the  index address*/

	struct
	{
		int ascii_value : 8; /*ascii character*/
		int unused : 8; /*not used in data table latter*/
		unsigned int E : 1;/*E FLAG*/
		unsigned int R : 1;/*R FLAG*/
		unsigned int A : 1;/*A FLAG*/
		unsigned int zero : 1; /*bit 19 always zero*/
	}letter; /*used for data table*/

	struct
	{
		unsigned int E : 4;
		unsigned int D : 4;
		unsigned int C : 4;
		unsigned int B : 4;
		unsigned int A : 4;
	}word_object_format; /*used to print the object file*/

}WORD;


#endif