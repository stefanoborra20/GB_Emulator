#include "cartridge.h"

static Rom rom;

const char* get_cart_type_name()
{
    if (rom.header->rom_type <= 0x22)
        return ROM_TYPES[rom.header->rom_type];

    return "UNKNOWN";
}

const char* get_cart_lic_name()
{
    if (rom.header->rom_type <= 0xA4)
        return LIC_CODE[rom.header->lic_code];

    return "UNKNOWN";
}

bool load_rom(char* file_path)
{
    FILE* file = fopen(file_path, "r");

    if (!file)
        return false;

    fseek(file, 0, SEEK_END);
    rom.size = ftell(file);

    rewind(file);

    rom.data = malloc(rom.size);
    fread(rom.data, rom.size, 1, file);

    fclose(file);

    rom.header = malloc(sizeof(rom_header));
    rom.header = (rom_header*)(rom.data + 0x100);
    rom.header->title[15] = 0;

    printf("\tCARTRIDGE INFO\n");
    printf("\tTitle    :\t%s\n", rom.header->title);
    printf("\tType     :\t%s\n", get_cart_type_name());
    printf("\tROM Size :\t%d Kb\n", 32 << rom.header->rom_size);
    printf("\tLIC Code :\t%s\n", get_cart_lic_name());

    uint16_t tmp = 0, i;
    for (i = 0x134; i <= 0x014C; i++)
        tmp = tmp - rom.data[tmp] - 1;

    printf("\tChecksum :\t%s", (tmp & 0xFF) ? "PASSED" : "FAILED");

    return true;
}

uint8_t read_rom(uint16_t address)
{
    return rom.data[address];
}

void write_rom(uint16_t address, uint16_t data)
{
    printf("writing data in (%04X)\n", address);
}












