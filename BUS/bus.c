#include "bus.h"

/**
 * 0000 - 7FFF ROM
 * 8000 - 9FFF Video RAM
 * A000 - BFFF ROM's RAM
 * C000 - DFFF WRAM
 * E000 - FDFF (Not usable area)
 * FE00 - FE0F OAM
 * FEA0 - FEFF (Not usable area)
 * FF00 - FF7F I/O Registers
 * FF80 - FFFE HRAM
 * FFFF - FFFF IE
 * **/


uint8_t read_bus(uint16_t address)
{
    if (address < 0x8000)
        return read_rom(address);

    NOT_IMPL
}

void write_bus(uint16_t address, uint8_t data)
{
    if (address < 0x8000)
        write_rom(address, data);

    NOT_IMPL
}

uint16_t read16_bus(uint16_t address)
{
    // combine two 8 bit data
    return read_bus(address) | (read_bus(address + 1) << 8);
}

void write16_bus(uint16_t address, uint16_t data)
{
    write_rom(address + 1, (data >> 8)); // (data >> 8) & 0xFF ???
    write_bus(address, data & 0xFF);
}