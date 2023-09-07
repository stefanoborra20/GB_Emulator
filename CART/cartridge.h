#ifndef GBEMU_CARTRIDGE_H
#define GBEMU_CARTRIDGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char* filename;
    uint32_t rom_size;
    uint8_t* rom_data;
} Rom;

bool load_rom(char* file_path, Rom* rom);

uint8_t read8_rom(Rom*, uint16_t address);
uint16_t read16_rom(Rom*, uint16_t address);
void write_rom(Rom*, uint16_t data);

#endif
