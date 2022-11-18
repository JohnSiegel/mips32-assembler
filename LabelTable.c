#include "LabelTable.h"
#include <string.h>

void freeLabelTableData(LabelTable *table, const bool verbose)
{
    for (int i = 0; i < table->numLabels; i++)
    {
        freeLabelData(&table->labels[i], verbose);
    }
    free(table->labels);
    if (verbose)
    {
        printf("Freed label table\n");
    }
}

void addLabelToTable(LabelTable *table, Label label, const bool verbose)
{
    table->labels[table->numLabels] = label;
    table->numLabels++;
    if (table->numLabels >= table->capacity)
    {
        if (verbose)
        {
            printf("Increasing label table capacity to: %d\n", table->capacity * 2);
        }
        table->capacity *= 2;
        table->labels = realloc(table->labels, table->capacity * sizeof(Label));
    }
}

Label *findLabelByName(LabelTable *table, char *name)
{
    for (int i = 0; i < table->numLabels; i++)
    {
        if (!strcmp(table->labels[i].name, name))
        {
            return table->labels + i;
        }
    }
    return NULL;
}

LabelTable createLabelTable(const bool verbose)
{
    int capacity = 100;
    if (verbose)
    {
        printf("Creating a label table with capacity: %d\n", capacity);
    }
    LabelTable table = {
        calloc(capacity, sizeof(Label)),
        0, capacity};
    return table;
}