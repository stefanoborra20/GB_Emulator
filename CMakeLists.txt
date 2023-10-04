cmake_minimum_required(VERSION 3.23)
project(gbEMU C)

set(CMAKE_C_STANDARD 99)

add_executable(gbEMU main.c
            CPU/cpu.c CPU/cpu.h CPU/in_procs.h
            CART/cartridge.c CART/cartridge.h
            BUS/bus.c BUS/bus.h
            Instructions/instruction.c Instructions/instruction.h)
