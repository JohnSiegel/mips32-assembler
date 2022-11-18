#ifndef OPCODE_H
#define OPCODE_H

enum _InstructionFormat
{
    D_S_T,
    S_T_I,
    T_S_I,
    T_I,
    T_I_S,
    S_T,
    S_I,
    D_T_SA,
    J,
    SRAV,
    SYS
};

typedef enum _InstructionFormat InstructionFormat;

struct _Opcode
{
    char *mnemonic;
    char *binary;
    char *funct;
    InstructionFormat format;
};

typedef struct _Opcode Opcode;

Opcode *findOpcode(const char *const mnemonic);

#endif