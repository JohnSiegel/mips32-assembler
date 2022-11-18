#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

#define BASE_CAPACITY 100

void freeSymbolTableData(SymbolTable *table, const bool verbose)
{
    for (int i = 0; i < table->numSymbols; i++)
    {
        freeSymbolData(&table->symbols[i], verbose);
    }
    free(table->symbols);
    if (verbose)
    {
        printf("Freed symbol table\n");
    }
}

Symbol *findSymbolByName(SymbolTable *table, char *name)
{
    for (int i = 0; i < table->numSymbols; i++)
    {
        if (!strcmp(table->symbols[i].name, name))
        {
            return table->symbols + i;
        }
    }
    return NULL;
}

void addSymbolToTable(SymbolTable *table, Symbol symbol, const bool verbose)
{
    table->symbols[table->numSymbols] = symbol;
    table->numSymbols++;

    if (table->numSymbols >= table->capacity)
    {
        if (verbose)
        {
            printf("Increasing symbol table capacity to: %d\n", table->capacity * 2);
        }
        table->capacity *= 2;
        table->symbols = realloc(table->symbols, table->capacity * sizeof(Symbol));
    }
}

SymbolTable createSymbolTable(const bool verbose)
{
    int capacity = BASE_CAPACITY;
    if (verbose)
    {
        printf("Creating a symbol table with capacity: %d\n", capacity);
    }
    SymbolTable table = {
        calloc(capacity, sizeof(Symbol)),
        0, capacity};
    return table;
}