#ifndef BUS_H
#define BUS_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "Smiley6502.h"

typedef struct Bus
{
    //Nossa CPU 6502
    struct Smiley6502 *cpu;

    //Fake RAM
    uint8_t ram[64 * 1024];
} Bus;

struct Bus *createBus();

void busWrite(Bus *bus, uint16_t addr, uint8_t data);
uint8_t busRead(Bus *bus, uint16_t addr, bool bReadOnly);

#endif