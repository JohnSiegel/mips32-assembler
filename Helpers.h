#ifndef HELPERS_H
#define HELPERS_H
#include <stdbool.h>

/**
 * Translates an asciiz string into binary
 */
char *stringToBinary(char *value);

/**
 * Checks if a string contains characters that are not whitespace
 */
bool isWhitespace(char *value);

/**
 * Returns last index of x if it is present.
 * Else returns -1.
 */
int findLastIndexOfChar(char *value, char x);

/**
 * Gets the 2s complement representation of an int
 */
char *TwosComplement(int value, int bits);

#endif