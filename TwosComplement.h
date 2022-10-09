#ifndef TWOS_COMPLEMENT_H
#define TWOS_COMPLEMENT_H
#include <inttypes.h>

int16_t ParseImmediate(const char *const Immediate);

char *TwosComplement(int16_t value);

#endif