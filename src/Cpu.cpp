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
    cycles--;
    return memory[PC++];
}

Word CPU::FetchWord() {
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

Word CPU::ReadWordWithWrap(Byte address) {
    cycles -= 2;
    
    union {
        uint32_t i;
        uint8_t b[4];
    } littleEndianTest = {0x01020304};

    bool isLittleEndian = littleEndianTest.b[0] == 0x04;

    Word word;

    if (isLittleEndian) {
        word = memory[address];
        word |= memory[address + 1] << 8;
    } else {
        word = memory[address] << 8;
        word |= memory[address + 1];
    }

    return word;
}

Byte CPU::ReadByte(Word address) {
    cycles--;
    return memory[address];
}

Byte CPU::ReadByteWithWrap(Byte address) {
    cycles--;
    return memory[address];
}

int32_t CPU::Execute(int32_t cycles) {
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
                AC = ReadByteWithWrap(zeroPageAddress);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_ZPX: {
                Byte zeroPageAddress = FetchNext();
                AC = ReadByteWithWrap(zeroPageAddress + X);
                this->cycles--;
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_ABS: {
                Word address = FetchWord();
                AC = ReadByte(address);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_ABSX: {
                Word address = FetchWord();
                AC = ReadByte(address + X);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
                if ((address & 0xFF) + X > 0xFF) // If it crosses a page boundary
                    this->cycles--;
            } break;

            case LDA_ABSY: {
                Word address = FetchWord();
                AC = ReadByte(address + Y);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
                if ((address & 0xFF) + Y > 0xFF) // If it crosses a page boundary
                    this->cycles--;
            } break;

            case LDA_INDX: {
                Byte zeroPageAddress = FetchNext();
                zeroPageAddress += X;
                this->cycles--;
                Word address = ReadWordWithWrap(zeroPageAddress);
                AC = ReadByte(address);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
            } break;

            case LDA_INDY: {
                Byte zeroPageAddress = FetchNext();
                Word effectiveAddress = ReadWordWithWrap(zeroPageAddress);
                AC = ReadByte(effectiveAddress + Y);
                Z = (AC == 0);
                N = (AC & 0b10000000) > 0;
                if ((effectiveAddress & 0xFF) + Y > 0xFF) // If it crosses a page boundary
                    this->cycles--;
            } break;

            case JSR: {
                Word address = FetchWord();
                WriteWord(SP, PC - 1);
                SP += 2; // SP is a Byte
                PC = address;
                this->cycles--;
            } break;

            default: {
                throw -1;
                printf("Instruction not implemented: %02X\n", instruction);
            } break;
        }
    }

    int32_t cyclesUsed = cycles - this->cycles;
    return cyclesUsed;
}