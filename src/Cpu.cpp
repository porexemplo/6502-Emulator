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

void CPU::LDSetFlags(Byte value) {
    Z = (value == 0);
    N = (value & 0b10000000) > 0;
}

void CPU::LD_ZP(Byte& reg) {
    Byte zeroPageAddress = FetchNext();
    reg = ReadByteWithWrap(zeroPageAddress);
    LDSetFlags(reg);
}

void CPU::LD_ZPX(Byte& reg) {
    Byte zeroPageAddress = FetchNext();
    reg = ReadByteWithWrap(zeroPageAddress + X);
    this->cycles--;
    LDSetFlags(reg);
}

void CPU::LD_ZPY(Byte& reg) {
    Byte zeroPageAddress = FetchNext();
    reg = ReadByteWithWrap(zeroPageAddress + Y);
    this->cycles--;
    LDSetFlags(reg);
}

void CPU::LD_ABS(Byte& reg) {
    Word address = FetchWord();
    reg = ReadByte(address);
    LDSetFlags(reg);
}

void CPU::LD_ABSX(Byte& reg) {
    Word address = FetchWord();
    reg = ReadByte(address + X);
    LDSetFlags(reg);
    if ((address & 0xFF) + X > 0xFF) // If it crosses a page boundary
        this->cycles--;
}

void CPU::LD_ABSY(Byte& reg) {
    Word address = FetchWord();
    reg = ReadByte(address + Y);
    LDSetFlags(reg);
    if ((address & 0xFF) + Y > 0xFF) // If it crosses a page boundary
        this->cycles--;
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

void CPU::LD_IM(Byte& reg) {
    reg = FetchNext();
    LDSetFlags(reg);
}

int32_t CPU::Execute(int32_t cycles) {
    this->cycles = cycles;
    while (this->cycles > 0) {
        Byte instruction = FetchNext();

        switch(instruction) {
            case LDA_IM: { LD_IM(AC); } break;

            case LDX_IM: { LD_IM(X); } break;

            case LDY_IM: { LD_IM(Y); } break;

            case LDA_ZP: { LD_ZP(AC); } break;

            case LDX_ZP: { LD_ZP(X); } break;

            case LDY_ZP: { LD_ZP(Y); } break;

            case LDA_ZPX: { LD_ZPX(AC); } break;

            case LDX_ZPY: { LD_ZPY(X); } break;
            
            case LDY_ZPX: { LD_ZPX(Y); } break;

            case LDA_ABS: { LD_ABS(AC); } break;

            case LDX_ABS: { LD_ABS(X); } break;

            case LDY_ABS: { LD_ABS(Y); } break;

            case LDA_ABSX: { LD_ABSX(AC); } break;

            case LDY_ABSX: { LD_ABSX(Y); } break;

            case LDA_ABSY: { LD_ABSY(AC); } break;

            case LDX_ABSY: { LD_ABSY(X); } break;

            case LDA_INDX: {
                Byte zeroPageAddress = FetchNext();
                zeroPageAddress += X;
                this->cycles--;
                Word address = ReadWordWithWrap(zeroPageAddress);
                AC = ReadByte(address);
                LDSetFlags(AC);
            } break;

            case LDA_INDY: {
                Byte zeroPageAddress = FetchNext();
                Word effectiveAddress = ReadWordWithWrap(zeroPageAddress);
                AC = ReadByte(effectiveAddress + Y);
                LDSetFlags(AC);
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