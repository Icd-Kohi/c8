#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <chrono>
#include <random>
#include "Types.h"

// character sprite = 5 bytes;

namespace Chip8{
    const u8 FONTSET_SIZE = 80;
    const u16 START_ADDRESS = 0x200;
    const u8 VIDEO_WIDTH = 64;
    const u8 VIDEO_HEIGHT = 32;

    
    /*  ------------ SCREEN STARTS AS: -------------
     *  11110000 10010000 10010000 10010000 11110000  // 0
     *  00100000 01100000 00100000 00100000 01110000  // 1
     *  11110000 00010000 11110000 10000000 11110000  // 2
     *  11110000 00010000 11110000 00010000 11110000  // 3
     *  10010000 10010000 11110000 00010000 00010000  // 4
     *  11110000 10000000 11110000 00010000 11110000  // 5
     *  11110000 10000000 11110000 10010000 11110000  // 6
     *  11110000 00010000 00100000 01000000 01000000  // 7
     *  11110000 10010000 11110000 10010000 11110000  // 8
     *  11110000 10010000 11110000 00010000 11110000  // 9
     *  11110000 10010000 11110000 10010000 10010000  // A
     *  11100000 10010000 11100000 10010000 11100000  // B
     *  11110000 10000000 10000000 10000000 11110000  // C
     *  11100000 10010000 10010000 10010000 11100000  // D
     *  11110000 10000000 11110000 10000000 11110000  // E
     *  11110000 10000000 11110000 10000000 10000000  // F
     *  
     *  ------------ Representation  -----------
     *  11110000 // 0
     *  10010000
     *  10010000 
     *  10010000 
     *  11110000  
     *
     *  00100000 
     *  01100000 
     *  00100000 
     *  00100000 
     *  01110000  // 1
     *
     */

