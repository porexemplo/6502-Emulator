#ifndef INSTUCTION_HPP
#define INSTUCTION_HPP

#include "Types.hpp"

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

// LDY - Load Y Register
constexpr Byte LDY_IM   = 0xA0; // Load Y Register Immediate, 2 Cycles
constexpr Byte LDY_ZP   = 0xA4; // Load Y Register Zero Page, 3 Cycles
constexpr Byte LDY_ZPX  = 0xB4; // Load Y Register Zero Page X, 4 Cycles
constexpr Byte LDY_ABS  = 0xAC; // Load Y Register Absolute, 4 Cycles

// JSR - Jump to Subroutine
constexpr Byte JSR  = 0x20; // Jump to Subroutine Absolute, 6 Cycles

// RTS - Return from Subroutine
constexpr Byte RTS  = 0x60; // Return from Subroutine, 6 Cycles

#endif