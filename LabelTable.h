#ifndef LABELTABLE_H
#define LABELTABLE_H
#include "Label.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct _LabelTable
{
    Label *labels;
    int numLabels;
    int capacity;
};

/**
 * Represents a table of labels
 */
typedef struct _LabelTable LabelTable;

/**
 * Frees the data from a table of labels
 */
void freeLabelTableData(LabelTable *table, const bool verbose);

/**
 * Finds a label by its name
 */
Label *findLabelByName(LabelTable *table, char *name);

/**
 * Adds a label to the table
 */
void addLabelToTable(LabelTable *table, Label label, const bool verbose);

/**
 * Creates a new label table
 */
LabelTable createLabelTable(const bool verbose);

#endif