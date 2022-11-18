#include "Parser.h"
#include "SymbolTable.h"
#include "LabelTable.h"
#include "Helpers.h"
#include "Opcode.h"
#include "Register.h"
#include <string.h>
#include <inttypes.h>

#define LINE_WIDTH 32
#define BASE_INSTR_ADDR 0
#define BASE_DATA_ADDR 8192
#define MAX_DATA_ARRAY_SIZE 200

bool findLabel(char *input, LabelTable *table, const int instructionAddress, const bool verbose)
{
    // Delimeter for tokenizing labels
    const char *delimeter = " \t";

    // Copy the line so we don't alter it
    char *instruction = calloc(strlen(input) + 1, sizeof(char));
    strcpy(instruction, input);

    // Skip parsing if the instruction was just a comment
    if (!isWhitespace(instruction))
    {
        // Get the first token
        char *token = strtok(instruction, delimeter);

        if (token && !isWhitespace(token))
        {
            // Get the first occurrence of ':'
            const size_t endIndex = strcspn(token, ":");

            // If ':' was found, this is a label
            if (endIndex < strlen(token))
            {
                // Allocate a substring up to ':'
                char *labelName = calloc(endIndex + 1, sizeof(char));
                memcpy(labelName, token, endIndex);
                if (verbose)
                {
                    printf("Found label: %s\n", labelName);
                }

                // Add the label to the table and return true
                Label label = createLabel(labelName, instructionAddress, verbose);
                addLabelToTable(table, label, verbose);
                free(labelName);
                free(instruction);
                return true;
            }
        }
    }
    free(instruction);
    return false;
}

