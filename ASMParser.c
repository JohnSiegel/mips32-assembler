#include "ASMParser.h"
#include "Opcode.h"
#include "Register.h"
#include "TwosComplement.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

ParseResult *
ParseASM(const char *const pASM)
{
   char *delimeter = " \t,()";
   char *ASMInstruction = calloc(strlen(pASM) + 1, sizeof(char));
   strcpy(ASMInstruction, pASM);
   char *Mnemonic = calloc(strlen(pASM) + 1, sizeof(char));
   strcpy(Mnemonic, pASM);
   Mnemonic = strtok(Mnemonic, delimeter);
   Opcode *Opcode = FindOpcode(Mnemonic);
   InstructionFormat Format = Opcode->Format;
   char *OpcodeBinary = calloc(strlen(Opcode->Binary) + 1, sizeof(char));
   strcpy(OpcodeBinary, Opcode->Binary);
   char *Token = NULL;
   char *rtName = NULL;
   char *rsName = NULL;
   char *rdName = NULL;
   char *RT = NULL;
   char *RD = NULL;
   char *RS = NULL;
   char *IMM = NULL;
   char *Funct = NULL;
   uint16_t Imm = 0;
   uint8_t rt = 255;
   uint8_t rs = 255;
   uint8_t rd = 255;
   if (Opcode->Funct)
   {
      Funct = calloc(strlen(Opcode->Funct) + 1, sizeof(Funct));
      strcpy(Funct, Opcode->Funct);
   }
   switch (Format)
   {
   case D_S_T:
      Token = strtok(NULL, delimeter);
      rdName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rdName, Token);
      Token = strtok(NULL, delimeter);
      rsName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rsName, Token);
      Token = strtok(NULL, delimeter);
      rtName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rtName, Token);
      break;
   case S_T_I:
      Token = strtok(NULL, delimeter);
      rsName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rsName, Token);
      Token = strtok(NULL, delimeter);
      rtName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rtName, Token);
      Imm = ParseImmediate(strtok(NULL, delimeter));
      break;
   case T_S_I:
      Token = strtok(NULL, delimeter);
      rtName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rtName, Token);
      Token = strtok(NULL, delimeter);
      rsName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rsName, Token);
      Imm = ParseImmediate(strtok(NULL, delimeter));
      break;
   case T_I:
      Token = strtok(NULL, delimeter);
      rtName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rtName, Token);
      Imm = ParseImmediate(strtok(NULL, delimeter));
      rs = 0;
      RS = calloc(6, sizeof(char));
      strcpy(RS, "000000");
      break;
   case T_I_S:
      Token = strtok(NULL, delimeter);
      rtName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rtName, Token);
      Imm = ParseImmediate(strtok(NULL, delimeter));
      Token = strtok(NULL, delimeter);
      rsName = calloc(strlen(Token) + 1, sizeof(char));
      strcpy(rsName, Token);
      break;
   }
   if (rdName)
   {
      Register *regD = FindRegister(rdName);
      rd = regD->Number;
      RD = calloc(strlen(regD->Binary) + 1, sizeof(char));
      strcpy(RD, regD->Binary);
   }
   if (rsName)
   {
      Register *regS = FindRegister(rsName);
      rs = regS->Number;
      RS = calloc(strlen(regS->Binary) + 1, sizeof(char));
      strcpy(RS, regS->Binary);
   }
   if (rtName)
   {
      Register *regT = FindRegister(rtName);
      rt = regT->Number;
      RT = calloc(strlen(regT->Binary) + 1, sizeof(char));
      strcpy(RT, regT->Binary);
   }
   if (Imm)
   {
      IMM = calloc(17, sizeof(char));
      strcpy(IMM, TwosComplement(Imm));
   }
   ParseResult *result = malloc(sizeof(ParseResult));
   result->ASMInstruction = ASMInstruction;
   result->Mnemonic = Mnemonic;
   result->Opcode = OpcodeBinary;
   result->Funct = Funct;
   result->rdName = rdName;
   result->rtName = rtName;
   result->rsName = rsName;
   result->RD = RD;
   result->RT = RT;
   result->RS = RS;
   result->rd = rd;
   result->rs = rs;
   result->rt = rt;
   result->Imm = Imm;
   result->IMM = IMM;
}
