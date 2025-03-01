#ifndef CPU_HPP
#define CPU_HPP

#include "Types.hpp"
#include "Memory.hpp"
#include "Opcodes.hpp"

class CPU {
public:
    Word PC; // Program Counter
    Word SP; // Stack Pointer

    Byte AC, X, Y;  // Registers
    
    // Status Flags
    Byte C : 1; // Carry Flag
    Byte Z : 1; // Zero Flag
    Byte I : 1; // Interrupt Disable
    Byte D : 1; // Decimal Mode
    Byte B : 1; // Break Command
    Byte V : 1; // Overflow Flag
    Byte N : 1; // Negative Flag

    Memory& memory;
    int32_t cycles;
    
    CPU(Memory& mem);

    void Reset();
    Byte FetchNext();
    Word FetchWord();
    Byte ReadByte(Word Address);
    Byte ReadByteWithWrap(Byte Address); // For Zero Page Addressing
    void WriteWord(Word address, Word value);
    int32_t Execute(int32_t cycles);
};
#endif