#ifndef CPU_INSTRUCTIONS_HPP
#define CPU_INSTRUCTIONS_HPP

#include "Types.hpp"

enum class LogicalOp;
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

void ST_ZP(CPU& cpu, Byte reg);
void ST_ZPX(CPU& cpu, Byte reg);
void ST_ZPY(CPU& cpu, Byte reg);
void ST_ABS(CPU& cpu, Byte reg);
void ST_ABSX(CPU& cpu);
void ST_ABSY(CPU& cpu);
void ST_INDX(CPU& cpu);
void ST_INDY(CPU& cpu);

void INS_JSR(CPU& cpu);
void INS_RTS(CPU& cpu);
void INS_JMP_ABS(CPU& cpu);
void INS_JMP_IND(CPU& cpu);

void INS_TSX(CPU& cpu);
void INS_TXS(CPU& cpu);
void INS_PHA(CPU& cpu);
void INS_PHP(CPU& cpu);
void INS_PLA(CPU& cpu);
void INS_PLP(CPU& cpu);

// AND, ORA, EOR
void LOG_IM(CPU& cpu, LogicalOp op);
void LOG_ZP(CPU& cpu, LogicalOp op);
void LOG_ZPX(CPU& cpu, LogicalOp op);
void LOG_ABS(CPU& cpu, LogicalOp op);
void LOG_ABSX(CPU& cpu, LogicalOp op);
void LOG_ABSY(CPU& cpu, LogicalOp op);
void LOG_INDX(CPU& cpu, LogicalOp op);
void LOG_INDY(CPU& cpu, LogicalOp op);

void INS_BIT_ZP(CPU& cpu);
void INS_BIT_ABS(CPU& cpu);

// Transfer Instructions
void INS_TAX(CPU& cpu);
void INS_TAY(CPU& cpu);
void INS_TXA(CPU& cpu);
void INS_TYA(CPU& cpu);

// Increment and Decrement Instructions
void INS_INX(CPU& cpu);
void INS_INY(CPU& cpu);
void INS_DEX(CPU& cpu);
void INS_DEY(CPU& cpu);

#endif