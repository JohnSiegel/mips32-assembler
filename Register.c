#include "Register.h"
#include <string.h>

#define NUM_REGISTERS 8

static Register RegisterTable[NUM_REGISTERS] = {
    {"$t0", 8, "01000"},
    {"$t1", 9, "01001"},
    {"$t2", 10, "01010"},
    {"$t3", 11, "01011"},
    {"$s0", 16, "10000"},
    {"$s1", 17, "10001"},
    {"$s2", 18, "10010"},
    {"$s3", 19, "10011"},
};

const Register *FindRegister(const char *const Name)
{
    for (size_t i = 0; i < NUM_REGISTERS; i++)
    {
        if (!strcmp(Name, RegisterTable[i].Name))
        {
            return RegisterTable + i;
        }
    }
}
