#include "Cpu.hpp"

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
            case LDA_IM: { LD_IM(*this, AC); } break;

            case LDX_IM: { LD_IM(*this, X); } break;

            case LDY_IM: { LD_IM(*this, Y); } break;

            case LDA_ZP: { LD_ZP(*this, AC); } break;

            case LDX_ZP: { LD_ZP(*this, X); } break;

            case LDY_ZP: { LD_ZP(*this, Y); } break;

            case LDA_ZPX: { LD_ZPX(*this, AC); } break;

            case LDX_ZPY: { LD_ZPY(*this, X); } break;
            
            case LDY_ZPX: { LD_ZPX(*this, Y); } break;

            case LDA_ABS: { LD_ABS(*this, AC); } break;

            case LDX_ABS: { LD_ABS(*this, X); } break;

            case LDY_ABS: { LD_ABS(*this, Y); } break;

            case LDA_ABSX: { LD_ABSX(*this, AC); } break;

            case LDY_ABSX: { LD_ABSX(*this, Y); } break;

            case LDA_ABSY: { LD_ABSY(*this, AC); } break;

            case LDX_ABSY: { LD_ABSY(*this, X); } break;

            case LDA_INDX: { LD_INDX(*this); } break;

            case LDA_INDY: { LD_INDY(*this); } break;

            case JSR: { INS_JSR(*this); } break;

            default: {
                throw -1;
                printf("Instruction not implemented: %02X\n", instruction);
            } break;
        }
    }

    int32_t cyclesUsed = cycles - this->cycles;
    return cyclesUsed;
}