    u8 fontset[FONTSET_SIZE] = {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    class Chip8 {
        public:
            Chip8(): randGen(std::chrono::system_clock::now().time_since_epoch().count()){

                pc = START_ADDRESS;

                randByte = std::uniform_int_distribution<uint8_t>(0, 255U);                
            }

            u8                                  registers[16]{};
            u8                                  memory[4096]{};

            u16                                 index{};
            u16                                 pc{};
            u16                                 stack[16]{};
            u8                                  sp{};

            u8                                  delayTimer{};
            u8                                  soundTimer{};

            u8                                  keypad[16]{};
            u32                                 video[64 * 32]{}; // 2048
            u16                                 opcode;

            // RNG (Could be achieved by hardware)
            std::default_random_engine          randGen;
            std::uniform_int_distribution<u8>   randByte; 

            void LoadROM(char const* filename);

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
    };

    // Initialize PC with a constructor
    const u8 FONTSET_START_ADDRESS = 0x50;
    
    // LoadROM Declaration
    void Chip8::LoadROM(char const* filename){

        // Open the file as a stream of binary and move the file pointer to the end
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if(file.is_open()){
            std::streampos size = file.tellg();
            char* buffer = new char[size]; // alloc

            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            for(size_t i = 0; i < FONTSET_SIZE; ++i){
                memory[FONTSET_START_ADDRESS + i] =  fontset[i];
            }
            for(size_t i = 0; i < size; ++i){
                memory[START_ADDRESS + 1] = buffer[i];
            }

            delete[] buffer;
        }
    }

    /*
     *  ------------ OPCODES --------------
     *      3.1 - Standard Chip-8 Instructions
            00E0 - CLS
            00EE - RET
            0nnn - SYS addr
            1nnn - JP addr
            2nnn - CALL addr
            3xkk - SE Vx, byte
            4xkk - SNE Vx, byte
            5xy0 - SE Vx, Vy
            6xkk - LD Vx, byte
            7xkk - ADD Vx, byte
            8xy0 - LD Vx, Vy
            8xy1 - OR Vx, Vy
            8xy2 - AND Vx, Vy
            8xy3 - XOR Vx, Vy
            8xy4 - ADD Vx, Vy
            8xy5 - SUB Vx, Vy
            8xy6 - SHR Vx {, Vy}
            8xy7 - SUBN Vx, Vy
            8xyE - SHL Vx {, Vy}
            9xy0 - SNE Vx, Vy
            Annn - LD I, addr
            Bnnn - JP V0, addr
            Cxkk - RND Vx, byte
            Dxyn - DRW Vx, Vy, nibble
            Ex9E - SKP Vx
            ExA1 - SKNP Vx
            Fx07 - LD Vx, DT
            Fx0A - LD Vx, K
            Fx15 - LD DT, Vx
            Fx18 - LD ST, Vx
            Fx1E - ADD I, Vx
            Fx29 - LD F, Vx
            Fx33 - LD B, Vx
            Fx55 - LD [I], Vx
            Fx65 - LD Vx, [I]
     */

    // OP 00E0: Clear the display; (Setting all of it with zeroes)
    void Chip8::OP_00E0(){
        memset(video, 0, sizeof(video)); // fill the 2048 pixel screen with 0;
    }

    /* 
     * OP 00EE: decrease 1 to the stack pointer, and also move the program counter to the value the sp is pointing to in the stack.
     * better phrasing: Return from a subroutine.
     */
    void Chip8::OP_00EE(){
        --sp;
        pc = stack[sp];
    }
    /*
     * OP 1nnn: Jump to location nnn  -> The program sets the program counter to nnn.
     * "A jump doesn't remember its origin, so no stack interation needed.
    question: how would I even come up on writing something like this code?
     */
    void Chip8::OP_1nnn(){
        u16 address = opcode & 0x0FFFu;
        pc = address;
    }
    
    /*
     * OP 2nnn: Call subroutine at nnn -> calling a subroutine means that we want to return eventually
     */
    void Chip8::OP_2nnn(){
        u16 address = opcode & 0x0FFFu;
        
        stack[sp] = pc;
        ++sp;
        pc = address;
    }
    
    /*
     * OP 3xkk: Skip next instruction if Vx == kk;
     * After `Cycle()`, we can just increment PC by 2 again to skip the next instruction.
     */
    void Chip8::OP_3xkk(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 byte = (opcode & 0x0F00u) >> 8u;

        if(registers[Vx] == byte){
            pc += 2;
        }
    }

    /*
     * OP 4xkk: Skip next instruction if Vx != kk;
     * After `Cycle()`, we can just increment PC by 2 again to skip the next instruction.
     */
    void Chip8::OP_4xkk(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 byte = (opcode & 0x00FFu);

        if(registers[Vx] != byte){
            pc += 2;
        }
    }

    /*
     * OP 5xy0: Skip next instruction if Vx == Vy;
     * After `Cycle()`, we can just increment PC by 2 again to skip the next instruction.
     */
    void Chip8::OP_5xy0(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        if(registers[Vx] == registers[Vy]){
            pc += 2;
        }
    }   
    // OP 6xkk: LD Vx, byte
    void Chip8::OP_6xkk(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 byte = (opcode & 0x0FFu);

        // Load byte in Vx;
        registers[Vx] = byte;
    }

    // OP 7xkk: ADD Vx, byte
    void Chip8::OP_7xkk(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 byte = (opcode & 0x00FFu);

        // ADD the value of byte and Vx and place inside of the current position of Vx;
        registers[Vx] += byte;
    }

    // OP 8xy0: LD Vx, Vy
    void Chip8::OP_8xy0(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        // Load Vy in Vx;
        registers[Vx] = registers[Vy];
    }

    // OP 8xy1: OR Vx, Vy
    // Set Vx = Vx OR Vy.
    void Chip8::OP_8xy1(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] |= registers[Vy];
    }

