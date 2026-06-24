#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <random>

#include "Types.h"

class Chip8 {
    public:
        Chip8();

        void LoadROM(const char* filename);

        // Chip8 cycle
        void Cycle();

        u8                                  keypad[16]{};
        u32                                 video[64 * 32]{};
        
    private:
        // Table functions
        void Table0();

        void Table8();

        void TableE();

        void TableF();

        void OP_NULL();

        // --------------- opcodes --------------------
        void OP_00E0();
        void OP_00EE();
        void OP_1nnn();
        void OP_2nnn();
        void OP_3xkk();
        void OP_4xkk();
        void OP_5xy0();
        void OP_6xkk();
        void OP_7xkk();
        void OP_8xy0();
        void OP_8xy1();
        void OP_8xy2();
        void OP_8xy3();
        void OP_8xy4();
        void OP_8xy5();
        void OP_8xy6();
        void OP_8xy7();
        void OP_8xyE();
        void OP_9xy0();
        void OP_Annn();
        void OP_Bnnn();
        void OP_Cxkk();
        void OP_Dxyn();
        void OP_Ex9E();
        void OP_ExA1();
        void OP_Fx07();
        void OP_Fx0A();
        void OP_Fx15();
        void OP_Fx18();
        void OP_Fx1E();
        void OP_Fx29();
        void OP_Fx33();
        void OP_Fx55();
        void OP_Fx65();


        u8                                  memory[4096]{};
        u8                                  registers[16]{};

        u16                                 index{};
        u16                                 pc{};

        u8                                  delayTimer{};
        u8                                  soundTimer{};

        u16                                 stack[16]{};
        u8                                  sp{};

        u16                                 opcode{};

        // RNG (Could be achieved by hardware)
        std::default_random_engine          randGen;
        std::uniform_int_distribution<u8>   randByte;

        // Tables
        typedef void (Chip8::*Chip8Func)();
        Chip8Func   table[0xF + 1];
        Chip8Func  table0[0xE + 1];
        Chip8Func  table8[0xE + 1];
        Chip8Func  tableE[0xE + 1];
        Chip8Func tableF[0x65 + 1];
};
#endif
