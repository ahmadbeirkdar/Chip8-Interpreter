//
// Created by Ahmad Beirkdar on 2020-05-06.
//

#ifndef CHIP8_INTERPRETER_CHIP8_H
#define CHIP8_INTERPRETER_CHIP8_H


#include <cstdint>

class Chip8 {
public:
    Chip8();
    void LoadGame(char* filename);
    void emulate();

    uint8_t keys[16]{};

    uint32_t display[64*32]{};
private:
    const unsigned int START_ADDR = 0x200;
    const unsigned int FONT_ADRR = 0x50;
    const unsigned int VIDEO_HEIGHT = 32;
    const unsigned int VIDEO_WIDTH = 64;

    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc {};
    uint8_t sp {};
    uint16_t stack[12]{};
    uint8_t DelayTimer{};
    uint8_t SoundTimer{};
    uint16_t opcode{};

    // Instructions starts with unique number or char. put in the main array. Unique start so & 0xF000u
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XKK();
    void OP_4XKK();
    void OP_5XY0();
    void OP_6XKK();
    void OP_7XKK();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXKK();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();

    // Instruction starts with 00E, Unique ending so & 0x000Fu. Put in one array
    void OP_00E0();
    void OP_00EE();

    // Instruction starts with 8XY, Unique ending so & 0x000Fu. Put in one array
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();

    // Instructions starts with FX, Unique ending so & 0x00FFu. Put in one array
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();

    // Array to pointers of functions
    // https://austinmorlan.com/posts/chip8_emulator/#function-pointer-table
    // To familiarize myself with arrays of pointers to functions instead of using a big switch statement
    void Table0();
    void Table8();
    void TableE();
    void TableF();

    typedef void (Chip8::*Chip8Func)();
    Chip8Func table[0xF + 1]{};
    Chip8Func table0[0xE + 1]{};
    Chip8Func table8[0xE + 1]{};
    Chip8Func tableE[0xE + 1]{};
    Chip8Func tableF[0x65 + 1]{};


};



#endif //CHIP8_INTERPRETER_CHIP8_H
