#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "Symbol.h"
#include <stdio.h>
#include <stdbool.h>

struct _SymbolTable
{
    Symbol *symbols;
    int numSymbols;
    int capacity;
};

/**
 * Represents a table of symbols
 */
typedef struct _SymbolTable SymbolTable;

/**
 * Frees the data from a table of symbols
 */
void freeSymbolTableData(SymbolTable *table, const bool verbose);

/**
 * Adds a symbol to the table
 */
void addSymbolToTable(SymbolTable *table, Symbol symbol, const bool verbose);

/**
 * Finds a symbol by its name
 */
Symbol *findSymbolByName(SymbolTable *table, char *name);

/**
 * Creates a new symbol table
 */
SymbolTable createSymbolTable(const bool verbose);

#endif