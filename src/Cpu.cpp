#include "Cpu.hpp"

CPU::CPU(Memory& memory): memory(memory) {
    Reset();
}

void CPU::Reset() {
    PC = 0xFFFC;
    SP = 0xFF;
    AC = X = Y = 0;
    // This will give the warning missing initializer for member 'CPU::P::U'
    // To solve it, we can initialize it like this:
    P = {0, 0, 0, 0, 0, 0, 0, 0};
    cycles = 0;
}

void CPU::Reset(Word address) {
    PC = address;
    SP = 0xFF;
    AC = X = Y = 0;
    P = {0, 0, 0, 0, 0, 0, 0, 0};
    cycles = 0;
}

void CPU::PushByte(Byte value) {
    WriteByte(0x0100 + SP, value);
    SP--;
}

void CPU::PopByte(Byte& value) {
    value = ReadByte(0x0100 + SP);
    SP++;
}

void CPU::PushWord(Word value) {
    WriteWord(0x0100 + SP - 1, value);
    SP -= 2;
}

void CPU::PopWord(Word& value) {
    value = ReadWord(0x0100 + SP + 1);
    SP += 2;
    cycles -= 2; // For incrementing SP
}

void CPU::PushPC() {
    PushWord(PC - 1);
}

void CPU::PopPC() {
    PopWord(PC);
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

void CPU::WriteByte(Word address, Byte value) {
    cycles--;
    memory[address] = value;
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

Word CPU::ReadWord(Word address) {
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

            case STA_ZP: { ST_ZP(*this, AC); } break;

            case STX_ZP: { ST_ZP(*this, X); } break;

            case STY_ZP: { ST_ZP(*this, Y); } break;

            case STA_ZPX: { ST_ZPX(*this, AC); } break;

            case STX_ZPY: { ST_ZPY(*this, X); } break;

            case STY_ZPX: { ST_ZPX(*this, Y); } break;

            case STA_ABS: { ST_ABS(*this, AC); } break;

            case STX_ABS: { ST_ABS(*this, X); } break;

            case STY_ABS: { ST_ABS(*this, Y); } break;

            case STA_ABSX: { ST_ABSX(*this); } break;

            case STA_ABSY: { ST_ABSY(*this); } break;

            case STA_INDX: { ST_INDX(*this); } break;

            case STA_INDY: { ST_INDY(*this); } break;

            case JSR: { INS_JSR(*this); } break;

            case RTS: { INS_RTS(*this); } break;

            case JMP_ABS: { INS_JMP_ABS(*this); } break;

            case JMP_IND: { INS_JMP_IND(*this); } break;

            default: {
                printf("Instruction not implemented: %02X\n", instruction);
                throw -1;
            } break;
        }
    }

    int32_t cyclesUsed = cycles - this->cycles;
    return cyclesUsed;
}