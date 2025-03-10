#include "Cpu.hpp"

CPU::CPU(Memory& memory): memory(memory) {
    Reset();
}

void CPU::Reset() {
    PC = 0xFFFC;
    SP = 0xFF;
    AC = X = Y = 0;
    PS = 0x00;
    cycles = 0;
}

void CPU::Reset(Word address) {
    PC = address;
    SP = 0xFF;
    AC = X = Y = 0;
    PS = 0x00;
    cycles = 0;
}

void CPU::PushByte(Byte value) {
    WriteByte(0x0100 + SP, value);
    SP--;
}

void CPU::PopByte(Byte& value) {
    value = ReadByte(0x0100 + SP + 1);
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

Byte CPU::PerformOp(LogicalOp op, Byte value, Byte reg) {
    switch (op) {
        case LogicalOp::AND:
            return reg & value;
        case LogicalOp::ORA:
            return reg | value;
        case LogicalOp::EOR:
            return reg ^ value;
        default:
            throw -1;
    }
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
            case TSX: { INS_TSX(*this); } break;
            case TXS: { INS_TXS(*this); } break;
            case PHA: { INS_PHA(*this); } break;
            case PHP: { INS_PHP(*this); } break;
            case PLA: { INS_PLA(*this); } break;
            case PLP: { INS_PLP(*this); } break;
            case AND_IM: { LOG_IM(*this, LogicalOp::AND); } break;
            case ORA_IM: { LOG_IM(*this, LogicalOp::ORA); } break;
            case EOR_IM: { LOG_IM(*this, LogicalOp::EOR); } break;
            case AND_ZP: { LOG_ZP(*this, LogicalOp::AND); } break;
            case ORA_ZP: { LOG_ZP(*this, LogicalOp::ORA); } break;
            case EOR_ZP: { LOG_ZP(*this, LogicalOp::EOR); } break;
            case AND_ZPX: { LOG_ZPX(*this, LogicalOp::AND); } break;
            case ORA_ZPX: { LOG_ZPX(*this, LogicalOp::ORA); } break;
            case EOR_ZPX: { LOG_ZPX(*this, LogicalOp::EOR); } break;
            case AND_ABS: { LOG_ABS(*this, LogicalOp::AND); } break;
            case ORA_ABS: { LOG_ABS(*this, LogicalOp::ORA); } break;
            case EOR_ABS: { LOG_ABS(*this, LogicalOp::EOR); } break;
            case AND_ABSX: { LOG_ABSX(*this, LogicalOp::AND); } break;
            case ORA_ABSX: { LOG_ABSX(*this, LogicalOp::ORA); } break;
            case EOR_ABSX: { LOG_ABSX(*this, LogicalOp::EOR); } break;
            case AND_ABSY: { LOG_ABSY(*this, LogicalOp::AND); } break;
            case ORA_ABSY: { LOG_ABSY(*this, LogicalOp::ORA); } break;
            case EOR_ABSY: { LOG_ABSY(*this, LogicalOp::EOR); } break;
            case AND_INDX: { LOG_INDX(*this, LogicalOp::AND); } break;
            case ORA_INDX: { LOG_INDX(*this, LogicalOp::ORA); } break;
            case EOR_INDX: { LOG_INDX(*this, LogicalOp::EOR); } break;
            case AND_INDY: { LOG_INDY(*this, LogicalOp::AND); } break;
            case ORA_INDY: { LOG_INDY(*this, LogicalOp::ORA); } break;
            case EOR_INDY: { LOG_INDY(*this, LogicalOp::EOR); } break;
            case BIT_ZP: { INS_BIT_ZP(*this); } break;
            case BIT_ABS: { INS_BIT_ABS(*this); } break;
            case TAX: { INS_TAX(*this); } break;
            case TAY: { INS_TAY(*this); } break;
            case TXA: { INS_TXA(*this); } break;
            case TYA: { INS_TYA(*this); } break;
            case INX: { INS_INX(*this); } break;
            case INY: { INS_INY(*this); } break;
            case DEX: { INS_DEX(*this); } break;
            case DEY: { INS_DEY(*this); } break;
            case DEC_ZP: { INS_DEC_ZP(*this); } break;
            case DEC_ZPX: { INS_DEC_ZPX(*this); } break;
            case DEC_ABS: { INS_DEC_ABS(*this); } break;
            case DEC_ABSX: { INS_DEC_ABSX(*this); } break;
            case INC_ZP: { INS_INC_ZP(*this); } break;
            case INC_ZPX: { INS_INC_ZPX(*this); } break;
            case INC_ABS: { INS_INC_ABS(*this); } break;
            case INC_ABSX: { INS_INC_ABSX(*this); } break;

            default: {
                printf("Instruction not implemented: %02X\n", instruction);
                throw -1;
            } break;
        }
    }

    int32_t cyclesUsed = cycles - this->cycles;
    return cyclesUsed;
}