#ifndef CHIP8EMU_CHIP8_H
#define CHIP8EMU_CHIP8_H

#include <string>

class Chip8
{
    typedef unsigned short opcode;
    typedef unsigned short index_register;
    typedef unsigned short program_counter;
    typedef unsigned char byte;
    typedef unsigned char cpu_register;
    typedef unsigned char pixel;
    typedef unsigned char key;

public:
    Chip8();
    void loadFontset();
    void loadGame(std::string);
    void emulateCycle();
    void setKeys();
    void printState();
    void displayGraphics();

    bool drawFlag;

private:
    opcode fetch();

    byte memory[4096] = {0};
    cpu_register V[16] = {0};
    pixel gfx[64 * 32] = {0};
    key keys[16] = {0};
    program_counter stack[16] = {0};
    program_counter pc = 0x200;
    index_register i = 0;
    unsigned short sp = 0;
    unsigned char delay_timer = 0;
    unsigned char sound_timer = 0;
    unsigned short fontset_base_address = 0x50;
    pixel fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
};

#endif