char *parse(char **input, const int numLines, const bool verbose, const bool symbols)
{
    // Delimeter for tokenizing
    const char *delimeter = " ,\t():\r";

    // Create a table for holding symbols
    SymbolTable symbolTable = createSymbolTable(verbose);

    // Create a table for holding labels
    LabelTable labelTable = createLabelTable(verbose);

    // Controls whether or not we are currently parsing as the data segment
    bool dataMode = true;

    // Holds the current instruction address
    int instructionAddress = BASE_INSTR_ADDR;

    // Array of lines of binary output for the text segment
    char **textSegment = calloc(numLines, sizeof(char *));
    int numTextOutputLines = 0;

    if (verbose)
    {
        printf("Scanning for labels...\n");
    }

    // Loop through to find all labels
    for (int lineIndex = 0; lineIndex < numLines; lineIndex++)
    {
        // Get the input line
        char *line = input[lineIndex];

        // Copy the line so we don't alter it on tokenization
        char *instruction = calloc(strlen(line) + 1, sizeof(char));
        strcpy(instruction, line);

        // Check for transition to text segment
        char *token = strtok(instruction, delimeter);
        if (token[0] != '#')
        {
            if (strlen(token) >= 5)
            {
                if (!strncmp(token, ".text", 5))
                {
                    dataMode = false;
                    free(instruction);
                    continue;
                }
            }

            // If we are not in the data segment, find a label
            if (!dataMode)
            {
                if (!findLabel(line, &labelTable, instructionAddress, verbose))
                {
                    instructionAddress += LINE_WIDTH;
                }
            }
        }
        free(instruction);
    }

    instructionAddress = BASE_INSTR_ADDR;
    dataMode = true;

    if (verbose)
    {
        printf("Finished scanning labels...\nNow parsing program...\n");
    }

    for (int lineIndex = 0; lineIndex < numLines; lineIndex++)
    {
        // Get the input line
        char *line = input[lineIndex];

        // Find index of the first occurence of '#', or the length of the line if there is no comment
        int commentIndex = strcspn(line, "#");

        // Get the non-commented section of the line
        char *instruction = calloc(commentIndex + 1, sizeof(char));
        strncpy(instruction, line, commentIndex);

        // Skip parsing if the instruction was just a comment
        if (!isWhitespace(instruction))
        {
            if (verbose)
            {
                printf("Parsing instruction: %s\n", instruction);
            }

            // Tokenize
            char *token = strtok(instruction, delimeter);
            char *tokens[MAX_DATA_ARRAY_SIZE];
            int tokenCount = 0;
            while (token && !isWhitespace(token))
            {
                tokens[tokenCount] = calloc(strlen(token) + 1, sizeof(char));
                strcpy(tokens[tokenCount], token);

                if (verbose)
                {
                    printf("Found token: %s\n", token);
                }
                tokenCount++;

                // For parsing asciiz strings as full tokens
                if (dataMode)
                {
                    if (!strcmp(token, ".asciiz"))
                    {
                        int quoteIndex = strcspn(line, "\"") + 1;
                        int endQuote = findLastIndexOfChar(line, '\"') - 1;
                        tokens[tokenCount] = calloc(endQuote - quoteIndex + 2, sizeof(char));
                        memcpy(tokens[tokenCount], line + quoteIndex, endQuote - quoteIndex + 1);
                        if (verbose)
                        {
                            printf("Found token: %s\n", tokens[tokenCount]);
                        }
                        tokenCount++;
                        break;
                    }
                    else if (!strcmp(token, ".word"))
                    {
                        token = strtok(NULL, " ,\t\r");
                        continue;
                    }
                }
                token = strtok(NULL, delimeter);
            }

            // Handle text/data segment headers
            if (tokenCount == 1 && strlen(tokens[0]) == 5)
            {
                // Switch to data segment
                if (!strcmp(tokens[0], ".data"))
                {
                    dataMode = true;
                    if (verbose)
                    {
                        printf("Switching to data segment...\n");
                    }
                }
                // Switch to text segment
                else if (!strcmp(tokens[0], ".text"))
                {
                    dataMode = false;
                    if (verbose)
                    {
                        printf("Switching to text segment...\n");
                    }
                }
            }
            // Handle data variables
            else if (dataMode)
            {
                // Get the next open address for a symbol (8192 by default)
                int nextOpenAddress = BASE_DATA_ADDR;
                int numSymbols = symbolTable.numSymbols;

                // Get the next open address by finding it from the previous symbol if there is one
                if (numSymbols > 0)
                {
                    nextOpenAddress = symbolTable.symbols[numSymbols - 1].address + symbolTable.symbols[numSymbols - 1].size;
                }

                // Get the name of this symbol
                char *name = tokens[0];

                // The data type can be either asciiz or word
                bool asciiz = !strcmp(tokens[1], ".asciiz");

                char *binValue;

                // Parse asciiz data type
                if (asciiz)
                {
                    binValue = stringToBinary(tokens[2]);
                }
                // Parse word
                else
                {
                    // Add padding if the previous symbol was a misaligned asciiz string
                    if (nextOpenAddress % LINE_WIDTH)
                    {
                        // Amount of padding to add
                        int offset = LINE_WIDTH - (nextOpenAddress % LINE_WIDTH);
                        nextOpenAddress += offset;

                        // Update the previous symbol table entry
                        symbolTable.symbols[numSymbols - 1].size += offset;
                        char *previousSymbolBinVal = symbolTable.symbols[numSymbols - 1].binaryData;
                        previousSymbolBinVal = realloc(previousSymbolBinVal, (symbolTable.symbols[numSymbols - 1].size + 1) * sizeof(char));
                        for (int i = 0; i < offset; i++)
                        {
                            strcat(previousSymbolBinVal, "0");
                        }
                        symbolTable.symbols[numSymbols - 1].binaryData = previousSymbolBinVal;

                        if (verbose)
                        {
                            printf("Had to pad symbol: %s\n\tNew value: %s\n", symbolTable.symbols[numSymbols - 1].name, symbolTable.symbols[numSymbols - 1].binaryData);
                        }
                    }

                    // The number of words to add for this symbol
                    int numWords = tokenCount - 2;

                    // Handle : operator
                    bool arrayOperator = false;
                    int spread;
                    if (numWords == 1)
                    {
                        int lenToken = strlen(tokens[2]);
                        int indexOfColon = strcspn(tokens[2], ":");
                        if (indexOfColon < lenToken)
                        {
                            arrayOperator = true;
                            char *spreadString = calloc(indexOfColon + 1, sizeof(char));
                            spreadString[indexOfColon] = '\0';
                            memcpy(spreadString, tokens[2], indexOfColon);
                            spread = atoi(spreadString);
                            free(spreadString);
                            spreadString = calloc(lenToken - indexOfColon, sizeof(char));
                            spreadString[lenToken - indexOfColon - 1] = '\0';
                            for (int i = 0; i < lenToken - indexOfColon - 1; i++)
                            {
                                spreadString[i] = tokens[2][i + indexOfColon + 1];
                            }
                            numWords = atoi(spreadString);
                            free(spreadString);
                        }
                    }

                    binValue = calloc(numWords * LINE_WIDTH + 1, sizeof(char));
                    binValue[0] = '\0';
                    for (int i = 0; i < numWords; i++)
                    {
                        // Get the int representation of this word
                        int word = arrayOperator ? spread : atoi(tokens[i + 2]);
                        char *tmp = TwosComplement(word, LINE_WIDTH);
                        strcat(binValue, tmp);
                        free(tmp);
                    }
                }
                // Create the symbol and add it to the symbol table
                Symbol symbol = createSymbol(name, binValue, nextOpenAddress, verbose);
                addSymbolToTable(&symbolTable, symbol, verbose);

                // Deallocate our binary representation after using it
                free(binValue);
            }
            // Handle text instructions (condition skips labels)
            else if (!(tokenCount == 1 && strcmp(tokens[0], "syscall") && strcmp(tokens[0], "nop")))
            {
                textSegment[numTextOutputLines] = parseInstruction(tokens, &symbolTable, &labelTable, tokenCount, instructionAddress, verbose);
                if (verbose)
                {
                    printf("Parsed: %s\n", textSegment[numTextOutputLines]);
                }
                int len = strlen(textSegment[numTextOutputLines]);
                instructionAddress += len - (len / LINE_WIDTH) + 1;
                numTextOutputLines++;
            }

            // Free up the memory allocated by the tokens array
            for (int i = 0; i < tokenCount; i++)
            {
                free(tokens[i]);
                tokens[i] = NULL;
            }
        }

        // Free the instruction string we allocated
        free(instruction);
    }

    // Create the output string
    char *output = createOutput(textSegment, numTextOutputLines, &symbolTable, &labelTable, symbols, verbose);

    // Free the memory allocated by the text segment output array
    for (int i = 0; i < numTextOutputLines; i++)
    {
        free(textSegment[i]);
        textSegment[i] = NULL;
    }
    free(textSegment);

    // Free the table data
    freeSymbolTableData(&symbolTable, verbose);
    freeLabelTableData(&labelTable, verbose);

    return output;
}

