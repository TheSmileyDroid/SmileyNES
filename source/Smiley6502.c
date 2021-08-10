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

void cpuClock(Smiley6502 *cpu)
{
    if (cpu->cycles == 0)
    {
        cpu->opcode = cpuRead(cpu, cpu->pc);
        cpu->pc++;

        // Número inicial de ciclos
        cpu->cycles = lookup[cpu->opcode].cycles;

        uint8_t additional_cycle1 = (lookup[cpu->opcode].addrmode)(cpu);
        uint8_t additional_cycle2 = (lookup[cpu->opcode].operate)(cpu);

        cpu->cycles += (additional_cycle1 & additional_cycle2);
    }

    cpu->cycles--;
}

void cpuReset(Smiley6502 *cpu)
{
    // Get address to set program counter to
    cpu->addr_abs = 0xFFFC;
    uint16_t lo = cpuRead(cpu, cpu->addr_abs + 0);
    uint16_t hi = cpuRead(cpu, cpu->addr_abs + 1);

    // Set it
    cpu->pc = (hi << 8) | lo;

    // Reset internal registers
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->stkp = 0xFD;
    cpu->status = 0x00 | U;

    // Clear internal helper variables
    cpu->addr_rel = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->fetched = 0x00;

    // Reset takes time
    cpu->cycles = 8;
}

void cpuIrq(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, I) == 0)
    {
        // Push the program counter to the stack. It's 16-bits dont
        // forget so that takes two pushes
        cpuWrite(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
        cpu->stkp--;
        cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
        cpu->stkp--;

        // Then Push the status register to the stack
        cpuSetFlag(cpu, B, 0);
        cpuSetFlag(cpu, U, 1);
        cpuSetFlag(cpu, I, 1);
        cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->status);
        cpu->stkp--;

        // Read new program counter location from fixed address
        cpu->addr_abs = 0xFFFE;
        uint16_t lo = cpuRead(cpu, cpu->addr_abs + 0);
        uint16_t hi = cpuRead(cpu, cpu->addr_abs + 1);
        cpu->pc = (hi << 8) | lo;

        // IRQs take time
        cpu->cycles = 7;
    }
}
void cpuNmi(Smiley6502 *cpu)
{
    cpuWrite(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
    cpu->stkp--;
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
    cpu->stkp--;

    cpuSetFlag(cpu, B, 0);
    cpuSetFlag(cpu, U, 1);
    cpuSetFlag(cpu, I, 1);
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->status);
    cpu->stkp--;

    cpu->addr_abs = 0xFFFA;
    uint16_t lo = cpuRead(cpu, cpu->addr_abs + 0);
    uint16_t hi = cpuRead(cpu, cpu->addr_abs + 1);
    cpu->pc = (hi << 8) | lo;

    cpu->cycles = 8;
}

uint8_t cpuGetFrag(Smiley6502 *cpu, enum FLAGS6502 f)
{
    return ((cpu->status & f) > 0) ? 1 : 0;
}
void cpuSetFlag(Smiley6502 *cpu, enum FLAGS6502 f, bool v)
{
    if (v)
        cpu->status |= f;
    else
        cpu->status &= ~f;
}

