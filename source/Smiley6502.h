#ifndef SMILEY6502_H
#define SMILEY6502_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Bus.h"

typedef struct Smiley6502
{
    struct Bus *bus;
} Smiley6502;

struct Smiley6502 *createCpu();
void cpuConnectBus(Smiley6502 *cpu, struct Bus *bus);
uint8_t cpuRead(Smiley6502 *cpu, uint16_t a);
void cpuWrite(Smiley6502 *cpu, uint16_t a, uint8_t d);

#endif