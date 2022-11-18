#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include <stdio.h>
#include <stdbool.h>

/**
 * Reads an input file into an array of lines and returns the number of lines
 */
int ReadFile(FILE *input, const bool verbose, int capacity, char **output);

#endif