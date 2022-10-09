#include "ParseResult.h"
#include <stdlib.h>

void ClearResult(ParseResult *const pPR)
{
	if (pPR->ASMInstruction)
	{
		free(pPR->ASMInstruction);
		pPR->ASMInstruction = NULL;
	}
	if (pPR->Mnemonic)
	{
		free(pPR->Mnemonic);
		pPR->Mnemonic = NULL;
	}
	if (pPR->Opcode)
	{
		free(pPR->Opcode);
		pPR->Opcode = NULL;
	}
	if (pPR->Funct)
	{
		free(pPR->Funct);
		pPR->Funct = NULL;
	}
	if (pPR->rdName)
	{
		free(pPR->rdName);
		pPR->rdName = NULL;
	}
	if (pPR->rtName)
	{
		free(pPR->rtName);
		pPR->rtName = NULL;
	}
	if (pPR->rsName)
	{
		free(pPR->rsName);
		pPR->rsName = NULL;
	}
	if (pPR->RD)
	{
		free(pPR->RD);
		pPR->RD = NULL;
	}
	if (pPR->RT)
	{
		free(pPR->RT);
		pPR->RT = NULL;
	}
	if (pPR->RS)
	{
		free(pPR->RS);
		pPR->RS = NULL;
	}
	if (pPR->IMM)
	{
		free(pPR->IMM);
		pPR->IMM = NULL;
	}
}

void PrintResult(FILE *Log, const ParseResult *const pPR)
{

	fprintf(Log, "%s\n", pPR->ASMInstruction);
	fprintf(Log, "   %s   %s\n", pPR->Opcode, pPR->Mnemonic);
	fprintf(Log, "   %2" PRIu8 "   %s", pPR->rd, pPR->rdName);
	if (pPR->RD != NULL)
	{
		fprintf(Log, "   %s", pPR->RD);
	}
	fprintf(Log, "\n");
	fprintf(Log, "   %2" PRIu8 "   %s", pPR->rs, pPR->rsName);
	if (pPR->RS != NULL)
	{
		fprintf(Log, "   %s", pPR->RS);
	}
	fprintf(Log, "\n");
	fprintf(Log, "   %2" PRIu8 "   %s", pPR->rt, pPR->rtName);
	if (pPR->RT != NULL)
	{
		fprintf(Log, "   %s", pPR->RT);
	}
	fprintf(Log, "\n");
	fprintf(Log, "   %s\n", pPR->Funct);
	fprintf(Log, "   %" PRId16 "\n", pPR->Imm);
	if (pPR->IMM != NULL)
	{
		fprintf(Log, "   %s", pPR->IMM);
	}
	fprintf(Log, "\n");
	fprintf(Log, "\n");
}
