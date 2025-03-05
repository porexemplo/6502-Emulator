/**
 * @file M6502LG.cpp
 * @brief Unit tests for the 6502 CPU Logical Instructions : AND, ORA, EOR and BIT.
 * 
 * This file contains a series of unit tests for verifying the correct functionality of the 6502 CPU's load instructions.
 * These tests are implemented using the Google Test framework.
 * 
 * @author Redwane HAMMAS
 * 
 * @class M6502LG
 * @brief Test fixture for setting up and tearing down the test environment.
 * 
 * The tests cover various addressing modes for the AND, ORA, EOR and BIT instructions, including:
 * - Immediate
 * - Zero Page
 * - Zero Page,X
 * - Zero Page,Y
 * - Absolute
 * - Absolute,X
 * - Absolute,Y
 * - Indexed Indirect
 * - Indirect Indexed
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502LG : public ::testing::Test {
protected:

    Bus bus;

    void SetUp() override {
        bus.Reset();
    }

    void TearDown() override {
        bus.Reset();
        bus.ClearMemory();
    }

    void TestLogicalIM(LogicalOp op, Byte value = 0xF0, int32_t cycles = 2);

    void TestLogicalZP(LogicalOp op, Byte value = 0x42, int32_t cycles = 3);

    void TestLogicalZPX(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalZPXWrap(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalZPY(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalZPYWrap(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalABS(LogicalOp op, Byte value = 0x37, int32_t cycles = 4);

    void TestLogicalABSX(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalABSXWrap(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalABSY(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);

    void TestLogicalABSYWrap(LogicalOp op, Byte value = 0x42, int32_t cycles = 4);
    
    void TestLogicalINDX(LogicalOp op, Byte value = 0x42, int32_t cycles = 6);
    
    void TestLogicalINDY(LogicalOp op, Byte value = 0x42, int32_t cycles = 5);
    
    void TestLogicalINDYWrap(LogicalOp op, Byte value = 0x42, int32_t cycles = 6);
};

Byte Op(LogicalOp op, Byte reg, Byte value) {
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

void M6502LG::TestLogicalIM(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_IM);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_IM);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_IM);
            break;
        default:
            throw -1;
    }

    bus.cpu.AC = 0x82;

    bus.Write(0xFFFD, value);
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalZP(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ZP);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ZP);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ZP);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, value);

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalZPX(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ZPX);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ZPX);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ZPX);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, value);

    bus.cpu.X = 0x03;
    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalZPXWrap(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ZPX);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ZPX);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ZPX);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x80);
    bus.Write(0x007F, value);

    bus.cpu.X = 0xFF;
    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalABS(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ABS);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ABS);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ABS);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, value); // Little Endian loading

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalABSX(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ABSX);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ABSX);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ABSX);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.X = 0x02;
    bus.Write(0x4482, value); // Little Endian loading

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalABSXWrap(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ABSX);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ABSX);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ABSX);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.X = 0xFF;
    bus.Write(0x4501, value); // = 0x4482 + 0xFF which crosses page boundary

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalABSY(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ABSY);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ABSY);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ABSY);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.Y = 0x02;
    bus.Write(0x4482, value); // Little Endian loading

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalABSYWrap(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_ABSY);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_ABSY);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_ABSY);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.Y = 0xFF;
    bus.Write(0x4501, value); // = 0x4482 + 0xFF which crosses page boundary

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalINDX(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_INDX);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_INDX);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_INDX);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x02);
    bus.cpu.X = 0x04;

    bus.Write(0x0006, 0x44);
    bus.Write(0x0007, 0x12);

    bus.Write(0x1244, value);

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalINDY(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_INDY);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_INDY);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_INDY);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0x04;

    bus.Write(0x0002, 0x44);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1248, value);

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

void M6502LG::TestLogicalINDYWrap(LogicalOp op, Byte value, int32_t cycles) {
    switch (op) {
        case LogicalOp::AND:
            bus.Write(0xFFFC, AND_INDY);
            break;
        case LogicalOp::ORA:
            bus.Write(0xFFFC, ORA_INDY);
            break;
        case LogicalOp::EOR:
            bus.Write(0xFFFC, EOR_INDY);
            break;
        default:
            throw -1;
    }

    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0xFF;

    bus.Write(0x0002, 0x02);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1301, value); // = 0x1202 + 0xFF which crosses page boundary

    bus.cpu.AC = 0x82;
    bus.Exec(cycles);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.AC, Op(op, 0x82, value));
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.P.Z, (bool)(Op(op, 0x82, value) == 0));
    ASSERT_EQ(bus.cpu.P.N, (bool)(Op(op, 0x82, value) & 0b10000000));
}

TEST_F(M6502LG, AND_IM_CanLogicalAndImmediateValueWithAccumulator) {
    TestLogicalIM(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_IM_CanLogicalEORImmediateValueWithAccumulator) {
    TestLogicalIM(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_IM_CanLogicalOrImmediateValueWithAccumulator) {
    TestLogicalIM(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ZP_CanLogicalAndZeroPageValueWithAccumulator) {
    TestLogicalZP(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ZP_CanLogicalEORZeroPageValueWithAccumulator) {
    TestLogicalZP(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ZP_CanLogicalOrZeroPageValueWithAccumulator) {
    TestLogicalZP(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ZPX_CanLogicalAndZeroPageValueWithAccumulator) {
    TestLogicalZPX(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ZPX_CanLogicalEORZeroPageValueWithAccumulator) {
    TestLogicalZPX(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ZPX_CanLogicalOrZeroPageValueWithAccumulator) {
    TestLogicalZPX(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ZPX_CanLogicalAndZeroPageValueWithAccumulatorWhenItWraps) {
    TestLogicalZPXWrap(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ZPX_CanLogicalAndZeroPageValueWithAccumulatorWhenItWraps) {
    TestLogicalZPXWrap(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ZPX_CanLogicalAndZeroPageValueWithAccumulatorWhenItWraps) {
    TestLogicalZPXWrap(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ABS_CanLogicalAndValueWithAccumulator) {
    TestLogicalABS(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ABS_CanLogicalAndValueWithAccumulator) {
    TestLogicalABS(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ABS_CanLogicalAndValueWithAccumulator) {
    TestLogicalABS(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ABSX_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSX(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ABSX_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSX(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ABSX_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSX(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ABSX_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSXWrap(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ABSX_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSXWrap(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ABSX_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSXWrap(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ABSY_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSY(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ABSY_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSY(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ABSY_CanLogicalAndValueWithAccumulator) {
    TestLogicalABSY(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_ABSY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSYWrap(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_ABSY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSYWrap(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_ABSY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalABSYWrap(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_INDX_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDX(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_INDX_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDX(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_INDX_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDX(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_INDY_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDY(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_INDY_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDY(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_INDY_CanLogicalAndValueWithAccumulator) {
    TestLogicalINDY(LogicalOp::ORA);
}

TEST_F(M6502LG, AND_INDY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalINDYWrap(LogicalOp::AND);
}

TEST_F(M6502LG, EOR_INDY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalINDYWrap(LogicalOp::EOR);
}

TEST_F(M6502LG, ORA_INDY_CanLogicalAndValueWithAccumulatorWhenItCrossesPageBoundary) {
    TestLogicalINDYWrap(LogicalOp::ORA);
}

TEST_F(M6502LG, BIT_ZP_CanTestBitsOfZeroPageValue) {
    bus.Write(0xFFFC, BIT_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, 0xCC);

    bus.cpu.PS = 0x00;
    bus.cpu.AC = 0xCC;
    bus.Exec(3);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0xCC);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}

TEST_F(M6502LG, BIT_ZP_CanTestBitsOfZeroPageValueWithZeroAccumulator) {
    bus.Write(0xFFFC, BIT_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, 0xCC);

    bus.cpu.PS = 0x00;
    bus.cpu.AC = 0x00;
    bus.Exec(3);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, true);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0x00);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}

TEST_F(M6502LG, BIT_ZP_CanTestBitsOfZeroPageValueWithOverflow) {
    bus.Write(0xFFFC, BIT_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, 0x43);

    bus.cpu.PS = 0xFF;
    bus.cpu.AC = 0x42;
    bus.Exec(3); // Result is 0x42 & 0x43 = 0x42

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0x42);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}

TEST_F(M6502LG, BIT_ABS_CanTestBitsOfAbsoluteValue) {
    bus.Write(0xFFFC, BIT_ABS);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, 0xCC);

    bus.cpu.PS = 0x00;
    bus.cpu.AC = 0xCC;
    bus.Exec(4);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0xCC);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}

TEST_F(M6502LG, BIT_ABS_CanTestBitsOfAbsoluteValueWithZeroAccumulator) {
    bus.Write(0xFFFC, BIT_ABS);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, 0xCC);

    bus.cpu.PS = 0x00;
    bus.cpu.AC = 0x00;
    bus.Exec(4);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, true);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0x00);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}

TEST_F(M6502LG, BIT_ABS_CanTestBitsOfAbsoluteValueWithOverflow) {
    bus.Write(0xFFFC, BIT_ABS);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, 0x43);

    bus.cpu.PS = 0xFF;
    bus.cpu.AC = 0x42;
    bus.Exec(4); // Result is 0x42 & 0x43 = 0x42

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
    ASSERT_EQ(bus.cpu.P.V, true);
    ASSERT_EQ(bus.cpu.AC, 0x42);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
}