// Modos de endereço
uint8_t cpuIMP(Smiley6502 *cpu)
{
    cpu->fetched = cpu->a;
    return 0;
}
uint8_t cpuIMM(Smiley6502 *cpu)
{
    cpu->addr_abs = cpu->pc;
    return 0;
}
uint8_t cpuZP0(Smiley6502 *cpu)
{
    cpu->addr_abs = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    cpu->addr_abs &= 0x00FF;
    return 0;
}
uint8_t cpuZPX(Smiley6502 *cpu)
{
    cpu->addr_abs = cpuRead(cpu, (cpu->pc + cpu->x));
    cpu->pc++;
    cpu->addr_abs &= 0x00FF;
    return 0;
}
uint8_t cpuZPY(Smiley6502 *cpu)
{
    cpu->addr_abs = cpuRead(cpu, (cpu->pc + cpu->y));
    cpu->pc++;
    cpu->addr_abs &= 0x00FF;
    return 0;
}
uint8_t cpuREL(Smiley6502 *cpu)
{
    cpu->addr_rel = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    if (cpu->addr_rel & 0x80)
        cpu->addr_rel |= 0xFF00;
    return 0;
}
uint8_t cpuABS(Smiley6502 *cpu)
{
    uint16_t lo = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo;

    return 0;
}
uint8_t cpuABX(Smiley6502 *cpu)
{
    uint16_t lo = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo;
    cpu->addr_abs += cpu->x;

    if ((cpu->addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}
uint8_t cpuABY(Smiley6502 *cpu)
{
    uint16_t lo = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo;
    cpu->addr_abs += cpu->y;

    if ((cpu->addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}
uint8_t cpuIND(Smiley6502 *cpu)
{
    uint16_t ptr_lo = cpuRead(cpu, cpu->pc);
    cpu->pc++;
    uint16_t ptr_hi = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
    {
        cpu->addr_abs = (cpuRead(cpu, ptr & 0xFF00) << 8) | cpuRead(cpu, ptr + 0);
    }
    else // Behave normally
    {
        cpu->addr_abs = (cpuRead(cpu, ptr + 1) << 8) | cpuRead(cpu, ptr + 0);
    }

    return 0;
}
uint8_t cpuIZX(Smiley6502 *cpu)
{
    uint16_t t = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = cpuRead(cpu, (uint16_t)(t + (uint16_t)cpu->x) & 0x00FF);
    uint16_t hi = cpuRead(cpu, (uint16_t)(t + (uint16_t)cpu->x + 1) & 0x00FF);

    cpu->addr_abs = (hi << 8) | lo;

    return 0;
}
uint8_t cpuIZY(Smiley6502 *cpu)
{
    uint16_t t = cpuRead(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = cpuRead(cpu, (uint16_t)(t + (uint16_t)cpu->y) & 0x00FF);
    uint16_t hi = cpuRead(cpu, (uint16_t)(t + (uint16_t)cpu->y + 1) & 0x00FF);

    cpu->addr_abs = (hi << 8) | lo;

    return 0;
}

uint8_t cpuFetch(Smiley6502 *cpu)
{
    if (!(lookup[cpu->opcode].addrmode == &cpuIMP))
        cpu->fetched = cpuRead(cpu, cpu->addr_abs);
    return cpu->fetched;
}

//opcodes
uint8_t cpuADC(Smiley6502 *cpu)
{
    // Grab the data that we are adding to the accumulator
    cpuFetch(cpu);

    // Add is performed in 16-bit domain for emulation to capture any
    // carry bit, which will exist in bit 8 of the 16-bit word
    cpu->temp = (uint16_t)cpu->a + (uint16_t)cpu->fetched + (uint16_t)cpuGetFlag(cpu, C);

    // The carry flag out exists in the high byte bit 0
    cpuSetFlag(cpu, C, cpu->temp > 255);

    // The Zero flag is set if the result is 0
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0);

    // The signed Overflow flag is set based on all that up there! :D
    cpuSetFlag(cpu, V, (~((uint16_t)cpu->a ^ (uint16_t)cpu->fetched) & ((uint16_t)cpu->a ^ (uint16_t)cpu->temp)) & 0x0080);

    // The negative flag is set to the most significant bit of the result
    cpuSetFlag(cpu, N, cpu->temp & 0x80);

    // Load the result into the accumulator (it's 8-bit dont forget!)
    cpu->a = cpu->temp & 0x00FF;

    // This instruction has the potential to require an additional clock cycle
    return 1;
}
uint8_t cpuAND(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->a = cpu->a & cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 1;
}
uint8_t cpuASL(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)cpu->fetched << 1;
    cpuSetFlag(cpu, C, (cpu->temp & 0xFF00) > 0);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x00);
    cpuSetFlag(cpu, N, cpu->temp & 0x80);
    if (lookup[cpu->opcode].addrmode == &cpuIMP)
        cpu->a = cpu->temp & 0x00FF;
    else
        cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    return 0;
}
uint8_t cpuBCC(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, C) == 0)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBCS(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBEQ(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, Z) == 1)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBIT(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = cpu->a & cpu->fetched;
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x00);
    cpuSetFlag(cpu, N, cpu->fetched & (1 << 7));
    cpuSetFlag(cpu, V, cpu->fetched & (1 << 6));
    return 0;
}
uint8_t cpuBMI(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, N) == 1)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBNE(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, Z) == 0)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBPL(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, N) == 0)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBRK(Smiley6502 *cpu)
{
    cpu->pc++;

    cpuSetFlag(cpu, I, 1);
    cpuWrite(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
    cpu->stkp--;
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
    cpu->stkp--;

    cpuSetFlag(cpu, B, 1);
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->status);
    cpu->stkp--;
    cpuSetFlag(cpu, B, 0);

    cpu->pc = (uint16_t)cpuRead(cpu, 0xFFFE) | ((uint16_t)cpuRead(cpu, 0xFFFF) << 8);
    return 0;
}

