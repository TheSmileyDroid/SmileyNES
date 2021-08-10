#ifndef SMILEY6502_H
#define SMILEY6502_H
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

struct INSTRUCTION lookup[] = {
    {"BRK", &cpuBRK, &cpuIMM, 7},
    {"ORA", &cpuORA, &cpuIZX, 6},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 3},
    {"ORA", &cpuORA, &cpuZP0, 3},
    {"ASL", &cpuASL, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"PHP", &cpuPHP, &cpuIMP, 3},
    {"ORA", &cpuORA, &cpuIMM, 2},
    {"ASL", &cpuASL, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"ORA", &cpuORA, &cpuABS, 4},
    {"ASL", &cpuASL, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BPL", &cpuBPL, &cpuREL, 2},
    {"ORA", &cpuORA, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"ORA", &cpuORA, &cpuZPX, 4},
    {"ASL", &cpuASL, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"CLC", &cpuCLC, &cpuIMP, 2},
    {"ORA", &cpuORA, &cpuABY, 4},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"ORA", &cpuORA, &cpuABX, 4},
    {"ASL", &cpuASL, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"JSR", &cpuJSR, &cpuABS, 6},
    {"AND", &cpuAND, &cpuIZX, 6},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"BIT", &cpuBIT, &cpuZP0, 3},
    {"AND", &cpuAND, &cpuZP0, 3},
    {"ROL", &cpuROL, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"PLP", &cpuPLP, &cpuIMP, 4},
    {"AND", &cpuAND, &cpuIMM, 2},
    {"ROL", &cpuROL, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"BIT", &cpuBIT, &cpuABS, 4},
    {"AND", &cpuAND, &cpuABS, 4},
    {"ROL", &cpuROL, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BMI", &cpuBMI, &cpuREL, 2},
    {"AND", &cpuAND, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"AND", &cpuAND, &cpuZPX, 4},
    {"ROL", &cpuROL, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"SEC", &cpuSEC, &cpuIMP, 2},
    {"AND", &cpuAND, &cpuABY, 4},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"AND", &cpuAND, &cpuABX, 4},
    {"ROL", &cpuROL, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"RTI", &cpuRTI, &cpuIMP, 6},
    {"EOR", &cpuEOR, &cpuIZX, 6},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 3},
    {"EOR", &cpuEOR, &cpuZP0, 3},
    {"LSR", &cpuLSR, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"PHA", &cpuPHA, &cpuIMP, 3},
    {"EOR", &cpuEOR, &cpuIMM, 2},
    {"LSR", &cpuLSR, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"JMP", &cpuJMP, &cpuABS, 3},
    {"EOR", &cpuEOR, &cpuABS, 4},
    {"LSR", &cpuLSR, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BVC", &cpuBVC, &cpuREL, 2},
    {"EOR", &cpuEOR, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"EOR", &cpuEOR, &cpuZPX, 4},
    {"LSR", &cpuLSR, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"CLI", &cpuCLI, &cpuIMP, 2},
    {"EOR", &cpuEOR, &cpuABY, 4},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"EOR", &cpuEOR, &cpuABX, 4},
    {"LSR", &cpuLSR, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"RTS", &cpuRTS, &cpuIMP, 6},
    {"ADC", &cpuADC, &cpuIZX, 6},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 3},
    {"ADC", &cpuADC, &cpuZP0, 3},
    {"ROR", &cpuROR, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"PLA", &cpuPLA, &cpuIMP, 4},
    {"ADC", &cpuADC, &cpuIMM, 2},
    {"ROR", &cpuROR, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"JMP", &cpuJMP, &cpuIND, 5},
    {"ADC", &cpuADC, &cpuABS, 4},
    {"ROR", &cpuROR, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BVS", &cpuBVS, &cpuREL, 2},
    {"ADC", &cpuADC, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"ADC", &cpuADC, &cpuZPX, 4},
    {"ROR", &cpuROR, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"SEI", &cpuSEI, &cpuIMP, 2},
    {"ADC", &cpuADC, &cpuABY, 4},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"ADC", &cpuADC, &cpuABX, 4},
    {"ROR", &cpuROR, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"STA", &cpuSTA, &cpuIZX, 6},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"STY", &cpuSTY, &cpuZP0, 3},
    {"STA", &cpuSTA, &cpuZP0, 3},
    {"STX", &cpuSTX, &cpuZP0, 3},
    {"???", &cpuXXX, &cpuIMP, 3},
    {"DEY", &cpuDEY, &cpuIMP, 2},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"TXA", &cpuTXA, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"STY", &cpuSTY, &cpuABS, 4},
    {"STA", &cpuSTA, &cpuABS, 4},
    {"STX", &cpuSTX, &cpuABS, 4},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"BCC", &cpuBCC, &cpuREL, 2},
    {"STA", &cpuSTA, &cpuIZY, 6},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"STY", &cpuSTY, &cpuZPX, 4},
    {"STA", &cpuSTA, &cpuZPX, 4},
    {"STX", &cpuSTX, &cpuZPY, 4},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"TYA", &cpuTYA, &cpuIMP, 2},
    {"STA", &cpuSTA, &cpuABY, 5},
    {"TXS", &cpuTXS, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"???", &cpuNOP, &cpuIMP, 5},
    {"STA", &cpuSTA, &cpuABX, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"LDY", &cpuLDY, &cpuIMM, 2},
    {"LDA", &cpuLDA, &cpuIZX, 6},
    {"LDX", &cpuLDX, &cpuIMM, 2},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"LDY", &cpuLDY, &cpuZP0, 3},
    {"LDA", &cpuLDA, &cpuZP0, 3},
    {"LDX", &cpuLDX, &cpuZP0, 3},
    {"???", &cpuXXX, &cpuIMP, 3},
    {"TAY", &cpuTAY, &cpuIMP, 2},
    {"LDA", &cpuLDA, &cpuIMM, 2},
    {"TAX", &cpuTAX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"LDY", &cpuLDY, &cpuABS, 4},
    {"LDA", &cpuLDA, &cpuABS, 4},
    {"LDX", &cpuLDX, &cpuABS, 4},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"BCS", &cpuBCS, &cpuREL, 2},
    {"LDA", &cpuLDA, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"LDY", &cpuLDY, &cpuZPX, 4},
    {"LDA", &cpuLDA, &cpuZPX, 4},
    {"LDX", &cpuLDX, &cpuZPY, 4},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"CLV", &cpuCLV, &cpuIMP, 2},
    {"LDA", &cpuLDA, &cpuABY, 4},
    {"TSX", &cpuTSX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"LDY", &cpuLDY, &cpuABX, 4},
    {"LDA", &cpuLDA, &cpuABX, 4},
    {"LDX", &cpuLDX, &cpuABY, 4},
    {"???", &cpuXXX, &cpuIMP, 4},
    {"CPY", &cpuCPY, &cpuIMM, 2},
    {"CMP", &cpuCMP, &cpuIZX, 6},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"CPY", &cpuCPY, &cpuZP0, 3},
    {"CMP", &cpuCMP, &cpuZP0, 3},
    {"DEC", &cpuDEC, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"INY", &cpuINY, &cpuIMP, 2},
    {"CMP", &cpuCMP, &cpuIMM, 2},
    {"DEX", &cpuDEX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"CPY", &cpuCPY, &cpuABS, 4},
    {"CMP", &cpuCMP, &cpuABS, 4},
    {"DEC", &cpuDEC, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BNE", &cpuBNE, &cpuREL, 2},
    {"CMP", &cpuCMP, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"CMP", &cpuCMP, &cpuZPX, 4},
    {"DEC", &cpuDEC, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"CLD", &cpuCLD, &cpuIMP, 2},
    {"CMP", &cpuCMP, &cpuABY, 4},
    {"NOP", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"CMP", &cpuCMP, &cpuABX, 4},
    {"DEC", &cpuDEC, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"CPX", &cpuCPX, &cpuIMM, 2},
    {"SBC", &cpuSBC, &cpuIZX, 6},
    {"???", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"CPX", &cpuCPX, &cpuZP0, 3},
    {"SBC", &cpuSBC, &cpuZP0, 3},
    {"INC", &cpuINC, &cpuZP0, 5},
    {"???", &cpuXXX, &cpuIMP, 5},
    {"INX", &cpuINX, &cpuIMP, 2},
    {"SBC", &cpuSBC, &cpuIMM, 2},
    {"NOP", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuSBC, &cpuIMP, 2},
    {"CPX", &cpuCPX, &cpuABS, 4},
    {"SBC", &cpuSBC, &cpuABS, 4},
    {"INC", &cpuINC, &cpuABS, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"BEQ", &cpuBEQ, &cpuREL, 2},
    {"SBC", &cpuSBC, &cpuIZY, 5},
    {"???", &cpuXXX, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 8},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"SBC", &cpuSBC, &cpuZPX, 4},
    {"INC", &cpuINC, &cpuZPX, 6},
    {"???", &cpuXXX, &cpuIMP, 6},
    {"SED", &cpuSED, &cpuIMP, 2},
    {"SBC", &cpuSBC, &cpuABY, 4},
    {"NOP", &cpuNOP, &cpuIMP, 2},
    {"???", &cpuXXX, &cpuIMP, 7},
    {"???", &cpuNOP, &cpuIMP, 4},
    {"SBC", &cpuSBC, &cpuABX, 4},
    {"INC", &cpuINC, &cpuABX, 7},
    {"???", &cpuXXX, &cpuIMP, 7},
};

#endif