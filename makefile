# Specify shell to execute recipes
SHELL=/bin/bash

CC=gcc
CFLAGS=-O0 -std=c99 -Wall -W -ggdb3

assembler: assemble.c FileOperations.o Parser.o
	$(CC) $(CFLAGS) -o assemble assemble.c FileOperations.o Parser.o Symbol.o SymbolTable.o Label.o LabelTable.o Helpers.o Opcode.o Register.o

FileOperations.o: FileOperations.c FileOperations.h
	$(CC) $(CFLAGS) -c FileOperations.c

Parser.o: Parser.c Parser.h SymbolTable.o LabelTable.o Helpers.o Opcode.o Register.o
	$(CC) $(CFLAGS) -c Parser.c

Opcode.o: Opcode.c Opcode.h
	$(CC) $(CFLAGS) -c Opcode.c

Register.o: Register.c Register.h
	$(CC) $(CFLAGS) -c Register.c

SymbolTable.o: SymbolTable.c SymbolTable.h Symbol.o
	$(CC) $(CFLAGS) -c SymbolTable.c

LabelTable.o: LabelTable.c LabelTable.h Label.o
	$(CC) $(CFLAGS) -c LabelTable.c

Symbol.o: Symbol.c Symbol.h
	$(CC) $(CFLAGS) -c Symbol.c

Label.o: Label.c Label.h
	$(CC) $(CFLAGS) -c Label.c

Helpers.o: Helpers.c Helpers.h
	$(CC) $(CFLAGS) -c Helpers.c

clean:
	rm *.o