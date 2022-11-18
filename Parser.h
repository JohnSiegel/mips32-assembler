#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "SymbolTable.h"
#include "LabelTable.h"

/**
 * Parses an input array into either a symbols table or assembled binary text
 */
char *parse(char **input, const int numLines, const bool verbose, const bool symbols);

/**
 * Parse an individual assembly instruction from the text segment
 */
char *parseInstruction(char **tokens, SymbolTable *symbolTable, LabelTable *labelTable, const int numTokens, const int address, const bool verbose);

/**
 * Generates the final output string for the parsed assembly program
 */
char *createOutput(char **textBinaryOutput, int numTextBinaryLines, SymbolTable *symbolTable, LabelTable *labelTable, const bool symbols, const bool verbose);

/**
 * Scans an assembly instruction for labels and adds them to the table if found. Returns true if a label was found
 */
bool scanLabels(char *input, LabelTable *table, const int instructionAddress, const bool verbose);

#endif