#include "Smiley6502.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct Smiley6502 *createCpu()
{
    struct Smiley6502 *cpu;
    cpu = (struct Smiley6502 *)malloc(sizeof *cpu);
    return cpu;
}

void cpuConnectBus(Smiley6502 *cpu, struct Bus *bus)
{
    cpu->bus = bus;
};

uint8_t cpuRead(Smiley6502 *cpu, uint16_t a)
{
    return busRead(cpu->bus, a, false);
}

void cpuWrite(Smiley6502 *cpu, uint16_t a, uint8_t d)
{
    busWrite(cpu->bus, a, d);
}