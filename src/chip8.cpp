#include "chip8.hpp"
#include <fstream>
#include <iostream>

Chip8::Chip8()
{
    std::random_device rd;
    this->rand_engine = std::mt19937(rd());
    this->rand_dist = std::uniform_int_distribution<>(0, 256);
}

void Chip8::loadFontset()
{
    for (int i = 0; i < 80; i++)
    {
        this->memory[this->fontset_base_address + i] = this->fontset[i];
    }
}

void Chip8::emulateCycle()
{
    Chip8::opcode op = this->fetch();
    switch (op & 0xF000)
    {
    case (0x0000):
    {
        switch (op & 0x00FF)
        {
        case (0x00EE):
            // 0x00EE returns from a subroutine.
            this->pc = this->stack[--this->sp];
            break;
        }
        break;
    }
    case (0x1000):
        // 0x1NNN jumps to address NNN (GOTO).
        this->pc = (op & 0x0FFF);
        break;
    case (0x2000):
        // 2NNN calls subroutine at NNN.
        this->stack[this->sp++] = this->pc + 2; // Resume execution with subsequent instruction after returning
        this->pc = op & 0x0FFF;
        break;
    case (0x3000):
        // 0x3XNN skips the next instruction if VX equals NN.
        if (this->V[(op & 0x0F00) >> 8] == (op & 0x00FF))
        {
            this->pc += 2;
        }
        break;
    case (0x6000):
        // 0x6XNN sets VX to NN.
        this->V[(op & 0x0F00) >> 8] = op & 0x00FF;
        break;
    case (0x7000):
        // 0x7XNN adds NN to VX. (Carry flag is not changed)
        this->V[(op & 0x0F00) >> 8] += (op & 0x00FF);
    case (0xA000):
        // 0xANNN sets I to the address NNN.
        this->i = op & 0x0FFF;
        break;
    case (0xC000):
        // 0xCXNN sets VX to the result of a bitwise and operation on a random number with mask NN
        this->V[(op & 0x0F00) >> 8] = this->rand_dist(this->rand_engine) & (op & 0x00FF);
        break;
    case (0xD000):
    {
        /*
        * 0xDXYN draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
        * Each row of 8 pixels is read as bit-coded starting from memory location I. 
        * I value doesn’t change after the execution of this instruction. 
        * VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
        * and to 0 if that doesn’t happen
        */
        int base_x = this->V[(op & 0x0F00) >> 8];
        int base_y = this->V[(op & 0x00F0) >> 8];
        this->V[0xF] = 0;
        for (int y_offset = 0; y_offset < (op & 0x000F); y_offset++)
        {
            byte sprite_row = this->memory[this->i + y_offset];
            for (int x_offset = 0; x_offset < 8; x_offset++)
            {
                if (sprite_row & (0x80 >> x_offset))
                {
                    // Pixel has to change value
                    // Set pixel collision register if set bit is unset
                    this->V[0xF] |= this->gfx[(base_y + y_offset) * 64 + base_x + x_offset];
                    this->gfx[(base_y + y_offset) * 64 + base_x + x_offset] ^= 1;
                }
            }
        }
        this->drawFlag = true;
        break;
    }
    case (0xE000):
    {
        switch (op & 0x00FF)
        {
        case (0x00A1):
            // 0xEXA1 skips the next instruction if the key stored in VX isn't pressed.
            break;
        }
        break;
    }
    case (0xF000):
    {
        switch (op & 0x00FF)
        {
        case (0x0007):
            // 0xFX07 sets VX to the value of the delay timer.
            this->V[(op & 0x0F00) >> 8] = this->delay_timer;
            break;
        case (0x0015):
            // 0xFX15 sets the delay timer to VX.
            this->delay_timer = this->V[(op & 0x0F00) >> 8];
            break;
        case (0x0018):
            // 0xFX18 sets the sound timer to VX.
            this->sound_timer = this->V[(op & 0x0F00) >> 8];
            break;
        case (0x0029):
            /*
             * 0xFX29 sets I to the location of the sprite for the character in VX. 
             * Characters 0-F (in hexadecimal) are represented by a 4x5 font. 
             */
            this->i = this->fontset_base_address + ((op & 0x0F00) >> 8) * 5;
            break;
        case (0x0033):
        {
            /*
            * 0xFX33 stores the binary-coded decimal representation of VX
            * with the most significant of three digits at the address in I 
            * the middle digit at I plus 1
            * and the least significant digit at I plus 2.
            */
            int target_reg = (op & 0x0F00) >> 8;
            this->memory[this->i] = (int)this->V[target_reg] / 100;
            this->memory[this->i + 1] = ((int)this->V[target_reg] % 100) / 10;
            this->memory[this->i + 2] = ((int)this->V[target_reg] % 10);
            break;
        }
        case (0x0065):
            /*
             * 0xFX65 fills V0 to VX (including VX) with values from memory starting at address I. 
             * The offset from I is increased by 1 for each value written, but I itself is left unmodified.
             */
            for (int reg_idx = 0; reg_idx <= ((op & 0x0F00) >> 8); reg_idx++)
            {
                this->V[reg_idx] = this->memory[this->i + reg_idx];
            }
            break;
        default:
            this->displayGraphics();
            this->printState();
            std::cout
                << "Unknown opcode " << std::hex << op << "\n";
            exit(1);
        }
        break;
    }
    default:
        this->displayGraphics();
        this->printState();
        std::cout
            << "Unknown opcode " << std::hex << op << "\n";
        exit(1);
    }

    if (this->delay_timer > 0)
    {
        this->delay_timer--;
    }
    if (this->sound_timer > 0)
    {
        this->sound_timer--;
    }
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
        std::cout << std::setfill('0') << std::setw(3) << std::right << std::hex << this->stack[frame] << " ";
        std::cout << "\n";
    }
    std::cout << "\nStack Pointer: " << this->sp << "\n";

    std::cout << "Program Counter: " << this->pc << "\n";

    std::cout << "\nTimers:\n";
    std::cout << "Delay: ";
    print_byte(this->delay_timer);
    std::cout << "\n";
    std::cout << "Sound: ";
    print_byte(this->sound_timer);
    std::cout << "\n";
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