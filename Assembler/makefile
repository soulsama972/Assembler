FLAGS=  -g -Wall -ansi -pedantic
assembler: preprocessing.o parser.o main.o io.o symbol_table.o code_table.o data_table.o
	gcc $(FLAGS) preprocessing.o parser.o main.o io.o symbol_table.o code_table.o data_table.o -o assembler

praser.o: praser.c praser.h
	gcc -c $(FLAGS) praser.c -o praser.o

io.o: io.c io.h
	gcc -c $(FLAGS) io.c -o io.o

main.o: main.c 
	gcc -c $(FLAGS) main.c -o main.o

preprocessing.o: preprocessing.c preprocessing.h 
	gcc -c $(FLAGS) preprocessing.c -o preprocessing.o

symbol_table.o: symbol_table.c symbol_table.h
	gcc -c $(FLAGS) symbol_table.c -o symbol_table.o

code_table.o: code_table.c code_table.h
	gcc -c $(FLAGS) code_table.c -o code_table.o

data_table.o: data_table.c data_table.h
	gcc -c $(FLAGS) data_table.c -o data_table.o









