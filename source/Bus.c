#include "Bus.h"

struct Bus *createBus()
{
    struct Bus *bus;
    bus = (struct Bus *)malloc(sizeof *bus);

    for (int i; i < 64 * 1024; i++)
        bus->ram[i] = 0x00;

    bus->cpu = createCpu();
    cpuConnectBus(bus->cpu, bus);
    return bus;
}

void busWrite(Bus *bus, uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
        bus->ram[addr] = data;
}

uint8_t busRead(Bus *bus, uint16_t addr, bool bReadOnly)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
        return bus->ram[addr];

    return 0x00;
}