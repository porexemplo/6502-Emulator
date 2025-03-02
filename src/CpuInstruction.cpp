#include "CpuInstruction.hpp"
#include "Cpu.hpp"

void LDSetFlags(CPU& cpu, Byte value) {
    cpu.Z = (value == 0);
    cpu.N = (value & 0b10000000) > 0;
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

void INS_JSR(CPU& cpu) {
    Word address = cpu.FetchWord();
    cpu.WriteWord(cpu.SP, cpu.PC - 1);
    cpu.SP += 2; // SP is a Byte
    cpu.PC = address;
    cpu.cycles--;
}