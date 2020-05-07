//
// Created by Ahmad Beirkdar on 2020-05-06.
//

#include "Chip8.h"

Chip8::Chip8()
    : pc(START_ADDR){

    for(auto i = 0; i < 80; i++)
        memory[FONT_ADRR + i] = chip8_fontset[i];

}
// disp_clear()
void Chip8::OP_00E0() {
    for(int i = 0; i < 32*64; i++)
        display[i] = 0;
}

// Return
void Chip8::OP_00EE() {
    pc = stack[--sp];
}

// goto NNN
void Chip8::OP_1NNN() {
    pc = opcode & 0x0FFFu;
}

// Call subroutine at NNN
void Chip8::OP_2NNN() {
    stack[sp++] = pc;
    pc = opcode & 0x0FFFu;
}

// Skips the next instruction if VX equals NN
void Chip8::OP_3XKK() {
    if(registers[(opcode & 0x0F00u) >> 8u] == (opcode & 0x00FFu))
        pc+=2;
}

// Skips the next instruction if VX doesn't equal NN
void Chip8::OP_4XKK() {
    if(registers[(opcode & 0x0F00u) >> 8u] != (opcode & 0x00FFu))
        pc+=2
}

// Skips the next instruction if VX equals VY
void Chip8::OP_5XY0() {
    if(registers[(opcode & 0x0F00u) >> 8u] == registers[(opcode & 0x00F0u) >> 4u])
        pc+=2
}
// Set Vx = kk.
void Chip8::OP_6XKK() {
    registers[(opcode & 0x0F00u) >> 8u] = (opcode & 0x00FFu)
}

// Set Vx = Vx + kk.
void Chip8::OP_7XKK() {
    registers[(opcode & 0x0F00u) >> 8u] += (opcode & 0x00FFu)
}

// Set Vx = Vy
void Chip8::OP_8XY0() {
    registers[(opcode & 0x0F00u) >> 8u] = registers[(opcode & 0x00F0u) >> 4u]
}


