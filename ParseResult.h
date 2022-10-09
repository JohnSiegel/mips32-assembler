#ifndef PARSERESULT_H
#define PARSERESULT_H
#include <inttypes.h>
#include <stdio.h>

struct _ParseResult
{
   char *ASMInstruction;
   char *Mnemonic;
   char *rdName;
   char *rsName;
   char *rtName;

   int16_t Imm;
   uint8_t rd;
   uint8_t rs;
   uint8_t rt;

   char *Opcode;
   char *Funct;
   char *RD;
   char *RS;
   char *RT;
   char *IMM;
};

typedef struct _ParseResult ParseResult;

void ClearResult(ParseResult *const pPR);

void PrintResult(FILE *Log, const ParseResult *const pPR);

#endif
