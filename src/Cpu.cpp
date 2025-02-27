#include "Cpu.hpp"
#include <iostream>

CPU::CPU(Memory& memory): memory(memory) {
    Reset();
}

void CPU::Reset() {
    PC = 0xFFFC;
    SP = 0x0100;
    AC = X = Y = 0;
    C = Z = I = D = B = V = N = 0;
    cycles = 0;
}

uint8_t CPU::FetchNext() {
    if (cycles <= 0) {
        std::cerr << "No cycles left to fetch byte" << std::endl;
        exit(1);
    }
    cycles--;
    return memory[PC++];
}

Word CPU::FetchWord() {
    if (cycles <= 0) {
        std::cerr << "No cycles left to fetch word" << std::endl;
        exit(1);
    }
    cycles -= 2;

    union {
        uint32_t i;
        uint8_t b[4];
    } littleEndianTest = {0x01020304};

    bool isLittleEndian = littleEndianTest.b[0] == 0x04;

    Word word;

    if (isLittleEndian) {
        word = memory[PC++];
        word |= memory[PC++] << 8;
    } else {
        word = memory[PC++] << 8;
        word |= memory[PC++];
    }
    
    return word;
}

void CPU::WriteWord(Word address, Word value) {
    if (cycles <= 0) {
        std::cerr << "No cycles left to write word" << std::endl;
        exit(1);
    }
    cycles -= 2;

    union {
        uint32_t i;
        uint8_t b[4];
    } littleEndianTest = {0x01020304};

    bool isLittleEndian = littleEndianTest.b[0] == 0x04;

    if (isLittleEndian) {
        memory[address] = value & 0xFF; // The & 0xFF is to get the lower 8 bits
        memory[address + 1] = value >> 8; // The >> 8 is to get the higher 8 bits
    } else {
        memory[address] = value >> 8;
        memory[address + 1] = value & 0xFF;
    }
}

uint8_t CPU::ReadByte(uint8_t address) {
    if (cycles <= 0) {
        std::cerr << "No cycles left to read byte" << std::endl;
        exit(1);
    }
    cycles--;
    return memory[address];
}

void CPU::Execute(uint32_t cycles) {
    this->cycles = cycles;
    while (this->cycles > 0) {
        Byte instruction = FetchNext();

        switch(instruction) {
            case LDA_IM: {
                Byte value = FetchNext();
                AC = value;
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_ZP: {
                Byte zeroPageAddress = FetchNext();
                AC = ReadByte(zeroPageAddress);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_ZPX: {
                Byte zeroPageAddress = FetchNext();
                AC = ReadByte(zeroPageAddress + X);
                this->cycles--;
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case JSR: {
                Word address = FetchWord();
                WriteWord(SP, PC - 1);
                SP += 2; // SP is a Byte
                PC = address;
                this->cycles--;
            } break;

            default: {
                std::cerr << "Instruction not allowed " << static_cast<int>(instruction) << std::endl;
                exit(1);
            } break;
        }
    }
}