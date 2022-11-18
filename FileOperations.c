#include "FileOperations.h"
#include "Helpers.h"
#include <stdlib.h>
#include <string.h>

int ReadFile(FILE *input, const bool verbose, int outputCapacity, char **output)
{
    if (verbose)
    {
        printf("------ READ FILE ------\n");
    }
    // Get number of bytes in the file
    fseek(input, 0L, SEEK_END);
    long numBytes = ftell(input);
    fseek(input, 0L, SEEK_SET);

    // Create one string with all the contents of the file
    char *buffer = calloc(numBytes + 1, sizeof(char));
    fread(buffer, sizeof(char), numBytes, input);

    if (verbose)
    {
        printf("Input file is %ld bytes\n", numBytes);
        printf("Contents of input file:\n%s\n", buffer);
    }

    // Tokenize by newline character to get an array of lines
    const char *delimeter = "\n";
    char *line = strtok(buffer, delimeter);
    int lineCount = 0;
    while (line)
    {
        // If the line is pure whitespace, skip it
        if (!isWhitespace(line))
        {
            // Allocate space for the line and copy it into the output array
            output[lineCount] = calloc(strlen(line) + 1, sizeof(char));
            strcpy(output[lineCount], line);
            if (verbose)
            {
                printf("Read line #%d: %s\n", lineCount, output[lineCount]);
            }

            // Increment the line counter
            lineCount++;

            // Reallocate the output buffer if it exceeds capacity.
            if (lineCount >= outputCapacity)
            {
                outputCapacity *= 2;
                output = realloc(output, outputCapacity);
            }
        }

        // Get the next line
        line = strtok(NULL, delimeter);
    }
    free(buffer);
    return lineCount;
}