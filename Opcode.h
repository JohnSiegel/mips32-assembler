#ifndef OPCODE_H
#define OPCODE_H

enum _InstructionFormat
{
    D_S_T,
    S_T_I,
    T_S_I,
    T_I,
    T_I_S
};

typedef enum _InstructionFormat InstructionFormat;

struct _Opcode
{
    const char *Mnemonic;
    const char *Binary;
    const char *Funct;
    InstructionFormat Format;
};

typedef struct _Opcode Opcode;

const Opcode *FindOpcode(const char *const Mnemonic);

#endif