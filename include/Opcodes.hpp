#ifndef INSTUCTION_HPP
#define INSTUCTION_HPP

#include "Types.hpp"

// Enum for logical operations
enum class LogicalOp {
    AND,
    ORA,
    EOR
};

// LDA - Load Accumulator
constexpr Byte LDA_IM   = 0xA9; // Load Accumulator Immediate, 2 Cycles
constexpr Byte LDA_ZP   = 0xA5; // Load Accumulator Zero Page, 3 Cycles
constexpr Byte LDA_ZPX  = 0xB5; // Load Accumulator Zero Page X, 4 Cycles
constexpr Byte LDA_ABS  = 0xAD; // Load Accumulator Absolute, 4 Cycles
constexpr Byte LDA_ABSX = 0xBD; // Load Accumulator Absolute X, 4+ Cycles
constexpr Byte LDA_ABSY = 0xB9; // Load Accumulator Absolute Y, 4+ Cycles
constexpr Byte LDA_INDX = 0xA1; // Load Accumulator Indirect X, 6 Cycles
constexpr Byte LDA_INDY = 0xB1; // Load Accumulator Indirect Y, 5+ Cycles

// LDX - Load X Register
constexpr Byte LDX_IM   = 0xA2; // Load X Register Immediate, 2 Cycles
constexpr Byte LDX_ZP   = 0xA6; // Load X Register Zero Page, 3 Cycles
constexpr Byte LDX_ZPY  = 0xB6; // Load X Register Zero Page Y, 4 Cycles
constexpr Byte LDX_ABS  = 0xAE; // Load X Register Absolute, 4 Cycles
constexpr Byte LDX_ABSY = 0xBE; // Load X Register Absolute Y, 4+ Cycles

// LDY - Load Y Register
constexpr Byte LDY_IM   = 0xA0; // Load Y Register Immediate, 2 Cycles
constexpr Byte LDY_ZP   = 0xA4; // Load Y Register Zero Page, 3 Cycles
constexpr Byte LDY_ZPX  = 0xB4; // Load Y Register Zero Page X, 4 Cycles
constexpr Byte LDY_ABS  = 0xAC; // Load Y Register Absolute, 4 Cycles
constexpr Byte LDY_ABSX = 0xBC; // Load Y Register Absolute X, 4+ Cycles

// STA - Store Accumulator
constexpr Byte STA_ZP   = 0x85; // Store Accumulator Zero Page, 3 Cycles
constexpr Byte STA_ZPX  = 0x95; // Store Accumulator Zero Page X, 4 Cycles
constexpr Byte STA_ABS  = 0x8D; // Store Accumulator Absolute, 4 Cycles
constexpr Byte STA_ABSX = 0x9D; // Store Accumulator Absolute X, 5 Cycles
constexpr Byte STA_ABSY = 0x99; // Store Accumulator Absolute Y, 5 Cycles
constexpr Byte STA_INDX = 0x81; // Store Accumulator Indirect X, 6 Cycles
constexpr Byte STA_INDY = 0x91; // Store Accumulator Indirect Y, 6 Cycles

// STX - Store X Register
constexpr Byte STX_ZP   = 0x86; // Store X Register Zero Page, 3 Cycles
constexpr Byte STX_ZPY  = 0x96; // Store X Register Zero Page Y, 4 Cycles
constexpr Byte STX_ABS  = 0x8E; // Store X Register Absolute, 4 Cycles

// STY - Store Y Register
constexpr Byte STY_ZP   = 0x84; // Store Y Register Zero Page, 3 Cycles
constexpr Byte STY_ZPX  = 0x94; // Store Y Register Zero Page X, 4 Cycles
constexpr Byte STY_ABS  = 0x8C; // Store Y Register Absolute, 4 Cycles

// Jump Instructions
constexpr Byte JSR  = 0x20; // Jump to Subroutine Absolute, 6 Cycles
constexpr Byte RTS  = 0x60; // Return from Subroutine, 6 Cycles
constexpr Byte JMP_ABS = 0x4C; // Jump to Absolute, 3 Cycles
constexpr Byte JMP_IND = 0x6C; // Jump to Indirect, 5 Cycles

// Stack Instructions
constexpr Byte TSX = 0xBA; // Transfer Stack Pointer to X, 2 Cycles
constexpr Byte TXS = 0x9A; // Transfer X to Stack Pointer, 2 Cycles
constexpr Byte PHA = 0x48; // Push Accumulator, 3 Cycles
constexpr Byte PHP = 0x08; // Push Processor Status, 3 Cycles
constexpr Byte PLA = 0x68; // Pull Accumulator, 4 Cycles (Sets Z and N)
constexpr Byte PLP = 0x28; // Pull Processor Status, 4 Cycles (Sets all flags)

// Logical Instructions
constexpr Byte AND_IM = 0x29; // AND Immediate, 2 Cycles
constexpr Byte AND_ZP = 0x25; // AND Zero Page, 3 Cycles
constexpr Byte AND_ZPX = 0x35; // AND Zero Page X, 4 Cycles
constexpr Byte AND_ABS = 0x2D; // AND Absolute, 4 Cycles
constexpr Byte AND_ABSX = 0x3D; // AND Absolute X, 4+ Cycles
constexpr Byte AND_ABSY = 0x39; // AND Absolute Y, 4+ Cycles
constexpr Byte AND_INDX = 0x21; // AND Indirect X, 6 Cycles
constexpr Byte AND_INDY = 0x31; // AND Indirect Y, 5+ Cycles

constexpr Byte ORA_IM = 0x09; // ORA Immediate, 2 Cycles
constexpr Byte ORA_ZP = 0x05; // ORA Zero Page, 3 Cycles
constexpr Byte ORA_ZPX = 0x15; // ORA Zero Page X, 4 Cycles
constexpr Byte ORA_ABS = 0x0D; // ORA Absolute, 4 Cycles
constexpr Byte ORA_ABSX = 0x1D; // ORA Absolute X, 4+ Cycles
constexpr Byte ORA_ABSY = 0x19; // ORA Absolute Y, 4+ Cycles
constexpr Byte ORA_INDX = 0x01; // ORA Indirect X, 6 Cycles
constexpr Byte ORA_INDY = 0x11; // ORA Indirect Y, 5+ Cycles

constexpr Byte EOR_IM = 0x49; // EOR Immediate, 2 Cycles
constexpr Byte EOR_ZP = 0x45; // EOR Zero Page, 3 Cycles
constexpr Byte EOR_ZPX = 0x55; // EOR Zero Page X, 4 Cycles
constexpr Byte EOR_ABS = 0x4D; // EOR Absolute, 4 Cycles
constexpr Byte EOR_ABSX = 0x5D; // EOR Absolute X, 4+ Cycles
constexpr Byte EOR_ABSY = 0x59; // EOR Absolute Y, 4+ Cycles
constexpr Byte EOR_INDX = 0x41; // EOR Indirect X, 6 Cycles
constexpr Byte EOR_INDY = 0x51; // EOR Indirect Y, 5+ Cycles

#endif