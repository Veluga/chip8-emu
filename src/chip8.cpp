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
        // 0x6XNN sets VX to NN.
        this->V[(op & 0x0F00) >> 8] = op & 0x00FF;
        break;
    case (0xA000):
        // 0xANNN sets I to the address NNN.
        this->i = op & 0x0FFF;
        break;
    case (0xD000):
        // TODO Draw sprite
        break;
    case (0x2000):
        // 2NNN calls subroutine at NNN.
        this->stack[this->sp++] = (this->pc + 2); // Add 2 to prevent infinite loop
        this->pc = op & 0x0FFF;
        break;
    default:
        this->printState();
        std::cout << "Unknown opcode " << std::hex << op << "\n";
        exit(1);
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

void Chip8::printState()
{
    auto print_byte = [](Chip8::byte b) {
        std::cout << std::setfill('0') << std::setw(2) << std::right << std::hex << (unsigned short)b << " ";
    };

    std::cout << "Memory:\n";
    for (int row = 0; row < 128; row++)
    {
        std::cout << "0x" << std::setfill('0') << std::setw(3) << std::right << std::hex << row * 32 << ": ";
        for (int col = 0; col < 32; col++)
        {
            print_byte(this->memory[row * 32 + col]);
        }
        std::cout << "\n";
    }

    std::cout << "\nRegisters:\n";
    for (int reg = 0; reg < 16; reg++)
    {
        std::cout << "V" << std::hex << reg << ": ";
        print_byte(this->V[reg]);
        std::cout << "\n";
    }
    std::cout << "I: " << this->i << "\n";

    std::cout << "\nKeys:\n";
    for (int key = 0; key < 16; key++)
    {
        std::cout << "Key " << std::hex << key << ": ";
        print_byte(this->keys[key]);
        std::cout << "\n";
    }

    // Stack and Stack Pointer
    std::cout << "\nStack:\n";
    for (int frame = 0; frame < 16; frame++)
    {
        std::cout << "Frame " << std::hex << frame << ": ";
        print_byte(this->keys[frame]);
        std::cout << "\n";
    }
    std::cout << "\nStack Pointer: " << this->sp << "\n";

    std::cout << "Program Counter: " << this->pc << "\n";

    std::cout << "\nTimers:\n";
    std::cout << "Delay: " << this->delay_timer << "\n";
    std::cout << "Sound: " << this->sound_timer << "\n";
}

void Chip8::displayGraphics()
{
    for (int row = 0; row < 64; row++)
    {
        for (int col = 0; col < 32; col++)
        {
            std::cout << (unsigned short)this->gfx[row * 32 + col] << " ";
        }
        std::cout << "\n";
    }
}