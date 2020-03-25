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
    void initialize();
    void loadGame(std::string);
    void emulateCycle();
    bool drawFlag();
    void setKeys();

private:
    byte memory[4096];
    cpu_register V[16];
    pixel gfx[64 * 32];
    key keys[16];
    program_counter pc;
    index_register i;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char delay_timer;
    unsigned char sound_timer;
};

#endif