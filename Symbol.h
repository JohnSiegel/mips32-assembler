#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdbool.h>

struct _Symbol
{
    char *name;
    char *binaryData;
    int address;
    int size;
};

/**
 * Represents a symbol in the symbol table
 */
typedef struct _Symbol Symbol;

/**
 * Create a new symbol
 */
Symbol createSymbol(const char *name, const char *binaryData, const int address, const bool verbose);

/**
 * Frees the memory from a symbol
 */
void freeSymbolData(Symbol *symbol, const bool verbose);

#endif