char *parseInstruction(char **tokens, SymbolTable *symbolTable, LabelTable *labelTable, const int numTokens, const int address, const bool verbose)
{
    // Length should be LINE_WIDTH or 2 * LINE_WIDTH for some pseudo instructions
    char *output = calloc(LINE_WIDTH + 1, sizeof(char));
    output[LINE_WIDTH] = '\0';

    // Get the opcode
    Opcode *opcode = findOpcode(tokens[0]);

    // Handle psuedo instructions by translating them
    if (!opcode)
    {
        if (!strcmp(tokens[0], "move"))
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "addu";
            psuedoTokens[1] = tokens[1];
            psuedoTokens[2] = "$zero";
            psuedoTokens[3] = tokens[2];
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
        }
        else if (!strcmp(tokens[0], "blt"))
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "slt";
            psuedoTokens[1] = "$at";
            psuedoTokens[2] = tokens[1];
            psuedoTokens[3] = tokens[2];
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
            output = realloc(output, ((LINE_WIDTH * 2) + 2) * sizeof(char));
            strcat(output, "\n");
            psuedoTokens[0] = "bne";
            psuedoTokens[1] = "$at";
            psuedoTokens[2] = "$zero";
            psuedoTokens[3] = tokens[3];
            psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcat(output, psuedo);
            free(psuedo);
        }
        else if (!strcmp(tokens[0], "la"))
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "addi";
            psuedoTokens[1] = tokens[1];
            psuedoTokens[2] = "$zero";
            Symbol *symbol = findSymbolByName(symbolTable, tokens[2]);
            char strAddress[5];
            sprintf(strAddress, "%d", (symbol->address % BASE_DATA_ADDR) / 8 + BASE_DATA_ADDR);
            psuedoTokens[3] = strAddress;
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
        }
        else if (!strcmp(tokens[0], "li"))
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "addiu";
            psuedoTokens[1] = tokens[1];
            psuedoTokens[2] = "$zero";
            psuedoTokens[3] = tokens[2];
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
        }
        else if (!strcmp(tokens[0], "nop"))
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "sll";
            psuedoTokens[1] = "$zero";
            psuedoTokens[2] = "$zero";
            psuedoTokens[3] = "0";
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
        }
        // Handle unknown instruction
        else
        {
            for (int i = 0; i < LINE_WIDTH; i++)
            {
                output[i] = '*';
            }
        }
        return output;
    }
    InstructionFormat format = opcode->format;
    char *opcodeBin = opcode->binary;
    char *funct = opcode->funct;

    // Need to parse each format
    switch (format)
    {
        Register *rd;
        Register *rs;
        Register *rt;
        Label *label;
        int16_t imm;
        char *immStr;
    case SRAV:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rd = findRegister(tokens[1]);
        rs = findRegister(tokens[2]);
        rt = findRegister(tokens[3]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rt->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rs->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 16] = rd->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 21] = '0';
        }
        for (int i = 0; i < 6; i++)
        {
            output[i + 26] = funct[i];
        }
        break;
    case D_S_T:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rd = findRegister(tokens[1]);
        rs = findRegister(tokens[2]);
        rt = findRegister(tokens[3]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rs->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 16] = rd->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 21] = '0';
        }
        for (int i = 0; i < 6; i++)
        {
            output[i + 26] = funct[i];
        }
        break;
    case S_T:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rs = findRegister(tokens[1]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rs->binary[i];
        }
        rt = findRegister(tokens[2]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        for (int i = 0; i < 10; i++)
        {
            output[i + 16] = '0';
        }
        for (int i = 0; i < 6; i++)
        {
            output[i + 26] = funct[i];
        }
        break;
    case D_T_SA:
        for (int i = 0; i < 11; i++)
        {
            output[i] = '0';
        }
        rd = findRegister(tokens[1]);
        rt = findRegister(tokens[2]);
        imm = atoi(tokens[3]);
        immStr = TwosComplement(imm, 5);
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 16] = rd->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 21] = immStr[i];
        }
        for (int i = 0; i < 6; i++)
        {
            output[i + 26] = funct[i];
        }
        free(immStr);
        break;
    case SYS:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        for (int i = 0; i < 20; i++)
        {
            output[i + 6] = '0';
        }
        for (int i = 0; i < 6; i++)
        {
            output[i + 26] = funct[i];
        }
        break;
    case T_S_I:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rt = findRegister(tokens[1]);
        rs = findRegister(tokens[2]);
        imm = atoi(tokens[3]);
        immStr = TwosComplement(imm, 16);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rs->binary[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        for (int i = 0; i < 16; i++)
        {
            output[i + 16] = immStr[i];
        }
        free(immStr);
        break;
    case S_I:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rs = findRegister(tokens[1]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rs->binary[i];
        }
        label = findLabelByName(labelTable, tokens[2]);
        immStr = TwosComplement((label->address - address) / LINE_WIDTH - 1, 16);
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = '0';
        }
        for (int i = 0; i < 16; i++)
        {
            output[i + 16] = immStr[i];
        }
        free(immStr);
        break;
    case S_T_I:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rs = findRegister(tokens[1]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = rs->binary[i];
        }
        rt = findRegister(tokens[2]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        label = findLabelByName(labelTable, tokens[3]);
        immStr = TwosComplement((label->address - address) / LINE_WIDTH - 1, 16);
        for (int i = 0; i < 16; i++)
        {
            output[i + 16] = immStr[i];
        }
        free(immStr);
        break;
    case J:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        label = findLabelByName(labelTable, tokens[1]);
        immStr = TwosComplement(label->address / LINE_WIDTH, 26);
        for (int i = 0; i < 26; i++)
        {
            output[i + 6] = immStr[i];
        }
        free(immStr);
        break;
    case T_I:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        for (int i = 0; i < 5; i++)
        {
            output[i + 6] = '0';
        }
        rt = findRegister(tokens[1]);
        for (int i = 0; i < 5; i++)
        {
            output[i + 11] = rt->binary[i];
        }
        imm = atoi(tokens[2]);
        immStr = TwosComplement(imm, 16);
        for (int i = 0; i < 16; i++)
        {
            output[i + 16] = immStr[i];
        }
        free(immStr);
        break;
    case T_I_S:
        for (int i = 0; i < 6; i++)
        {
            output[i] = opcodeBin[i];
        }
        rt = findRegister(tokens[1]);
        if (numTokens == 3)
        {
            char *psuedoTokens[4];
            psuedoTokens[0] = "lw";
            psuedoTokens[1] = tokens[1];
            Symbol *symbol = findSymbolByName(symbolTable, tokens[2]);
            char strAddress[5];
            sprintf(strAddress, "%d", (symbol->address % BASE_DATA_ADDR) / 8 + BASE_DATA_ADDR);
            psuedoTokens[3] = strAddress;
            psuedoTokens[2] = strAddress;
            psuedoTokens[3] = "$zero";
            char *psuedo = parseInstruction(psuedoTokens, symbolTable, labelTable, 4, address, verbose);
            strcpy(output, psuedo);
            free(psuedo);
        }
        else
        {
            imm = atoi(tokens[2]);
            immStr = TwosComplement(imm, 16);
            rs = findRegister(tokens[3]);
            for (int i = 0; i < 5; i++)
            {
                output[i + 6] = rs->binary[i];
            }
            for (int i = 0; i < 5; i++)
            {
                output[i + 11] = rt->binary[i];
            }
            for (int i = 0; i < 16; i++)
            {
                output[i + 16] = immStr[i];
            }
            free(immStr);
        }
        break;
    }
    return output;
}

