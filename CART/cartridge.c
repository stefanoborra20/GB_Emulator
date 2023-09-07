#include "cartridge.h"

bool load_rom(char* file_path, Rom* rom)
{
    FILE* file = fopen(file_path, "r");

    if (!file)
        return false;

    fseek(file, 0, SEEK_END);
    rom->rom_size = ftell(file);

    rewind(file);

    rom->rom_data = malloc(rom->rom_size);
    fread(rom->rom_data, sizeof(rom->rom_size), 1, file);

    fclose(file);

    return true;
}

uint16_t read16_rom(Rom* rom, uint16_t address)
{
    return rom->rom_data[address];
}

void write_rom(Rom* rom, uint16_t data)
{
    // todo
}