#pragma once
//#ifndef SMILEY6502_H
//#define SMILEY6502_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Bus.h"

enum FLAGS6502
{
    C = (1 << 0), // Carry Bit
    Z = (1 << 1), // Zero
    I = (1 << 2), // Disable Interrupts
    D = (1 << 3), // Decimal Mode (unused in this implementation)
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow
    N = (1 << 7), // Negativo
};

struct INSTRUCTION
{
    char *name;
    uint8_t (*operate)(struct Smiley6502 *);
    uint8_t (*addrmode)(struct Smiley6502 *);
    uint8_t cycles;
};

typedef struct Smiley6502
{
    struct Bus *bus;

    // Registro de acumulação
    uint8_t a;
    // Registro de X
    uint8_t x;
    // Registro de Y
    uint8_t y;
    // Apontador de pilha
    uint8_t stkp;
    // Program pointer
    uint16_t pc;
    // registrador de estado
    uint8_t status;

    // Valor de Input
    uint8_t fetched;

    uint16_t temp;

    uint16_t addr_abs;
    uint16_t addr_rel;
    uint8_t opcode;
    uint8_t cycles;
    uint32_t clock_count;

} Smiley6502;

struct Smiley6502 *createCpu();
void cpuConnectBus(Smiley6502 *cpu, struct Bus *bus);
uint8_t cpuRead(Smiley6502 *cpu, uint16_t a);
void cpuWrite(Smiley6502 *cpu, uint16_t a, uint8_t d);

//Modos de endereço
uint8_t cpuIMP(Smiley6502 *cpu);
uint8_t cpuIMM(Smiley6502 *cpu);
uint8_t cpuZP0(Smiley6502 *cpu);
uint8_t cpuZPX(Smiley6502 *cpu);
uint8_t cpuZPY(Smiley6502 *cpu);
uint8_t cpuREL(Smiley6502 *cpu);
uint8_t cpuABS(Smiley6502 *cpu);
uint8_t cpuABX(Smiley6502 *cpu);
uint8_t cpuABY(Smiley6502 *cpu);
uint8_t cpuIND(Smiley6502 *cpu);
uint8_t cpuIZX(Smiley6502 *cpu);
uint8_t cpuIZY(Smiley6502 *cpu);

//opcodes
uint8_t cpuADC(Smiley6502 *cpu);
uint8_t cpuAND(Smiley6502 *cpu);
uint8_t cpuASL(Smiley6502 *cpu);
uint8_t cpuBCC(Smiley6502 *cpu);
uint8_t cpuBCS(Smiley6502 *cpu);
uint8_t cpuBEQ(Smiley6502 *cpu);
uint8_t cpuBIT(Smiley6502 *cpu);
uint8_t cpuBMI(Smiley6502 *cpu);
uint8_t cpuBNE(Smiley6502 *cpu);
uint8_t cpuBPL(Smiley6502 *cpu);
uint8_t cpuBRK(Smiley6502 *cpu);
uint8_t cpuBVC(Smiley6502 *cpu);
uint8_t cpuBVS(Smiley6502 *cpu);
uint8_t cpuCLC(Smiley6502 *cpu);
uint8_t cpuCLD(Smiley6502 *cpu);
uint8_t cpuCLI(Smiley6502 *cpu);
uint8_t cpuCLV(Smiley6502 *cpu);
uint8_t cpuCMP(Smiley6502 *cpu);
uint8_t cpuCPX(Smiley6502 *cpu);
uint8_t cpuCPY(Smiley6502 *cpu);
uint8_t cpuDEC(Smiley6502 *cpu);
uint8_t cpuDEX(Smiley6502 *cpu);
uint8_t cpuDEY(Smiley6502 *cpu);
uint8_t cpuEOR(Smiley6502 *cpu);
uint8_t cpuINC(Smiley6502 *cpu);
uint8_t cpuINX(Smiley6502 *cpu);
uint8_t cpuINY(Smiley6502 *cpu);
uint8_t cpuJMP(Smiley6502 *cpu);
uint8_t cpuJSR(Smiley6502 *cpu);
uint8_t cpuLDA(Smiley6502 *cpu);
uint8_t cpuLDX(Smiley6502 *cpu);
uint8_t cpuLDY(Smiley6502 *cpu);
uint8_t cpuLSR(Smiley6502 *cpu);
uint8_t cpuNOP(Smiley6502 *cpu);
uint8_t cpuORA(Smiley6502 *cpu);
uint8_t cpuPHA(Smiley6502 *cpu);
uint8_t cpuPHP(Smiley6502 *cpu);
uint8_t cpuPLA(Smiley6502 *cpu);
uint8_t cpuPLP(Smiley6502 *cpu);
uint8_t cpuROL(Smiley6502 *cpu);
uint8_t cpuROR(Smiley6502 *cpu);
uint8_t cpuRTI(Smiley6502 *cpu);
uint8_t cpuRTS(Smiley6502 *cpu);
uint8_t cpuSBC(Smiley6502 *cpu);
uint8_t cpuSEC(Smiley6502 *cpu);
uint8_t cpuSED(Smiley6502 *cpu);
uint8_t cpuSEI(Smiley6502 *cpu);
uint8_t cpuSTA(Smiley6502 *cpu);
uint8_t cpuSTX(Smiley6502 *cpu);
uint8_t cpuSTY(Smiley6502 *cpu);
uint8_t cpuTAX(Smiley6502 *cpu);
uint8_t cpuTAY(Smiley6502 *cpu);
uint8_t cpuTSX(Smiley6502 *cpu);
uint8_t cpuTXA(Smiley6502 *cpu);
uint8_t cpuTXS(Smiley6502 *cpu);
uint8_t cpuTYA(Smiley6502 *cpu);

//Opcode não oficial
uint8_t cpuXXX(Smiley6502 *cpu);

// Funções de eventos externos
void cpuClock(Smiley6502 *cpu);
void cpuReset(Smiley6502 *cpu);
void cpuIrq(Smiley6502 *cpu);
void cpuNmi(Smiley6502 *cpu);

uint8_t cpuFetch(Smiley6502 *cpu);

uint8_t cpuGetFlag(Smiley6502 *cpu, enum FLAGS6502 f);
void cpuSetFlag(Smiley6502 *cpu, enum FLAGS6502 f, bool v);


//#endif