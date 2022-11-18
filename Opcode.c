#include "Opcode.h"
#include <string.h>
#include <stdlib.h>

#define NUM_OPCODES 24

static Opcode opcodeTable[NUM_OPCODES] = {
    {"add", "000000", "100000", D_S_T},
    {"addi", "001000", NULL, T_S_I},
    {"addu", "000000", "100001", D_S_T},
    {"addiu", "001001", NULL, T_S_I},
    {"and", "000000", "100100", D_S_T},
    {"andi", "001100", NULL, T_S_I},
    {"mult", "000000", "011000", S_T},
    {"nor", "000000", "100111", D_S_T},
    {"sll", "000000", "000000", D_T_SA},
    {"slt", "000000", "101010", D_S_T},
    {"slti", "001010", NULL, T_S_I},
    {"sra", "000000", "000011", D_T_SA},
    {"srav", "000000", "000111", SRAV},
    {"blez", "000110", NULL, S_I},
    {"bgtz", "000111", NULL, S_I},
    {"bne", "000101", NULL, S_T_I},
    {"j", "000010", NULL, J},
    {"syscall", "000000", "001100", SYS},
    {"mul", "011100", "000010", D_S_T},
    {"beq", "000100", NULL, S_T_I},
    {"lui", "001111", NULL, T_I},
    {"lw", "100011", NULL, T_I_S},
    {"sw", "101011", NULL, T_I_S},
    {"sub", "000000", "100010", D_S_T},
};

Opcode *findOpcode(const char *const mnemonic)
{
    for (size_t i = 0; i < NUM_OPCODES; i++)
    {
        if (!strcmp(opcodeTable[i].mnemonic, mnemonic))
        {
            return opcodeTable + i;
        }
    }
    return NULL;
}
