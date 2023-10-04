#ifndef GBEMU_CPU_H
#define GBEMU_CPU_H

#include <stdint.h>
#include <stdbool.h>

#include "../BUS/bus.h"

typedef enum {H, L, D, E, B, C, A, F} reg_type;

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

    bool is_halted;
    unsigned int ticks;
} CPU;

void cpu_init();

bool cpu_cycle();

#endif
