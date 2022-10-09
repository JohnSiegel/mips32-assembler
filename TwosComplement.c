#include "TwosComplement.h"
#include <stdlib.h>

int16_t ParseImmediate(const char *const Immediate)
{
    return atoi(Immediate);
}

char *TwosComplement(int16_t value)
{
    const size_t BITS = 8 * sizeof(value);
    char *bin_str = calloc(8 * sizeof(value) + 1, sizeof(char));

    for (unsigned int i = 0; i < BITS; i++)
    {
        unsigned int mask = 1u << (BITS - 1 - i);
        bin_str[i] = (value & mask) ? '1' : '0';
    }
    bin_str[BITS] = '\0';
    return bin_str;
}