#ifndef LABEL_H
#define LABEL_H
#include <stdbool.h>

struct _Label
{
    char *name;
    int address;
};

/**
 * Represents a label in the label table
 */
typedef struct _Label Label;

/**
 * Create a new label
 */
Label createLabel(const char *name, const int address, const bool verbose);

/**
 * Frees the memory from a label
 */
void freeLabelData(Label *label, const bool verbose);

#endif