uint8_t cpuBVC(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, V) == 0)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuBVS(Smiley6502 *cpu)
{
    if (cpuGetFlag(cpu, V) == 1)
    {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}
uint8_t cpuCLC(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, C, false);
    return 0;
}
uint8_t cpuCLD(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, D, false);
    return 0;
}
uint8_t cpuCLI(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, I, false);
    return 0;
}
uint8_t cpuCLV(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, V, false);
    return 0;
}
uint8_t cpuCMP(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)cpu->a - (uint16_t)cpu->fetched;
    cpuSetFlag(cpu, C, cpu->a >= cpu->fetched);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    return 1;
}
uint8_t cpuCPX(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)cpu->x - (uint16_t)cpu->fetched;
    cpuSetFlag(cpu, C, cpu->x >= cpu->fetched);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    return 0;
}
uint8_t cpuCPY(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)cpu->y - (uint16_t)cpu->fetched;
    cpuSetFlag(cpu, C, cpu->y >= cpu->fetched);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    return 0;
}
uint8_t cpuDEC(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = cpu->fetched - 1;
    cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    return 0;
}
uint8_t cpuDEX(Smiley6502 *cpu)
{
    cpu->x--;
    cpuSetFlag(cpu, Z, cpu->x == 0x00);
    cpuSetFlag(cpu, N, cpu->x & 0x80);
    return 0;
}
uint8_t cpuDEY(Smiley6502 *cpu)
{
    cpu->y--;
    cpuSetFlag(cpu, Z, cpu->y == 0x00);
    cpuSetFlag(cpu, N, cpu->y & 0x80);
    return 0;
}
uint8_t cpuEOR(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->a = cpu->a ^ cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 1;
}
uint8_t cpuINC(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = cpu->fetched + 1;
    cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    return 0;
}
uint8_t cpuINX(Smiley6502 *cpu)
{
    cpu->x++;
    cpuSetFlag(cpu, Z, cpu->x == 0x00);
    cpuSetFlag(cpu, N, cpu->x & 0x80);
    return 0;
}
uint8_t cpuINY(Smiley6502 *cpu)
{
    cpu->y++;
    cpuSetFlag(cpu, Z, cpu->y == 0x00);
    cpuSetFlag(cpu, N, cpu->y & 0x80);
    return 0;
}
uint8_t cpuJMP(Smiley6502 *cpu)
{
    cpu->pc = cpu->addr_abs;
    return 0;
}
uint8_t cpuJSR(Smiley6502 *cpu)
{
    cpu->pc--;

    cpuWrite(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
    cpu->stkp--;
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
    cpu->stkp--;

    cpu->pc = cpu->addr_abs;
    return 0;
}
uint8_t cpuLDA(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->a = cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 1;
}
uint8_t cpuLDX(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->x = cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->x == 0x00);
    cpuSetFlag(cpu, N, cpu->x & 0x80);
    return 1;
}
uint8_t cpuLDY(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->y = cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->y == 0x00);
    cpuSetFlag(cpu, N, cpu->y & 0x80);
    return 1;
}
uint8_t cpuLSR(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpuSetFlag(cpu, C, cpu->fetched & 0x0001);
    cpu->temp = cpu->fetched >> 1;
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    if (lookup[cpu->opcode].addrmode == &cpuIMP)
        cpu->a = cpu->temp & 0x00FF;
    else
        cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    return 0;
}
uint8_t cpuNOP(Smiley6502 *cpu)
{
    // Sadly not all NOPs are equal, Ive added cpu->a few here
    // based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_cpu->opcodes
    // and will add more based on game compatibility, and ultimately
    // I'd like to cover all illegal cpu->opcodes too
    switch (cpu->opcode)
    {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
        return 1;
        break;
    }
    return 0;
}
uint8_t cpuORA(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->a = cpu->a | cpu->fetched;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 1;
}
uint8_t cpuPHA(Smiley6502 *cpu)
{
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->a);
    cpu->stkp--;
    return 0;
}
uint8_t cpuPHP(Smiley6502 *cpu)
{
    cpuWrite(cpu, 0x0100 + cpu->stkp, cpu->status | B | U);
    cpuSetFlag(cpu, B, 0);
    cpuSetFlag(cpu, U, 0);
    cpu->stkp--;
    return 0;
}
uint8_t cpuPLA(Smiley6502 *cpu)
{
    cpu->stkp++;
    cpu->a = cpuRead(cpu, 0x0100 + cpu->stkp);
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 0;
}
uint8_t cpuPLP(Smiley6502 *cpu)
{
    cpu->stkp++;
    cpu->status = cpuRead(cpu, 0x0100 + cpu->stkp);
    cpuSetFlag(cpu, U, 1);
    return 0;
}
uint8_t cpuROL(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)(cpu->fetched << 1) | cpuGetFlag(cpu, C);
    cpuSetFlag(cpu, C, cpu->temp & 0xFF00);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    if (lookup[cpu->opcode].addrmode == &cpuIMP)
        cpu->a = cpu->temp & 0x00FF;
    else
        cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    return 0;
}
uint8_t cpuROR(Smiley6502 *cpu)
{
    cpuFetch(cpu);
    cpu->temp = (uint16_t)(cpuGetFlag(cpu, C) << 7) | (cpu->fetched >> 1);
    cpuSetFlag(cpu, C, cpu->fetched & 0x01);
    cpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x00);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    if (lookup[cpu->opcode].addrmode == &cpuIMP)
        cpu->a = cpu->temp & 0x00FF;
    else
        cpuWrite(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    return 0;
}
uint8_t cpuRTI(Smiley6502 *cpu)
{
    cpu->stkp++;
    cpu->status = cpuRead(cpu, 0x0100 + cpu->stkp);
    cpu->status &= ~B;
    cpu->status &= ~U;

    cpu->stkp++;
    cpu->pc = (uint16_t)cpuRead(cpu, 0x0100 + cpu->stkp);
    cpu->stkp++;
    cpu->pc |= (uint16_t)cpuRead(cpu, 0x0100 + cpu->stkp) << 8;
    return 0;
}
uint8_t cpuRTS(Smiley6502 *cpu)
{
    cpu->stkp++;
    cpu->pc = (uint16_t)cpuRead(cpu, 0x0100 + cpu->stkp);
    cpu->stkp++;
    cpu->pc |= (uint16_t)cpuRead(cpu, 0x0100 + cpu->stkp) << 8;

    cpu->pc++;
    return 0;
}
uint8_t cpuSBC(Smiley6502 *cpu)
{
    cpuFetch(cpu);

    // Operating in 16-bit domain to capture carry out

    // We can invert the bottom 8 bits with bitwise xor
    uint16_t value = ((uint16_t)cpu->fetched) ^ 0x00FF;

    // Notice this is exactly the same as addition from here!
    cpu->temp = (uint16_t)cpu->a + value + (uint16_t)cpuGetFlag(cpu, C);
    cpuSetFlag(cpu, C, cpu->temp & 0xFF00);
    cpuSetFlag(cpu, Z, ((cpu->temp & 0x00FF) == 0));
    cpuSetFlag(cpu, V, (cpu->temp ^ (uint16_t)cpu->a) & (cpu->temp ^ value) & 0x0080);
    cpuSetFlag(cpu, N, cpu->temp & 0x0080);
    cpu->a = cpu->temp & 0x00FF;
    return 1;
}

