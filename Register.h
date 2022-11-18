#ifndef REGISTER_H
#define REGISTER_H

struct _Register
{
    char *name;
    char *binary;
};

/**
 * A valid register with a unique 5 bit code
 */
typedef struct _Register Register;

/**
 * Finds a register in the static lookup table
 */
Register *findRegister(char *name);

#endif