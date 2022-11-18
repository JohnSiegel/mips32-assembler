#include "Label.h"
#include "Helpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Label createLabel(const char *name, const int address, const bool verbose)
{
    char *labelName = calloc(strlen(name) + 1, sizeof(char));
    strcpy(labelName, name);
    Label label = {
        labelName,
        address};
    if (verbose)
    {
        printf("Created Label:\n\tLabel name: %s\n\tHex address: 0x%x\n\tAddress: %d\n", labelName, address, address);
    }
    return label;
}

void freeLabelData(Label *label, const bool verbose)
{
    if (verbose)
    {
        printf("Freeing data from label: %s...\n", label->name);
    }
    free(label->name);
}