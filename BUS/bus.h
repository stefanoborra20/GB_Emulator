#ifndef GBEMU_BUS_H
#define GBEMU_BUS_H

#include <stdint.h>
#include <stdbool.h>

bool write_bus(uint16_t address, uint8_t byte);

uint8_t read_bus(uint16_t address);

#endif