uint8_t cpuSEC(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, C, true);
    return 0;
}
uint8_t cpuSED(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, D, true);
    return 0;
}
uint8_t cpuSEI(Smiley6502 *cpu)
{
    cpuSetFlag(cpu, I, true);
    return 0;
}
uint8_t cpuSTA(Smiley6502 *cpu)
{
    cpuWrite(cpu, cpu->addr_abs, cpu->a);
    return 0;
}
uint8_t cpuSTX(Smiley6502 *cpu)
{
    cpuWrite(cpu, cpu->addr_abs, cpu->x);
    return 0;
}
uint8_t cpuSTY(Smiley6502 *cpu)
{
    cpuWrite(cpu, cpu->addr_abs, cpu->y);
    return 0;
}
uint8_t cpuTAX(Smiley6502 *cpu)
{
    cpu->x = cpu->a;
    cpuSetFlag(cpu, Z, cpu->x == 0x00);
    cpuSetFlag(cpu, N, cpu->x & 0x80);
    return 0;
}
uint8_t cpuTAY(Smiley6502 *cpu)
{
    cpu->y = cpu->a;
    cpuSetFlag(cpu, Z, cpu->y == 0x00);
    cpuSetFlag(cpu, N, cpu->y & 0x80);
    return 0;
}
uint8_t cpuTSX(Smiley6502 *cpu)
{
    cpu->x = cpu->stkp;
    cpuSetFlag(cpu, Z, cpu->x == 0x00);
    cpuSetFlag(cpu, N, cpu->x & 0x80);
    return 0;
}
uint8_t cpuTXA(Smiley6502 *cpu)
{
    cpu->a = cpu->x;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 0;
}
uint8_t cpuTXS(Smiley6502 *cpu)
{
    cpu->stkp = cpu->x;
    return 0;
}
uint8_t cpuTYA(Smiley6502 *cpu)
{
    cpu->a = cpu->y;
    cpuSetFlag(cpu, Z, cpu->a == 0x00);
    cpuSetFlag(cpu, N, cpu->a & 0x80);
    return 0;
}

//Opcode não oficial
uint8_t cpuXXX(Smiley6502 *cpu)
{
    return 0;
}