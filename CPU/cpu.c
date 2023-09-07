#include "cpu.h"

void cpu_init(CPU* cpu)
{
    cpu->regs.pc = 0x100;
    cpu->regs.a = 0x01;
}

bool cpu_cycle(CPU* cpu)
{
    if (cpu->is_halted) // increment cycle ???
        return false;


    return true;
}