#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char; // 8 bits
using Word = unsigned short; // 16 bits

struct Memory {
    static constexpr unsigned int MEMORY_SIZE = 1024 * 64; // 64KB
    Byte Data[MEMORY_SIZE];

    void Initialize() {
        for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
            Data[i] = 0;
        }
    }

    Byte operator[](unsigned int address) const {
        return Data[address];
    }

    Byte& operator[](unsigned int address) {
        return Data[address];
    }
};

struct CPU {
    Word PC; // Program Counter
    Word SP; // Stack Pointer

    Byte AC, X, Y; // Registers

    // Status Flags
    Byte C : 1; // Carry Flag
    Byte Z : 1; // Zero Flag
    Byte I : 1; // Interrupt Disable
    Byte D : 1; // Decimal Mode
    Byte B : 1; // Break Command
    Byte V : 1; // Overflow Flag
    Byte N : 1; // Negative Flag

    // Opcodes
    static constexpr Byte LDA_IM = 0xA9; // LDA Immediate
    static constexpr Byte LDA_ZP = 0xA5; // LDA Zero Page

    void Initialize(Memory& memory) {
        PC = 0xFFFC; // Reset Vector Address (i.e. where the program starts)
        SP = 0x0100; // Stack Pointer starts at 0x0100
        AC = X = Y = 0; // Clear all registers
        C = Z = I = D = B = V = N = 0; // Clear all flags

        memory.Initialize();
    }

    Byte FetchByte(Memory& memory, unsigned int& cycles) {
        Byte data = memory[PC];
        PC++;
        cycles--;
        return data;
    }

    Byte ReadByte(Byte& adress, Memory& memory, unsigned int& cycles) {
        Byte data = memory[adress];
        cycles--;
        return data;
    }

    void Execute(unsigned int cycles, Memory& memory) {
        while (cycles > 0) {
            Byte instruction = FetchByte(memory, cycles);

            switch(instruction) {
                case LDA_IM: {
                    Byte value = FetchByte(memory, cycles);
                    AC = value;
                    Z = (AC == 0);
                    N = (AC & 0b10000000) > 0;
                } break;

                case LDA_ZP: {
                    Byte zeroPageAddress = FetchByte(memory, cycles);
                    AC = ReadByte(zeroPageAddress, memory, cycles);
                    Z = (AC == 0);
                    N = (AC & 0b10000000) > 0;
                } break;

                default: {
                    printf("Instruction not allowed %d", instruction);
                } break;
            }
        }
    }
};

int main() {
    Memory memory;
    CPU cpu;

    cpu.Initialize(memory);

    memory[0xFFFC] = CPU::LDA_IM;
    memory[0xFFFD] = 0x42;

    cpu.Execute(2, memory);
    return 0;
}