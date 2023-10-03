#include "cpu.h"

#define set_BC(data) cpu.regs.c = data & 0xFF; cpu.regs.b = data >> 8;
#define set_HL(data) cpu.regs.l = data & 0xFF; cpu.regs.h = data >> 8;
#define set_DE(data) cpu.regs.e = data & 0xFF; cpu.regs.d = data >> 8;
#define set_AF(data) cpu.regs.f = data & 0xFF; cpu.regs.a = data >> 8;

#define get_BC() ((cpu.regs.b << 8) | cpu.regs.c)
#define get_HL() ((cpu.regs.h << 8) | cpu.regs.l)
#define get_DE() ((cpu.regs.d << 8) | cpu.regs.e)
#define get_AF() ((cpu.regs.a << 8) | cpu.regs.f)

// should work
#define set_Z(val) cpu.regs.f = ((cpu.regs.f & ~(1 << 7)) | ((val) << 7))
#define set_N(val) cpu.regs.f = ((cpu.regs.f & ~(1 << 6)) | ((val) << 6))
#define set_H(val) cpu.regs.f = ((cpu.regs.f & ~(1 << 5)) | ((val) << 5))
#define set_C(val) cpu.regs.f = ((cpu.regs.f & ~(1 << 4)) | ((val) << 4))

#define get_Z (cpu.regs.f & (1 << 7)) != 0
#define get_N (cpu.regs.f & (1 << 6)) != 0
#define get_H (cpu.regs.f & (1 << 5)) != 0
#define get_C (cpu.regs.f & (1 << 4)) != 0

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

    int auxi;
    uint16_t aux16;
    uint8_t aux8;

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
            cpu.ticks++;
            set_BC((get_BC() + 1))
            cpu.ticks++;
        break;

        case 0x04: /* INC B */
            cpu.regs.b = +1;
            set_Z(!cpu.regs.b); // if value is = 0 -> set flag to 1
            set_N(0);
            set_H((cpu.regs.b & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x05: /* DEC B */
            cpu.regs.b--;
            set_Z(!cpu.regs.b);
            set_N(1);
            set_H((cpu.regs.b & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x06: /* LD B, d8 */
            cpu.regs.b = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x07: /* RLCA (Rotate Left Circular) */
            aux8 = cpu.regs.a;
            cpu.regs.a = (cpu.regs.a << 1) | (aux8 >> 7);
            set_Z(0);
            set_N(0);
            set_H(0);
            set_C(aux8 > 0xFF);
            cpu.ticks++;
        break;

        case 0x08: /* LD (a16), SP */
            cpu.ticks += 3;
            write16_bus(read16_bus(cpu.regs.pc), cpu.regs.sp);
            cpu.regs.pc += 2;
            cpu.ticks += 2;
        break;

        case 0x09: /* ADD HL, BC */
            auxi = get_HL() + get_BC();
            set_N(0);
            set_C(auxi >= 0xFFFF); // if result is >= 65535 (0xFFFF) means there's carry
            set_H((auxi & 0xFFF) < (get_HL() & 0xFFF));
            set_HL(auxi & 0xFFFF)
        break;

        case 0xA: /* LD A, (BC) */



        break;


        default:
            NOT_IMPL
    }

    return true;
}