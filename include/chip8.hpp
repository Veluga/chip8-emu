#ifndef CHIP8EMU_CHIP8_H
#define CHIP8EMU_CHIP8_H

#include <vector>
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
    bool drawFlag();
    void setKeys();

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
};

#endif