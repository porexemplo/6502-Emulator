#ifndef INSTUCTION_HPP
#define INSTUCTION_HPP

#include "Types.hpp"

constexpr Byte LDA_IM   = 0xA9; // Load Accumulator Immediate, 2 Cycles
constexpr Byte LDA_ZP   = 0xA5; // Load Accumulator Zero Page, 3 Cycles
constexpr Byte LDA_ZPX  = 0xB5; // Load Accumulator Zero Page X, 4 Cycles
constexpr Byte LDA_ABS  = 0xAD; // Load Accumulator Absolute, 4 Cycles

constexpr Byte JSR  = 0x20; // Jump to Subroutine Absolute, 6 Cycles

#endif