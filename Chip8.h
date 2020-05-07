//
// Created by Ahmad Beirkdar on 2020-05-06.
//

#ifndef CHIP8_INTERPRETER_CHIP8_H
#define CHIP8_INTERPRETER_CHIP8_H


#include <cstdint>

class Chip8 {
public:
    Chip8();

private:
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[16];
    uint8_t DelayTimer;
    uint8_t SoundTimer;
    uint8_t keys[16];
    uint32_t display[64*32];
    uint16_t opcode;

};


#endif //CHIP8_INTERPRETER_CHIP8_H