char *createOutput(char **textBinaryOutput, int numTextBinaryLines, SymbolTable *symbolTable, LabelTable *labelTable, const bool symbols, const bool verbose)
{
    // Output string
    char *output;

    // Symbols output
    if (symbols)
    {
        // Allocate arbitrary amount of memory for output
        size_t cap = 256;
        output = calloc(cap, sizeof(char *));
        output[0] = '\0';

        // Append each label to the output
        for (int i = 0; i < labelTable->numLabels; i++)
        {
            char *hexAdd = calloc(15 + strlen(labelTable->labels[i].name), sizeof(char));
            hexAdd[0] = '\0';
            sprintf(hexAdd, "0x0000%04x:  %s\n", labelTable->labels[i].address, labelTable->labels[i].name);
            if (strlen(hexAdd) + strlen(output) > cap)
            {
                cap *= 2;
                output = realloc(output, cap);
            }
            strcat(output, hexAdd);
            free(hexAdd);
        }
        // Append each symbol to the output
        for (int i = 0; i < symbolTable->numSymbols; i++)
        {
            char *hexAdd = calloc(15 + strlen(symbolTable->symbols[i].name), sizeof(char));
            hexAdd[0] = '\0';
            sprintf(hexAdd, "0x0000%04x:  %s\n", symbolTable->symbols[i].address, symbolTable->symbols[i].name);
            if (strlen(hexAdd) + strlen(output) > cap)
            {
                cap *= 2;
                output = realloc(output, cap);
            }
            strcat(output, hexAdd);
            free(hexAdd);
        }
    }
    // Assembler output
    else
    {
        // Total number text binary characters
        int numTextChars = numTextBinaryLines * LINE_WIDTH;

        // Total number of newline characters
        int numNewLines = numTextBinaryLines + 1;

        // Allocate enough space for the text segment
        output = calloc(numTextChars + numNewLines + 1, sizeof(char));
        output[0] = '\0';

        // Concatenate the text segment
        for (int i = 0; i < numTextBinaryLines; i++)
        {
            if (verbose)
            {
                printf("Outputting text segment line #%d: %s\n", i, textBinaryOutput[i]);
            }

            // If the parsed instruction is multiple lines, adjust the output array accordingly
            int len = strlen(textBinaryOutput[i]);
            int newLines = (len - LINE_WIDTH) / LINE_WIDTH;
            if (strlen(textBinaryOutput[i]) > LINE_WIDTH)
            {
                numTextChars += len - LINE_WIDTH - newLines;
                numNewLines += newLines;
            }
            output = realloc(output, numTextChars + numNewLines + 1);

            // Concatenate the text segment
            strcat(output, textBinaryOutput[i]);
            strcat(output, "\n");
        }

        // Separate the text segment and the data segment by a new line character
        strcat(output, "\n");

        // Number of data segment characters
        int numDataChars = 0;
        for (int i = 0; i < symbolTable->numSymbols; i++)
        {
            // Binary data for this symbol
            char *bin = symbolTable->symbols[i].binaryData;
            if (verbose)
            {
                printf("Outputting data segment #%d: \n%s\n", i, bin);
            }

            // Length of binary data
            int dataLength = strlen(bin);

            // Reallocate the output buffer to support our data symbol
            output = realloc(output, (numTextChars + numDataChars + numNewLines + dataLength + dataLength / LINE_WIDTH + 1) * sizeof(char));

            // Concatenate the binary data
            for (int j = 0; j < dataLength; j++)
            {
                output[numTextChars + numNewLines + numDataChars] = bin[j];
                numDataChars++;
                if (!(numDataChars % LINE_WIDTH))
                {
                    output[numTextChars + numNewLines + numDataChars] = '\n';
                    numNewLines++;
                }
            }
        }

        // Null terminate the output string
        output[numTextChars + numNewLines + numDataChars] = '\0';

        // If need be, pad the data segment
        if (numDataChars % LINE_WIDTH)
        {
            if (verbose)
            {
                printf("Padding data segment for eof...\n");
            }
            output = realloc(output, (numTextChars + numNewLines + numDataChars + 1 + (LINE_WIDTH - (numDataChars % LINE_WIDTH))) * sizeof(char));
            while (numDataChars % LINE_WIDTH)
            {
                strcat(output, "0");
                numDataChars++;
            }
        }
    }
    return output;
}