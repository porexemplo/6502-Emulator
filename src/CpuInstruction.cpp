#include "CpuInstruction.hpp"
#include "Cpu.hpp"

void LDSetFlags(CPU& cpu, Byte value) {
    cpu.P.Z = (value == 0);
    cpu.P.N = (value & 0b10000000) > 0;
}

void LD_IM(CPU& cpu, Byte& reg) {
    reg = cpu.FetchNext();
    LDSetFlags(cpu, reg);
}

void LD_ZP(CPU& cpu, Byte& reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    reg = cpu.ReadByteWithWrap(zeroPageAddress);
    LDSetFlags(cpu, reg);
}

void LD_ZPX(CPU& cpu, Byte& reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    reg = cpu.ReadByteWithWrap(zeroPageAddress + cpu.X);
    cpu.cycles--;
    LDSetFlags(cpu, reg);
}

void LD_ZPY(CPU& cpu, Byte& reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    reg = cpu.ReadByteWithWrap(zeroPageAddress + cpu.Y);
    cpu.cycles--;
    LDSetFlags(cpu, reg);
}

void LD_ABS(CPU& cpu, Byte& reg) {
    Word address = cpu.FetchWord();
    reg = cpu.ReadByte(address);
    LDSetFlags(cpu, reg);
}

void LD_ABSX(CPU& cpu, Byte& reg) {
    Word address = cpu.FetchWord();
    reg = cpu.ReadByte(address + cpu.X);
    LDSetFlags(cpu, reg);
    if ((address & 0xFF) + cpu.X > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}

void LD_ABSY(CPU& cpu, Byte& reg) {
    Word address = cpu.FetchWord();
    reg = cpu.ReadByte(address + cpu.Y);
    LDSetFlags(cpu, reg);
    if ((address & 0xFF) + cpu.Y > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}

void LD_INDX(CPU& cpu) {
    Byte zeroPageAddress = cpu.FetchNext();
    zeroPageAddress += cpu.X;
    cpu.cycles--;
    Word address = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.AC = cpu.ReadByte(address);
    LDSetFlags(cpu, cpu.AC);
}

void LD_INDY(CPU& cpu) {
    Byte zeroPageAddress = cpu.FetchNext();
    Word effectiveAddress = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.AC = cpu.ReadByte(effectiveAddress + cpu.Y);
    LDSetFlags(cpu, cpu.AC);
    if ((effectiveAddress & 0xFF) + cpu.Y > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}

void ST_ZP(CPU& cpu, Byte reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    cpu.WriteByte(zeroPageAddress, reg);
}

void ST_ZPX(CPU& cpu, Byte reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    cpu.cycles--;
    cpu.WriteByte(zeroPageAddress + cpu.X, reg);
}

void ST_ZPY(CPU& cpu, Byte reg) {
    Byte zeroPageAddress = cpu.FetchNext();
    cpu.cycles--;
    cpu.WriteByte(zeroPageAddress + cpu.Y, reg);
}

void ST_ABS(CPU& cpu, Byte reg) {
    Word address = cpu.FetchWord();
    cpu.WriteByte(address, reg);
}

void ST_ABSX(CPU& cpu) {
    Word address = cpu.FetchWord();
    cpu.cycles--;
    cpu.WriteByte(address + cpu.X, cpu.AC);
}

void ST_ABSY(CPU& cpu) {
    Word address = cpu.FetchWord();
    cpu.cycles--;
    cpu.WriteByte(address + cpu.Y, cpu.AC);
}

void ST_INDX(CPU& cpu) {
    Byte zeroPageAddress = cpu.FetchNext();
    zeroPageAddress += cpu.X;
    cpu.cycles--;
    Word address = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.WriteByte(address, cpu.AC);
}

void ST_INDY(CPU& cpu) {
    Byte zeroPageAddress = cpu.FetchNext();
    Word effectiveAddress = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.cycles--;
    cpu.WriteByte(effectiveAddress + cpu.Y, cpu.AC);
}

void INS_JSR(CPU& cpu) {
    Word address = cpu.FetchWord();
    cpu.PushPC(); // Takes 2 cycles (Writes 2 bytes)
    cpu.PC = address;
    cpu.cycles--; // For setting PC
}

void INS_RTS(CPU& cpu) {
    cpu.PopPC(); // Takes 4 cycles (Reads 2 bytes, Increments SP by 2)
    cpu.PC++; // Increment PC to point to the next instruction
    cpu.cycles--; // For the increment
}

void INS_JMP_ABS(CPU& cpu) {
    cpu.PC = cpu.FetchWord();
}

void INS_JMP_IND(CPU& cpu) {
    Word address = cpu.FetchWord();
    cpu.PC = cpu.ReadWord(address);
}

void INS_TSX(CPU& cpu) {
    cpu.X = cpu.SP;
    LDSetFlags(cpu, cpu.X);
    cpu.cycles--;
}

void INS_TXS(CPU& cpu) {
    cpu.SP = cpu.X;
    LDSetFlags(cpu, cpu.SP);
    cpu.cycles--;
}

void INS_PHA(CPU& cpu) {
    cpu.PushByte(cpu.AC);
    cpu.cycles--;
}

void INS_PHP(CPU& cpu) {
    cpu.PushByte(cpu.PS);
    cpu.cycles--;
}

void INS_PLA(CPU& cpu) {
    cpu.PopByte(cpu.AC);
    LDSetFlags(cpu, cpu.AC);
    cpu.cycles -= 2;
}

void INS_PLP(CPU& cpu) {
    cpu.PopByte(cpu.PS);
    cpu.cycles -= 2;
}

void LOG_IM(CPU& cpu, LogicalOp op) {
    cpu.AC = CPU::PerformOp(op, cpu.FetchNext(), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
}

void LOG_ZP(CPU& cpu, LogicalOp op) {
    Byte zeroPageAddress = cpu.FetchNext();
    cpu.AC = CPU::PerformOp(op, cpu.ReadByteWithWrap(zeroPageAddress), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
}

void LOG_ZPX(CPU& cpu, LogicalOp op) {
    Byte zeroPageAddress = cpu.FetchNext();
    cpu.AC = CPU::PerformOp(op, cpu.ReadByteWithWrap(zeroPageAddress + cpu.X), cpu.AC);
    cpu.cycles--;
    LDSetFlags(cpu, cpu.AC);
}

void LOG_ABS(CPU& cpu, LogicalOp op) {
    Word address = cpu.FetchWord();
    cpu.AC = CPU::PerformOp(op, cpu.ReadByte(address), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
}

void LOG_ABSX(CPU& cpu, LogicalOp op) {
    Word address = cpu.FetchWord();
    cpu.AC = CPU::PerformOp(op, cpu.ReadByte(address + cpu.X), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
    if ((address & 0xFF) + cpu.X > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}

void LOG_ABSY(CPU& cpu, LogicalOp op) {
    Word address = cpu.FetchWord();
    cpu.AC = CPU::PerformOp(op, cpu.ReadByte(address + cpu.Y), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
    if ((address & 0xFF) + cpu.Y > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}

void LOG_INDX(CPU& cpu, LogicalOp op) {
    Byte zeroPageAddress = cpu.FetchNext();
    zeroPageAddress += cpu.X;
    cpu.cycles--;
    Word address = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.AC = CPU::PerformOp(op, cpu.ReadByte(address), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
}

void LOG_INDY(CPU& cpu, LogicalOp op) {
    Byte zeroPageAddress = cpu.FetchNext();
    Word effectiveAddress = cpu.ReadWordWithWrap(zeroPageAddress);
    cpu.AC = CPU::PerformOp(op, cpu.ReadByte(effectiveAddress + cpu.Y), cpu.AC);
    LDSetFlags(cpu, cpu.AC);
    if ((effectiveAddress & 0xFF) + cpu.Y > 0xFF) // If it crosses a page boundary
        cpu.cycles--;
}