/**
 * @file M6502LD.cpp
 * @brief Unit tests for the 6502 CPU Load Accumulator (LDA), Load X Register (LDX), and Load Y Register (LDY) instructions.
 * 
 * This file contains a series of unit tests for verifying the correct functionality of the 6502 CPU's load instructions.
 * These tests are implemented using the Google Test framework.
 * 
 * @author Redwane HAMMAS
 * 
 * @class M6502LD
 * @brief Test fixture for setting up and tearing down the test environment.
 * 
 * The tests cover various addressing modes for the LDA, LDX, and LDY instructions, including:
 * - Immediate
 * - Zero Page
 * - Zero Page,X
 * - Zero Page,Y
 * - Absolute
 * - Absolute,X
 * - Absolute,Y
 * - Indexed Indirect (LDA only)
 * - Indirect Indexed (LDA only)
 * 
 * Each test ensures that the CPU correctly loads the specified value into the appropriate register (A, X, or Y),
 * and verifies the CPU's state after execution, including the program counter, stack pointer, and processor status flags.
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502LD : public ::testing::Test {
protected:

    Bus bus;

    void SetUp() override {
        bus.Reset();
    }

    void TearDown() override {
        bus.Reset();
        bus.ClearMemory();
    }

    void TestLoadRegisterIM(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 2);

    void TestLoadRegisterZP(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 3);

    void TestLoadRegisterZPX(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterZPXWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterZPY(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterZPYWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterABS(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x37, int32_t cycles = 4);

    void TestLoadRegisterABSX(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterABSXWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterABSY(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterABSYWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);
};

TEST_F(M6502LD, CPUDoesNothingWhenGivenZeroCycles) {
    CPU cpuCopy = bus.cpu;
    bus.Exec(0);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFC);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

TEST_F(M6502LD, CPUCanExecuteMoreCyclesIfRequiredByInstruction) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x42);

    CPU cpuCopy = bus.cpu;
    bus.Exec(1);

    ASSERT_EQ(bus.cpu.AC, 0x42);
    ASSERT_FALSE(bus.cpu.P.Z);
    ASSERT_FALSE(bus.cpu.P.N);

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterIM(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, valueToLoad);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterZP(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, valueToLoad);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterZPX(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, valueToLoad);

    bus.cpu.X = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterZPXWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x007F, valueToLoad);

    bus.cpu.X = 0xFF;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterZPY(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, valueToLoad);

    bus.cpu.Y = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterZPYWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x007F, valueToLoad);

    bus.cpu.Y = 0xFF;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterABS(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, valueToLoad); // Little Endian loading

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterABSX(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.X = 0x02;
    bus.Write(0x4482, valueToLoad); // Little Endian loading

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterABSXWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.X = 0xFF;
    bus.Write(0x4501, valueToLoad); // = 0x4482 + 0xFF which crosses page boundary

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterABSY(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.Y = 0x02;
    bus.Write(0x4482, valueToLoad); // Little Endian loading

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);

    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

void M6502LD::TestLoadRegisterABSYWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.Y = 0xFF;
    bus.Write(0x4501, valueToLoad); // = 0x4482 + 0xFF which crosses page boundary

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.P.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.P.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0xFF);
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

// LDA Opcodes

TEST_F(M6502LD, LDA_IM_CanLoadValueIntoAccumulator) {
    TestLoadRegisterIM(LDA_IM, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ZP_CanLoadZeroPageValueIntoAccumulator) {
    TestLoadRegisterZP(LDA_ZP, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWithZeroPageX) {
    TestLoadRegisterZPX(LDA_ZPX, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWhenItWraps) {
    TestLoadRegisterZPXWrap(LDA_ZPX, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ABS_CanLoadValueIntoAccumulator) {
    TestLoadRegisterABS(LDA_ABS, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ABSX_CanLoadValueIntoAccumulator) {
    TestLoadRegisterABSX(LDA_ABSX, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ABSX_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    TestLoadRegisterABSXWrap(LDA_ABSX, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ABSY_CanLoadValueIntoAccumulator) {
    TestLoadRegisterABSY(LDA_ABSY, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_ABSY_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    TestLoadRegisterABSYWrap(LDA_ABSY, &bus.cpu.AC);
}

TEST_F(M6502LD, LDA_INDX_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_INDX);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.X = 0x04;

    bus.Write(0x0006, 0x44);
    bus.Write(0x0007, 0x12);

    bus.Write(0x1244, 0x11);

    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(6);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.P.Z);
    ASSERT_FALSE(bus.cpu.P.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
    ASSERT_EQ(cyclesTaken, 6);
}

TEST_F(M6502LD, LDA_INDY_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_INDY);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0x04;

    bus.Write(0x0002, 0x44);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1248, 0x11);


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(5);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.P.Z);
    ASSERT_FALSE(bus.cpu.P.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
    ASSERT_EQ(cyclesTaken, 5);
}

TEST_F(M6502LD, LDA_INDY_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    bus.Write(0xFFFC, LDA_INDY);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0xFF;

    bus.Write(0x0002, 0x02);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1301, 0x11); // = 0x1202 + 0xFF which crosses page boundary


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(6);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.P.Z);
    ASSERT_FALSE(bus.cpu.P.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
    ASSERT_EQ(cyclesTaken, 6);
}

// LDX Opcodes

TEST_F(M6502LD, LDX_IM_CanLoadValueIntoX) {
    TestLoadRegisterIM(LDX_IM, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ZP_CanLoadZeroPageValueIntoX) {
    TestLoadRegisterZP(LDX_ZP, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ZPY_CanLoadZeroPageValueIntoXWithZeroPageY) {
    TestLoadRegisterZPY(LDX_ZPY, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ZPY_CanLoadZeroPageValueIntoXWhenItWraps) {
    TestLoadRegisterZPYWrap(LDX_ZPY, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ABS_CanLoadValueIntoX) {
    TestLoadRegisterABS(LDX_ABS, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ABSY_CanLoadValueIntoX) {
    TestLoadRegisterABSY(LDX_ABSY, &bus.cpu.X);
}

TEST_F(M6502LD, LDX_ABSY_CanLoadValueIntoXWhenItCrossesPageBoundary) {
    TestLoadRegisterABSYWrap(LDX_ABSY, &bus.cpu.X);
}

// LDY Opcodes

TEST_F(M6502LD, LDY_IM_CanLoadValueIntoY) {
    TestLoadRegisterIM(LDY_IM, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ZP_CanLoadZeroPageValueIntoY) {
    TestLoadRegisterZP(LDY_ZP, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ZPX_CanLoadZeroPageValueIntoYWithZeroPageX) {
    TestLoadRegisterZPX(LDY_ZPX, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ZPX_CanLoadZeroPageValueIntoYWhenItWraps) {
    TestLoadRegisterZPXWrap(LDY_ZPX, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ABS_CanLoadValueIntoY) {
    TestLoadRegisterABS(LDY_ABS, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ABSX_CanLoadValueIntoY) {
    TestLoadRegisterABSX(LDY_ABSX, &bus.cpu.Y);
}

TEST_F(M6502LD, LDY_ABSX_CanLoadValueIntoYWhenItCrossesPageBoundary) {
    TestLoadRegisterABSXWrap(LDY_ABSX, &bus.cpu.Y);
}