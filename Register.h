#ifndef REGISTER_H
#define REGISTER_H
#include <inttypes.h>

struct _Register
{
    const char *Name;
    uint8_t Number;
    const char *Binary;
};

typedef struct _Register Register;

const Register *FindRegister(const char *const Name);

#endif