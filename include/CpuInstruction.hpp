#ifndef CPU_INSTRUCTIONS_HPP
#define CPU_INSTRUCTIONS_HPP

#include "Types.hpp"

class CPU;

void LDSetFlags(CPU& cpu, Byte value);

void LD_IM(CPU& cpu, Byte& reg);
void LD_ZP(CPU& cpu, Byte& reg);
void LD_ZPX(CPU& cpu, Byte& reg);
void LD_ZPY(CPU& cpu, Byte& reg);
void LD_ABS(CPU& cpu, Byte& reg);
void LD_ABSX(CPU& cpu, Byte& reg);
void LD_ABSY(CPU& cpu, Byte& reg);
void LD_INDX(CPU& cpu);
void LD_INDY(CPU& cpu);

void INS_JSR(CPU& cpu);

#endif