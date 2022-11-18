#include "Helpers.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int findLastIndexOfChar(char *value, char x)
{
    int length = strlen(value);
    int index = length;
    for (int i = 0; i < length; i++)
    {
        if (value[i] == x)
        {
            index = i;
        }
    }
    return index;
}

char *stringToBinary(char *value)
{
    int len = strlen(value);
    int numBits = len * 8 + 1;
    // Each char is one byte (8 bits) and + 1 at the end for null terminator
    char *binary = calloc(numBits + 8, sizeof(char));
    binary[0] = '\0';
    for (int i = 0; i < len; ++i)
    {
        char ch = value[i];
        for (int j = 7; j >= 0; --j)
        {
            if (ch & (1 << j))
            {
                strcat(binary, "1");
            }
            else
            {
                strcat(binary, "0");
            }
        }
    }
    // Null terminate
    strcat(binary, "00000000");
    return binary;
}

bool isWhitespace(char *value)
{
    int len = strlen(value);
    for (int i = 0; i < len; i++)
    {
        if (!isspace(value[i]))
        {
            return false;
        }
    }
    return true;
}

char *TwosComplement(int value, int bits)
{
    char *bin_str = calloc(bits + 1, sizeof(char));

    for (int i = 0; i < bits; i++)
    {
        unsigned int mask = 1u << (bits - 1 - i);
        bin_str[i] = (value & mask) ? '1' : '0';
    }
    bin_str[bits] = '\0';
    return bin_str;
}