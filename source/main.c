#include <stdio.h>
#include "Bus.h"
#include "Smiley6502.h"

int main()
{
    Bus *bus = createBus();
    printf("0x%02x\n", bus->ram[0]);
    bus->ram[0] = 2;
    printf("0x%02x\n", bus->ram[0]);
    cpuWrite(bus->cpu, 0, 0x0F);
    printf("Endereco: \n");
    printf("0x%04x\n", 0);
    printf("valor: 0x%02x\n", bus->ram[0]);
    free(bus);
    return 0;
}