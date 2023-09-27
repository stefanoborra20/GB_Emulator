#include <stdio.h>
#include "CPU/cpu.h"
#include  "CART/cartridge.h"

#define ROM_PATH "C:\\Users\\stefano\\Desktop\\GameBoy Programs\\ROMs\\Super Mario Land.gb"

int main()
{
    if (load_rom(ROM_PATH) == false)
        printf("Cartridge loading error");

    cpu_init();

    while ( 1 )
    {
        if (!cpu_cycle())
            break;

    }

    return 0;
}
