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
            }

            default: {
                std::cerr << "Instruction not allowed " << static_cast<int>(instruction) << std::endl;
                exit(1);
            } break;
        }
    }
}