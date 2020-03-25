#include <stdio.h>
#include <iostream>
#include <fstream>

int main()
{
    std::ifstream game;
    unsigned char lo;
    unsigned char hi;
    unsigned short opcode;
    game.open("Games/PONG", std::ios::binary);
    if (game.is_open())
    {
        while (game.read(reinterpret_cast<char *>(&hi), 1))
        {
            game.read(reinterpret_cast<char *>(&lo), 1);
            opcode = (hi << 8) | lo;
            std::cout << std::setfill('0') << std::setw(4) << std::right << std::hex << opcode << "\n";
        }
        game.close();
    }
}