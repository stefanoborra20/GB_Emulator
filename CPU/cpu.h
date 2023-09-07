#ifndef GBEMU_CPU_H
#define GBEMU_CPU_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t h;
    uint8_t l;

    uint8_t d;
    uint8_t e;

    uint8_t b;
    uint8_t c;

    uint8_t a;
    uint8_t f;

    uint16_t sp;
    uint16_t pc;
} Registers;

typedef struct
{
    Registers regs;

    // fetch
    uint16_t fetch;
    uint16_t address_dest;
    uint8_t opcode;

    bool is_halted;
    unsigned int cycles;
} CPU;

void cpu_init(CPU*);

bool cpu_cycle(CPU*);

#endif
