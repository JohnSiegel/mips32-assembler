#include "FileOperations.h"
#include "Parser.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Confirm sufficient number of arguments
    if (argc < 3)
    {
        printf("Invalid number of arguments. Got %d, expected 2\n", argc - 1);
        return -1;
    }

    // Verbose mode will enable additional output for debugging
    bool verbose = false;

    // Symbols mode will have a different output
    bool symbols = false;

    // Get arguments
    if (argc >= 4)
    {
        if (!strcmp(argv[3], "-symbols"))
        {
            symbols = true;
        }
        if (!strcmp(argv[3], "--verbose") || (argc >= 5 && !strcmp(argv[4], "--verbose")))
        {
            verbose = true;
        }
    }

    if (verbose)
    {
        if (symbols)
        {
            printf("Running in symbols mode...\n");
        }
        else
        {
            printf("Running in assembler mode...\n");
        }
    }

    // I/O file paths should be specified in first two the execution arguments
    const char *inputFilePath = argv[1];
    const char *outputFilePath = argv[2];

    // Open the input file, read-only
    FILE *inputFile = fopen(inputFilePath, "r");

    // Check if the input file was successfully opened
    if (inputFile == NULL)
    {
        printf("Could not find input file: %s\n", inputFilePath);
        return -1;
    }

    // Open the output file, write permissions
    FILE *outputFile = fopen(outputFilePath, "w");

    // Check if the file was created successfully
    if (outputFile == NULL)
    {
        printf("Could not create output file: %s\n", outputFilePath);
        return -1;
    }

    // Output input/output file paths if in verbose mode
    if (verbose)
    {
        printf("Input file: %s\n", inputFilePath);
        printf("Output file: %s\n", outputFilePath);
    }

    // Read lines from input file
    char **input = calloc(100, sizeof(char *));
    int numLines = ReadFile(inputFile, verbose, 100 * sizeof(char *), input);
    input = realloc(input, numLines * sizeof(char *));

    // Parse output
    char *output = parse(input, numLines, verbose, symbols);

    // Write output to output file
    fwrite(output, sizeof(char), strlen(output), outputFile);

    // Close input and output files
    fclose(inputFile);
    fclose(outputFile);

    // Free the memory from our input array
    for (int i = 0; i < numLines; i++)
    {
        free(input[i]);
        input[i] = NULL;
    }
    free(input);

    // Free our output array
    free(output);
    return 0;
}