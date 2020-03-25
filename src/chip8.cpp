#include "chip8.hpp"
#include <fstream>
#include <iostream>

Chip8::Chip8() {}

void Chip8::loadFontset()
{
    // Stub
}

void Chip8::emulateCycle()
{
    Chip8::opcode op = this->fetch();
    switch (op & 0xF000)
    {
    case (0x6000):
        this->V[(op & 0x0F00) >> 8] = op & 0x00FF;
        break;
    default:
        std::cout << "Unknown opcode " << std::hex << op << "\n";
    }
    // Decode
    // Execute
    // Update timers
}

bool Chip8::drawFlag()
{
    return false;
}

void Chip8::setKeys()
{
    // Stub
}

void Chip8::loadGame(std::string name)
{
    std::ifstream source;
    byte b;
    unsigned short addr = 0x200;
    source.open(name, std::ios::binary);
    if (source.is_open())
    {
        while (source.read(reinterpret_cast<char *>(&b), 1))
        {
            this->memory[addr++] = b;
        }
        source.close();
    }
}

Chip8::opcode Chip8::fetch()
{
    Chip8::byte hi = this->memory[this->pc++];
    Chip8::byte lo = this->memory[this->pc++];
    return (hi << 8) | lo;
}