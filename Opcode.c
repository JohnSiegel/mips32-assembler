#include "Opcode.h"
#include <string.h>

#define NUM_OPCODES 8
static Opcode OpcodeTable[NUM_OPCODES] = {
    {"add", "000000", "100000", D_S_T},
    {"addi", "001000", NULL, T_S_I},
    {"mul", "011100", "000010", D_S_T},
    {"beq", "000100", NULL, S_T_I},
    {"lui", "001111", NULL, T_I},
    {"lw", "100011", NULL, T_I_S},
    {"sw", "101011", NULL, T_I_S},
    {"sub", "000000", "100010", D_S_T},
};

const Opcode *FindOpcode(const char *const Mnemonic)
{
    for (size_t i = 0; i < NUM_OPCODES; i++)
    {
        if (!strcmp(Mnemonic, OpcodeTable[i].Mnemonic))
        {
            return OpcodeTable + i;
        }
    }
}
