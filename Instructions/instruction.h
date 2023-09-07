#ifndef GBEMU_INSTRUCTION_H
#define GBEMU_INSTRUCTION_H

typedef enum
{
    NONE
} AM;

typedef enum
{
    A
} REG;

typedef enum
{
    NOP
} NAME;

typedef struct
{
    NAME name;
    REG reg_1;
    REG reg2;
    AM adrr_mode;

} Instruction;



#endif
