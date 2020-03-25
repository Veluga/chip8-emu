#include <stdio.h>
#include <iostream>
#include <fstream>

#include "chip8.hpp"

int main()
{
    Chip8 chip8;
    chip8.loadGame("Games/PONG");
}