    // OP 8xy2: AND Vx, Vy 
    // Set Vx = Vx AND Vy.
    void Chip8::OP_8xy2(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] &= registers[Vy];
    }

    // OP 8xy3: XOR Vx, Vy 
    // Set Vx = Vx XOR Vy.
    void Chip8::OP_8xy3(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] ^= registers[Vy];
    }

    // OP 8xy4: ADD Vx, Vy
    // Set Vx = Vx + Vy, set VF = carry.
    void Chip8::OP_8xy4(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        u16 sum = registers[Vx] + registers[Vy];

        if(sum > 255U){
            registers[0xF] = 1;
        }else {
            registers[0xF] = 0;
        }

        registers[Vx] = sum & 0xFFu;
    }

    // OP 8xy5: SUB Vx, Vy
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is SUB from Vx, and the result stored in Vx.
    void Chip8::OP_8xy5(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        if(registers[Vx] > registers[Vy]){
            registers[0xF] = 1;
        }else {
            registers[0xF] = 0;
        }
        registers[Vx] -= registers[Vy];
    }
    // OP 8xy6: SHR Vx
    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2;
    void Chip8::OP_8xy6(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
       
        registers[0xF] = (registers[Vx] & 0x1u);
        registers[Vx] >>= 1;
    }

    // OP 8xy7: SUBN Vx, Vy
    // Set Vx = Vy - Vx, set VF = NOT borrow;
    // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is SUB from Vy, and the results stored in Vx.
    void Chip8::OP_8xy7(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;

        if(registers[Vy] > registers[Vx]){
            registers[0xF] = 1;
        }else {
            registers[0xF] = 0;
        }

        registers[Vx] =  registers[Vy] - registers[Vx];
    }

    // OP 8xyE: SHL Vx {, Vy}
    // Set Vx = Vx SHL 1.
    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
    void Chip8::OP_8xyE(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;
        // Left shift is performed (multiplication by 2), and the MSB is saved in register VF. (0xF);
        registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

        registers[Vx] <<= 1;
    }

    // OP 9xy0:  SNE Vx, Vy
    // Skip next instruction if Vx != Vy;
    // PC incremented by 2 in Cycle(), then we just increment by 2 again to skip the next instruction.
    void Chip8::OP_9xy0(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;
        
        if(registers[Vx] != registers[Vy]){
            pc+=2;
        }
    }

    // -----------------------------------------------------------------
    // OP Annn: LD I, addr
    // Set I = nnn.
    void Chip8::OP_Annn(){
        u16 address = opcode & 0x0FFFu;

        index = address;
    }

    // OP Bnnn: JP V0, addr
    // JMP to location nnn + V0; 
    void Chip8::OP_Bnnn(){
        u16 address = opcode & 0x0FFFu;

        pc = registers[0] + address;
    }

    // OP Cxkk: RND Vx, byte 
    // Set Vx = random byte AND kk.
    void Chip8::OP_Cxkk(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 byte = opcode & 0x00FFu;

        // Could be done by the hardware or lib as we did below;
        registers[Vx] = randByte(randGen) & byte;
    }

    // OP Dxyn: DRW Vx, Vy Vx, nibble 
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    // TODO: REVISIT THIS
    void Chip8::OP_Dxyn(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 Vy = (opcode & 0x00F0u) >> 4u;
        u8 height = opcode & 0x000Fu;

        // Wrap if going beyond screen boundaries
        u8 xPos = registers[Vx] % VIDEO_WIDTH;
        u8 yPos = registers[Vy] % VIDEO_HEIGHT;

        registers[0xF] = 0;

        // Cache hit by doing row-first iteration.
        for(unsigned int row = 0; row < 8; ++row){
            u8 spriteByte = memory[index + row];

            for(unsigned int col = 0; col < 8; ++col){
                u8 spritePixel = spritePixel & (0x80u >> col);
                u32* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

                if(spritePixel){
                    if(*screenPixel == 0xFFFFFFFF){
                        registers[0xF] = 1;
                    }

                    *screenPixel ^= 0xFFFFFFFF;
                }
            }
        }

    }

    // OP Ex9E: SKP Vx 
    // Skip next instruction if key with the value of Vx is pressed.
    // The same as other incr instructions. 
    void Chip8::OP_Ex9E(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 key = registers[Vx];

        if(keypad[key]){
            pc += 2;
        }
    }

    // OP ExA1: SKNP Vx
    // Skip next instruction if key with the vale of Vx is not pressed.
    // The same as other incr instructions.
    void Chip8::OP_ExA1(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 key = registers[Vx];

        if(!keypad[key]){
            pc += 2;
        }
    }

    // OP Fx07: LD Vx, DT
    // Set Vx = delay timer value;
    void Chip8::OP_Fx07(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        registers[Vx] = delayTimer;
    }

    // Wait for a key press, store the value of the key in Vx.
    // "Easiest way to "wait" is to decrement the PC by 2 whenever a keypad value is not detected. This has the effect of running the same intruction repeatedly.
    void Chip8::OP_Fx0A(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        if (keypad[0])
        {
            registers[Vx] = 0;
        }
        else if (keypad[1])
        {
            registers[Vx] = 1;
        }
        else if (keypad[2])
        {
            registers[Vx] = 2;
        }
        else if (keypad[3])
        {
            registers[Vx] = 3;
        }
        else if (keypad[4])
        {
            registers[Vx] = 4;
        }
        else if (keypad[5])
        {
            registers[Vx] = 5;
        }
        else if (keypad[6])
        {
            registers[Vx] = 6;
        }
        else if (keypad[7])
        {
            registers[Vx] = 7;
        }
        else if (keypad[8])
        {
            registers[Vx] = 8;
        }
        else if (keypad[9])
        {
            registers[Vx] = 9;
        }
        else if (keypad[10])
        {
            registers[Vx] = 10;
        }
        else if (keypad[11])
        {
            registers[Vx] = 11;
        }
        else if (keypad[12])
        {
            registers[Vx] = 12;
        }
        else if (keypad[13])
        {
            registers[Vx] = 13;
        }
        else if (keypad[14])
        {
            registers[Vx] = 14;
        }
        else if (keypad[15])
        {
            registers[Vx] = 15;
        }
        else
        {
            pc -= 2;
        }
    }

    // OP Fx15: LD DT, Vx
    // Set delay timer = Vx;
    void Chip8::OP_Fx15(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        // DT = delayTimer
        delayTimer = registers[Vx];
    }

    // OP Fx18: LD ST, Vx
    // Set delay timer = Vx;
    void Chip8::OP_Fx18(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        soundTimer = registers[Vx];
    }

    // OP Fx1E: ADD I, Vx
    // Set I = I + Vx;
    void Chip8::OP_Fx1E(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        index += registers[Vx];
    }

    // OP Fx29: ADD I, Vx
    // Set I = I + Vx;
    void Chip8::OP_Fx29(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 digit = registers[Vx];

        index += FONTSET_START_ADDRESS + (5 * digit);
    }

    // OP Fx33: LD B, Vx
    // Store BCD representation of Vx in memory locations I, I+1, I+2.
    // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2;
    void Chip8::OP_Fx33(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;
        u8 value = registers[Vx];
        
        // Ones
        memory[index + 2] = value % 10;
        value /= 10;
        // Tens
        memory[index + 1] = value % 10;
        value /= 10;
        // Hundreds
        memory[index] = value % 10;
    }

    // OP Fx55: LD [I], Vx
    // Store registers V0 through Vx in memory starting at location I.
    void Chip8::OP_Fx55(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        for(u8 i = 0; i <= Vx; ++i){
            memory[i + 1] = registers[i];
        }
    }

    // OP Fx65: LD Vx, [I]
    // Read registers V0 through Vx from memory starting at location I.
    void Chip8::OP_Fx65(){
        u8 Vx = (opcode & 0x0F00u) >> 8u;

        for(u8 i = 0; i <= Vx; ++i){
            registers[i] = memory[i + 1];
        }
    }

}


