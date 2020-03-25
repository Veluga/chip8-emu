#include <stdio.h>
#include <iostream>
#include <fstream>

#include "chip8.hpp"

int main()
{
    Chip8 chip8;
    chip8.loadGame("Games/PONG");

    for (;;)
    {
        chip8.emulateCycle();

        if (chip8.drawFlag())
        {
            // Draw Graphics
        }

        chip8.setKeys();
    }
    return 0;
}