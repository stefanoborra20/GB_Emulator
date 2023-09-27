#include "cpu.h"

#define set_BC(data) cpu.regs.c = data & 0xFF; cpu.regs.b = data >> 8;
#define set_HL(data) cpu.regs.l = data & 0xFF; cpu.regs.h = data >> 8;
#define set_DE(data) cpu.regs.e = data & 0xFF; cpu.regs.d = data >> 8;
#define set_AF(data) cpu.regs.f = data & 0xFF; cpu.regs.a = data >> 8;

#define get_BC() ((cpu.regs.b << 8) | cpu.regs.c)
#define get_HL() ((cpu.regs.h << 8) | cpu.regs.l)
#define get_DE() ((cpu.regs.d << 8) | cpu.regs.e)
#define get_AF() ((cpu.regs.a << 8) | cpu.regs.f)

// not sure if it works
#define set_Z(val) cpu.regs.f = ((cpu.regs.f & 1 << 0x7F) | ((val) << 7)
#define set_N(val) cpu.regs.f = ((cpu.regs.f & 1 << 0xBF) | ((val) << 6)
#define set_H(val) cpu.regs.f = ((cpu.regs.f & 1 << 0xDF) | ((val) << 5)
#define set_C(val) cpu.regs.f = ((cpu.regs.f & 1 << 0xEF) | ((val) << 4)

static CPU cpu;

void cpu_init()
{
    cpu.regs.pc = 0x100;
    cpu.regs.a = 0x01;
}


bool cpu_cycle()
{
    uint16_t fetched_data;
    unsigned char next_instruction;

    uint16_t aux;

    if (cpu.is_halted)
    {
        cpu.ticks++;
        return false;
    }


    next_instruction = read_bus(cpu.regs.pc++);

    printf("\nIN: %02X   PC: %04X", next_instruction, cpu.regs.pc);

    switch (next_instruction)
    {
        case 0x00: /* NOP */
            cpu.ticks++;
            break;

        case 0x01: /* LD BC, d16 */
            fetched_data = read16_bus(cpu.regs.pc);
            set_BC(fetched_data)
            cpu.regs.pc += 2;
            cpu.ticks += 3;
            break;

        case 0x02: /* LD (BC),A */
            cpu.ticks++;
            write_bus(get_BC(), cpu.regs.a);
            cpu.ticks++;
            break;

        case 0x03: /* INC BC */
            set_BC((get_BC() + 1))
            break;

        case 0x04: /* INC B */
            //cpu.regs.b = +1;
            break;


        default:
            NOT_IMPL
    }

    return true;
}