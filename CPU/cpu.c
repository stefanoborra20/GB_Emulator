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

#define get_Z ((cpu.regs.f & (1 << 7)) != 0)
#define get_N ((cpu.regs.f & (1 << 6)) != 0)
#define get_H ((cpu.regs.f & (1 << 5)) != 0)
#define get_C ((cpu.regs.f & (1 << 4)) != 0)

#define LDRR(a, b) a = b; cpu.ticks++;

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
            set_BC(read16_bus(cpu.regs.pc))
            cpu.regs.pc += 2;
            cpu.ticks += 3;
        break;

        case 0x02: /* LD (BC), A */
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
            cpu.regs.b++;
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

        case 0x0A: /* LD A, (BC) */
            cpu.ticks++;
            cpu.regs.a = read_bus(get_BC());
            cpu.ticks++;
        break;

        case 0x0B: /* DEC BC */
            cpu.ticks++;
            set_BC((get_BC() - 1))
            cpu.ticks++;
        break;

        case 0x0C: /* INC C -> maybe create a function INC */
            cpu.regs.c++;
            set_Z(!cpu.regs.c);
            set_N(0);
            set_H((cpu.regs.c & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x0D: /* DEC C */
            cpu.regs.c--;
            set_Z(!cpu.regs.c);
            set_N(1);
            set_H((cpu.regs.c & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x0E: /* LD C, d8 */
            cpu.regs.c = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x0F: /* RRCA */
            aux8 = cpu.regs.a;
            cpu.regs.a = (cpu.regs.a >> 1) | (aux8 << 7);
            set_Z(0);
            set_N(0);
            set_H(0);
            set_C(aux8 > 0xFF);
            cpu.ticks++;
        break;

        case 0x10 /* ??? */:
        break;

        case 0x11: /* LD DE, d16 */
            set_DE(read16_bus(cpu.regs.pc))
            cpu.regs.pc += 2;
            cpu.ticks += 3;
        break;

        case 0x12: /* LD (DE), A */
            cpu.ticks++;
            write_bus(get_DE(), cpu.regs.a);
            cpu.ticks++;
        break;

        case 0x13: /* INC DE */
            cpu.ticks++;
            set_DE((get_DE() + 1))
            cpu.ticks++;
        break;

        case 0x14: /* INC D */
            cpu.regs.d++;
            set_Z(!cpu.regs.d);
            set_N(0);
            set_H((cpu.regs.d & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x15: /* DEC D */
            cpu.regs.d--;
            set_Z(!cpu.regs.d);
            set_N(1);
            set_H((cpu.regs.d & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x16: /* LD D, d8 */
            cpu.regs.d = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x17: /* RLA */
            aux8 = get_C;
            set_C(cpu.regs.a & 0x80);
            cpu.regs.a = (cpu.regs.a << 1) | aux8;
            set_Z(0);
            set_N(0);
            set_H(0);
            cpu.ticks++;
        break;

        case 0x18: /* JR r8 */
            cpu.regs.pc += (signed) read_bus(cpu.regs.pc++);
            cpu.ticks += 3;
        break;

        case 0x19: /* ADD HL, DE */
            auxi = get_HL() + get_DE();
            set_N(0);
            set_C(auxi >= 0xFFFF); // if result is >= 65535 (0xFFFF) means there's carry
            set_H((auxi & 0xFFF) < (get_HL() & 0xFFF));
            set_HL(auxi & 0xFFFF)
            cpu.ticks++;
        break;

        case 0x1A: /* LD A, (DE) */
            cpu.ticks++;
            cpu.regs.a = read_bus(get_DE());
            cpu.ticks++;
        break;

        case 0x1B: /* DEC DE */
            cpu.ticks++;
            set_BC((get_DE() - 1))
            cpu.ticks++;
        break;

        case 0x1C: /* INC E */
            cpu.regs.e++;
            set_Z(!cpu.regs.e);
            set_N(0);
            set_H((cpu.regs.e & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x1D: /* DEC E */
            cpu.regs.e--;
            set_Z(!cpu.regs.e);
            set_N(1);
            set_H((cpu.regs.e & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x1E: /* LD E, d8 */
            cpu.regs.e = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x1F: /* RRA */
            aux8 = get_C;
            set_C(cpu.regs.a & 1);
            cpu.regs.a = (cpu.regs.a >> 1) | (aux8 << 7);
            set_Z(0);
            set_N(0);
            set_H(0);
            cpu.ticks++;
        break;

        case 0x20: /* JR NZ, r8 */
            if (get_Z == 0)
            {
                cpu.regs.pc = (signed) read_bus(cpu.regs.pc);
                cpu.ticks += 3;
            }
            else
            {
                cpu.regs.pc++;
                cpu.ticks += 2;
            }
        break;

        case 0x21: /* LD HL, d16 */
            set_HL(read16_bus(cpu.regs.pc))
            cpu.regs.pc += 2;
            cpu.ticks += 3;
        break;

        case 0x22: /* LD (HL+), A */
            cpu.ticks++;
            write_bus(get_HL(), cpu.regs.a);
            set_HL((get_HL() + 1))
            cpu.ticks++;
        break;

        case 0x23: /* INC HL */
            cpu.ticks++;
            set_DE((get_DE() + 1))
            cpu.ticks++;
        break;

        case 0x24: /* INC H */
            cpu.regs.h++;
            set_Z(!cpu.regs.h);
            set_N(0);
            set_H((cpu.regs.h & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x25: /* DEC H */
            cpu.regs.h--;
            set_Z(!cpu.regs.h);
            set_N(1);
            set_H((cpu.regs.h & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x26: /* LD H, d8 */
            cpu.regs.h = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x27: /* DAA */
            aux8 = cpu.regs.a;

            if (get_N)
            {
                if (get_H)
                    aux8 = (aux8 - 0x06) & 0xFF;

                if (get_C)
                    aux8 -= 0x60;
            }
            else
            {
                if (get_H || (aux8 & 0xF) > 9)
                    aux8 += 0x06;

                if (get_C || aux8 > 0x9F)
                    aux8 += 0x60;
            }

            cpu.regs.a = aux8;
            set_H(0);
            set_Z(!cpu.regs.a);

            if (aux8 > 0x99)
                set_C(1);

            cpu.ticks++;

        break;

        case 0x28: /* JR Z, r8 */
            if (get_Z == 1)
            {
                cpu.regs.pc += (signed) read_bus(cpu.regs.pc);
                cpu.ticks += 3;
            }
            else
            {
                cpu.regs.pc++;
                cpu.ticks += 2;
            }
        break;

        case 0x29: /* ADD HL, HL */
            auxi = get_HL() + get_HL();
            set_N(0);
            set_C(auxi >= 0xFFFF); // if result is >= 65535 (0xFFFF) means there's carry
            set_H((auxi & 0xFFF) < (get_HL() & 0xFFF));
            set_HL(auxi)
            cpu.ticks += 2;
        break;

        case 0x2A: /* LD A, (HL+) */
            cpu.ticks++;
            cpu.regs.a = read_bus(get_HL());
            set_HL((get_HL() + 1))
            cpu.ticks++;
        break;

        case 0x2B: /* DEC HL */
            set_HL((get_HL() - 1))
            cpu.ticks += 2;
        break;

        case 0x2C: /* INC L */
            cpu.regs.l++;
            set_Z(!cpu.regs.l);
            set_N(0);
            set_H((cpu.regs.l & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x2D: /* DEL L */
            cpu.regs.l--;
            set_Z(!cpu.regs.l);
            set_N(1);
            set_H((cpu.regs.l & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x2E: /* LD L, d8 */
            cpu.regs.l = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x2F: /* CPL */
            cpu.regs.a = ~cpu.regs.a;
            set_N(0);
            set_H(0);
            cpu.ticks++;
        break;

        case 0x30: /* JR NC, r8 */
            if(get_C == 0)
            {
                cpu.regs.pc += (signed) read_bus(cpu.regs.pc);
                cpu.ticks += 3;
            }
            else
            {
                cpu.regs.pc++;
                cpu.ticks += 2;
            }
        break;

        case 0x31: /* LD SP, d16 */
            cpu.regs.sp = read16_bus(cpu.regs.pc);
            cpu.regs.pc += 2;
            cpu.ticks += 3;
        break;

        case 0x32: /* LD (HL-), A */
            cpu.ticks++;
            write_bus(get_HL(), cpu.regs.a);
            set_HL((get_HL() - 1))
            cpu.ticks++;
        break;

        case 0x33: /* INC SP */
            cpu.regs.sp += 2;
            cpu.ticks += 2;
        break;

        case 0x34: /* INC (HL) */
            cpu.ticks++;
            aux8 = read_bus(get_HL());
            aux8++;
            cpu.ticks++;
            write_bus(get_HL(), aux8);
            set_Z(!aux8);
            set_N(0);
            set_H((aux8 & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x35: /* DEC (HL) */
            cpu.ticks++;
            aux8 = read_bus(get_HL());
            aux8--;
            cpu.ticks++;
            write_bus(get_HL(), aux8);
            set_Z(!aux8);
            set_N(1);
            set_H((aux8 & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x36: /* LD (HL), d8 */
            cpu.ticks += 2;
            write_bus(get_HL(), read_bus(cpu.regs.pc++));
            cpu.ticks++;
        break;

        case 0x37: /* SCF */
            set_N(0);
            set_H(0);
            set_C(1);
            cpu.ticks++;
        break;

        case 0x38: /* JR C, r8 */
            if (get_C)
            {
                cpu.regs.pc += (signed) read_bus(cpu.regs.pc++);
                cpu.ticks += 3;
            }
            else
            {
                cpu.regs.pc++;
                cpu.ticks++;
            }
        break;

        case 0x39: /* ADD HL, SP */
            auxi = get_HL() + cpu.regs.sp;
            set_N(0);
            set_C(auxi >= 0xFFFF); // if result is >= 65535 (0xFFFF) means there's carry
            set_H((auxi & 0xFFF) < (get_HL() & 0xFFF));
            set_HL(auxi)
            cpu.ticks += 2;
        break;

        case 0x3A: /* LD A, (HL-) */
            cpu.ticks++;
            cpu.regs.a = read_bus(get_HL());
            set_HL((get_HL() - 1))
            cpu.ticks++;
        break;

        case 0x3B: /* DEC SP */
            cpu.regs.sp--;
            cpu.ticks += 2;
        break;

        case 0x3C: /* INC A */
            cpu.regs.a++;
            set_Z(!cpu.regs.a);
            set_N(0);
            set_H((cpu.regs.a & 0xF) == 0);
            cpu.ticks++;
        break;

        case 0x3D: /* DEC A */
            cpu.regs.a--;
            set_Z(!cpu.regs.a);
            set_N(1);
            set_H((cpu.regs.a & 0xF) == 0xF);
            cpu.ticks++;
        break;

        case 0x3E: /* LD A, d8 */
            cpu.regs.a = read_bus(cpu.regs.pc++);
            cpu.ticks += 2;
        break;

        case 0x3F: /* CCF */
            set_N(0);
            set_H(0);
            set_C(!get_C);
        break;

        case 0x40: /* LD B, B */
            LDRR(cpu.regs.b, cpu.regs.b)
        break;

        case 0x41: /* LD B, C */
            LDRR(cpu.regs.b, cpu.regs.c)
        break;

        case 0x42: /* LD B, D */
            LDRR(cpu.regs.b, cpu.regs.d)
        break;

        case 0x43: /* LD B, E */
            LDRR(cpu.regs.b, cpu.regs.e)
        break;

        case 0x44: /* LD B, H */
            LDRR(cpu.regs.b, cpu.regs.h)
        break;

        case 0x45: /* LD B, L */
            LDRR(cpu.regs.b, cpu.regs.l)
        break;

        case 0x46: /* LD B, (HL) */
            cpu.ticks++;
            cpu.regs.b = read_bus(get_HL());
            cpu.ticks++;
        break;

        case 0x47: /* LD B, A */
            LDRR(cpu.regs.b, cpu.regs.a)
        break;

        case 0x48: /* LD C, B */
            LDRR(cpu.regs.c, cpu.regs.b)
        break;

        case 0x49: /* LD C, C */
            LDRR(cpu.regs.c, cpu.regs.c)
        break;

        case 0x4A: /* LD C, D */
            LDRR(cpu.regs.c, cpu.regs.d)
        break;

        case 0x4B: /* LD C, E */
            LDRR(cpu.regs.c, cpu.regs.e)
        break;

        case 0x4C: /* LD C, H */
            LDRR(cpu.regs.c, cpu.regs.h)
        break;

        case 0x4D: /* LD C, L */
            LDRR(cpu.regs.c, cpu.regs.l)
        break;

        case 0x4E: /* LD C, (HL) */
            cpu.ticks++;
            cpu.regs.c = read_bus(get_HL());
            cpu.ticks++;
        break;

        case 0x4F: /* LD C, A */
            LDRR(cpu.regs.c, cpu.regs.a)
        break;



        default:
            NOT_IMPL
    }

    return true;
}