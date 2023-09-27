#ifndef GBEMU_BUS_H
#define GBEMU_BUS_H

#include <stdint.h>
#include <stdbool.h>

#include "../CART/cartridge.h"

#define NOT_IMPL { printf("NOT IMPLEMENTED"); }

uint8_t read_bus(uint16_t address);

void write_bus(uint16_t address, uint8_t data);

uint16_t read16_bus(uint16_t address);

void write16_bus(uint16_t address, uint16_t data);

#endif