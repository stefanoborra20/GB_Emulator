#include <stdio.h>
#include "CPU/cpu.h"
#include  "CART/cartridge.h"


int main()
{
    CPU cpu;
    Rom rom = { 0 };

    // sostituisci questo con una finestra per selezionare il file (windows.h library ???)
    if (load_rom("INSERISCI ROM PATH", &rom) == false)
        printf("Cartridge loading error");



    return 0;
}
