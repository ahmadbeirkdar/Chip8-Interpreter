//
// Created by Ahmad Beirkdar on 2020-05-06.
//

#include "Chip8.h"
#include <cstdlib>

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
        pc+=2;
}

// Skips the next instruction if VX equals VY
void Chip8::OP_5XY0() {
    if(registers[(opcode & 0x0F00u) >> 8u] == registers[(opcode & 0x00F0u) >> 4u])
        pc+=2;
}
// Set Vx = kk.
void Chip8::OP_6XKK() {
    registers[(opcode & 0x0F00u) >> 8u] = (opcode & 0x00FFu);
}

// Set Vx = Vx + kk.
void Chip8::OP_7XKK() {
    registers[(opcode & 0x0F00u) >> 8u] += (opcode & 0x00FFu);
}

// Set Vx = Vy
void Chip8::OP_8XY0() {
    registers[(opcode & 0x0F00u) >> 8u] = registers[(opcode & 0x00F0u) >> 4u];
}

// Set Vx = Vx Or Vy
void Chip8::OP_8XY1() {
    registers[(opcode & 0x0F00u) >> 8u] |= registers[(opcode & 0x00F0u) >> 4u];
}

// Set Vx = Vx AND Vy
void Chip8::OP_8XY2() {
    registers[(opcode & 0x0F00u) >> 8u] &= registers[(opcode & 0x00F0u) >> 4u];
}

// Set Vx = Vx XOR Vy
void Chip8::OP_8XY3() {
    registers[(opcode & 0x0F00u) >> 8u] ^= registers[(opcode & 0x00F0u) >> 4u];
}

// Set Vx = Vx + Vy, set VF = carry. V at 0xF. If sum larger than 255, what a 8bit can hold max then it is carried.
void Chip8::OP_8XY4() {
    if(registers[(opcode & 0x00F0u) >> 4u] > (0xFF - registers[(opcode & 0x0F00u) >> 8u]))
        registers[0xF] = 1;
    else
        registers[0xF] = 0;
    registers[(opcode & 0x0F00u) >> 8u] += registers[(opcode & 0x00F0u) >> 4u];
}

// Set Vx = Vx - Vy, set VF = NOT borrow.
void Chip8::OP_8XY5() {
    if(registers[(opcode & 0x00F0u) >> 4u] > registers[(opcode & 0x0F00u) >> 8u])
        registers[0xF] = 0;
    else
        registers[0xF] = 1;
    registers[(opcode & 0x0F00u) >> 8u] -= registers[(opcode & 0x00F0u) >> 4u];
}

// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
void Chip8::OP_8XY6() {
    registers[0xF] = (registers[(opcode & 0x0F00u) >> 8u] & 0x1u);
    registers[(opcode & 0x0F00u) >> 8u] >>= 1u;
}

// Set Vx = Vy - Vx, set VF = NOT borrow.
void Chip8::OP_8XY7() {
    if(registers[(opcode & 0x0F00u) >> 8u] > registers[(opcode & 0x00F0u) >> 4u])
        registers[0xF] = 0;
    else
        registers[0xF] = 1;
    registers[(opcode & 0x00F0u) >> 4u] -= registers[(opcode & 0x0F00u) >> 8u];
}

// Stores the most significant bit of VX in VF and then shifts VX to the left by 1
void Chip8::OP_8XYE() {
    registers[0xF] = (registers[(opcode & 0x0F00u) >> 8u] >> 7u);
    registers[(opcode & 0x0F00u) >> 8u] <<= 1u;
}

// Skip next instruction if Vx != Vy.
void Chip8::OP_9XY0() {
    if(registers[(opcode & 0x0F00u) >> 8u] != registers[(opcode & 0x00F0u) >> 4u])
        pc+= 2;
}


// Sets I to the address NNN. index
void Chip8::OP_ANNN() {
    index = (opcode & 0x0FFFu);
}

// Jump to location nnn + V0.
void Chip8::OP_BNNN() {
    pc = registers[0] + (opcode & 0x0FFFu);
}

// Set Vx = random byte AND kk.
void Chip8::OP_CXKK() {
    registers[(opcode & 0x0F00u) >> 8u] = ((std::rand() % 0xFFu)&(opcode & 0x00FFu));
}

// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
// The interpreter reads n bytes from memory, starting at the address stored in I.
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
// If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
// See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
void Chip8::OP_DXYN() {
    uint8_t Vx = registers[(opcode & 0x0F00u) >> 8u];
    uint8_t Vy = registers[(opcode & 0x00F0u) >> 4u];
    uint8_t xPos = Vx % VIDEO_WIDTH;
    uint8_t yPos = Vy % VIDEO_HEIGHT;
    uint8_t h = (opcode & 0x000Fu);
    uint8_t pixel;
    registers[0xF] = 0;

    for(unsigned int y = 0; y < h; y++){
        pixel = memory[index + y];
        for(unsigned int x = 0; x < 8; x++){
            if(pixel & (0x80u >> x)){
                if(display[(xPos + x) + VIDEO_WIDTH*(yPos + y)] == 0xFFFFFFFF)
                    registers[0xF] = 1;
                display[(xPos + x) + VIDEO_WIDTH*(yPos + y)] ^= 0xFFFFFFFF;
            }
        }
    }



}

// Skip next instruction if key with the value of Vx is pressed.
void Chip8::OP_EX9E() {
    if(keys[registers[(opcode & 0x0F00u) >> 8u]])
        pc+=2;
}

// Skip next instruction if key with the value of Vx is not pressed.
void Chip8::OP_EXA1() {
    if(!keys[registers[(opcode & 0x0F00u) >> 8u]])
        pc+=2;
}

// Set Vx = delay timer value.
void Chip8::OP_FX07() {
    registers[(opcode & 0x0F00u) >> 8u] = DelayTimer;
}

// Wait for a key press, store the value of the key in Vx.
void Chip8::OP_FX0A() {
    // TODO
}







