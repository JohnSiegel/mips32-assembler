#include "Register.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_REGISTERS 32

static Register registerTable[NUM_REGISTERS] = {
    {"$zero", "00000"},
    {"$at", "00001"},
    {"$v0", "00010"},
    {"$v1", "00011"},
    {"$a0", "00100"},
    {"$a1", "00101"},
    {"$a2", "00110"},
    {"$a3", "00111"},
    {"$t0", "01000"},
    {"$t1", "01001"},
    {"$t2", "01010"},
    {"$t3", "01011"},
    {"$t4", "01100"},
    {"$t5", "01101"},
    {"$t6", "01110"},
    {"$t7", "01111"},
    {"$s0", "10000"},
    {"$s1", "10001"},
    {"$s2", "10010"},
    {"$s3", "10011"},
    {"$s4", "10100"},
    {"$s5", "10101"},
    {"$s6", "10110"},
    {"$s7", "10111"},
    {"$t8", "11000"},
    {"$t9", "11001"},
    {"$k0", "11010"},
    {"$k1", "11011"},
    {"$gp", "11100"},
    {"$sp", "11101"},
    {"$fp", "11110"},
    {"$ra", "11111"},
};

Register *findRegister(char *name)
{
    for (size_t i = 0; i < NUM_REGISTERS; i++)
    {
        if (!strcmp(registerTable[i].name, name))
        {
            return registerTable + i;
        }
    }
    return NULL;
}
