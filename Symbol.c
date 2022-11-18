#include "Symbol.h"
#include "Helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Symbol createSymbol(const char *name, const char *binaryData, const int address, const bool verbose)
{
    char *symbolName = calloc(strlen(name) + 1, sizeof(char));
    strcpy(symbolName, name);
    char *symbolValue = calloc(strlen(binaryData) + 1, sizeof(char));
    strcpy(symbolValue, binaryData);
    int size = strlen(symbolValue);
    Symbol symbol = {
        symbolName,
        symbolValue,
        address,
        size};
    if (verbose)
    {
        printf("Created symbol:\n\tSymbol name: %s\n\tBinary data: %s\n\tHex address: 0x%x\n\tAddress: %d\n\tSize: %d\n", symbolName, symbolValue, address, address, size);
    }
    return symbol;
}

void freeSymbolData(Symbol *symbol, const bool verbose)
{
    if (verbose)
    {
        printf("Freeing data from symbol: %s...\n", symbol->name);
    }
    free(symbol->name);
    free(symbol->binaryData);
}