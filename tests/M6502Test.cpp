#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502Test : public ::testing::Test {
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

    void TestLoadRegisterABS(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x37, int32_t cycles = 4);

    void TestLoadRegisterABSX(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);

    void TestLoadRegisterABSY(Byte opcode, Byte* registerToLoad, Byte valueToLoad = 0x42, int32_t cycles = 4);
};

static void CheckUnchangedRegisters(CPU& cpu, CPU& cpuCopy) {
    ASSERT_EQ(cpu.C, cpuCopy.C);
    ASSERT_EQ(cpu.I, cpuCopy.I);
    ASSERT_EQ(cpu.D, cpuCopy.D);
    ASSERT_EQ(cpu.B, cpuCopy.B);
    ASSERT_EQ(cpu.V, cpuCopy.V);
}

TEST_F(M6502Test, CPUDoesNothingWhenGivenZeroCycles) {
    CPU cpuCopy = bus.cpu;
    bus.Exec(0);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFC);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

TEST_F(M6502Test, CPUCanExecuteMoreCyclesIfRequiredByInstruction) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x42);

    CPU cpuCopy = bus.cpu;
    bus.Exec(1);

    ASSERT_EQ(bus.cpu.AC, 0x42);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, -1);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterIM(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, valueToLoad);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterZP(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, valueToLoad);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterZPX(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, valueToLoad);

    bus.cpu.X = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterZPXWrap(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x007F, valueToLoad);

    bus.cpu.X = 0xFF;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterABS(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, valueToLoad); // Little Endian loading

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFF);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

void M6502Test::TestLoadRegisterZPY(Byte opcode, Byte* registerToLoad, Byte valueToLoad, int32_t cycles) {
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, valueToLoad);

    bus.cpu.Y = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(*registerToLoad, valueToLoad);
    ASSERT_TRUE(bus.cpu.Z == (valueToLoad == 0));
    ASSERT_TRUE(bus.cpu.N == (valueToLoad & 0b10000000));

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

// LDA Opcodes

TEST_F(M6502Test, LDA_IM_CanLoadValueIntoAccumulator) {
    TestLoadRegisterIM(LDA_IM, &bus.cpu.AC);
}

TEST_F(M6502Test, LDA_ZP_CanLoadZeroPageValueIntoAccumulator) {
    TestLoadRegisterZP(LDA_ZP, &bus.cpu.AC);
}

TEST_F(M6502Test, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWithZeroPageX) {
    TestLoadRegisterZPX(LDA_ZPX, &bus.cpu.AC);
}

TEST_F(M6502Test, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWhenItWraps) {
    TestLoadRegisterZPXWrap(LDA_ZPX, &bus.cpu.AC);
}

TEST_F(M6502Test, LDA_ABS_CanLoadValueIntoAccumulator) {
    TestLoadRegisterABS(LDA_ABS, &bus.cpu.AC);
}

TEST_F(M6502Test, LDA_ABSX_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_ABSX);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.cpu.X = 0x02;
    bus.Write(0x4482, 0x11); // Little Endian loading


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(4);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 4);
}

TEST_F(M6502Test, LDA_ABSX_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    bus.Write(0xFFFC, LDA_ABSX);
    bus.cpu.X = 0xFF;

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4501, 0x11); // = 0x4402 + 0xFF which crosses page boundary


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(5);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 5);
}

TEST_F(M6502Test, LDA_ABSY_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_ABSY);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4482, 0x11); // Little Endian loading

    bus.cpu.Y = 0x02;

    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(4);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 4);
}

TEST_F(M6502Test, LDA_ABSY_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    bus.Write(0xFFFC, LDA_ABSY);
    bus.cpu.Y = 0xFF;

    bus.Write(0xFFFD, 0x02);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4501, 0x11); // = 0x4402 + 0xFF which crosses page boundary


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(5);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 5);
}

TEST_F(M6502Test, LDA_INDX_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_INDX);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.X = 0x04;

    bus.Write(0x0006, 0x44);
    bus.Write(0x0007, 0x12);

    bus.Write(0x1244, 0x11);

    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(6);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 6);
}

TEST_F(M6502Test, LDA_INDY_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_INDY);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0x04;

    bus.Write(0x0002, 0x44);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1248, 0x11);


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(5);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 5);
}

TEST_F(M6502Test, LDA_INDY_CanLoadValueIntoAccumulatorWhenItCrossesPageBoundary) {
    bus.Write(0xFFFC, LDA_INDY);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0xFF;

    bus.Write(0x0002, 0x02);
    bus.Write(0x0003, 0x12);

    bus.Write(0x1301, 0x11); // = 0x1202 + 0xFF which crosses page boundary


    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(6);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    
    CheckUnchangedRegisters(bus.cpu, cpuCopy);
    ASSERT_EQ(cyclesTaken, 6);
}

// LDX Opcodes

TEST_F(M6502Test, LDX_IM_CanLoadValueIntoX) {
    TestLoadRegisterIM(LDX_IM, &bus.cpu.X);
}

TEST_F(M6502Test, LDX_ZP_CanLoadZeroPageValueIntoX) {
    TestLoadRegisterZP(LDX_ZP, &bus.cpu.X);
}

TEST_F(M6502Test, LDX_ZPY_CanLoadZeroPageValueIntoXWithZeroPageY) {
    TestLoadRegisterZPY(LDX_ZPY, &bus.cpu.X);
}

TEST_F(M6502Test, LDX_ABS_CanLoadValueIntoX) {
    TestLoadRegisterABS(LDX_ABS, &bus.cpu.X);
}

// LDY Opcodes

TEST_F(M6502Test, LDY_IM_CanLoadValueIntoY) {
    TestLoadRegisterIM(LDY_IM, &bus.cpu.Y);
}

TEST_F(M6502Test, LDY_ZP_CanLoadZeroPageValueIntoY) {
    TestLoadRegisterZP(LDY_ZP, &bus.cpu.Y);
}

TEST_F(M6502Test, LDY_ZPX_CanLoadZeroPageValueIntoYWithZeroPageX) {
    TestLoadRegisterZPX(LDY_ZPX, &bus.cpu.Y);
}

TEST_F(M6502Test, LDY_ZPX_CanLoadZeroPageValueIntoYWhenItWraps) {
    TestLoadRegisterZPXWrap(LDY_ZPX, &bus.cpu.Y);
}

TEST_F(M6502Test, LDY_ABS_CanLoadValueIntoY) {
    TestLoadRegisterABS(LDY_ABS, &bus.cpu.Y);
}

// JSR Opcodes

TEST_F(M6502Test, JSR_CanJumpToSubroutine) {
    bus.Write(0xFFFC, JSR);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, 0x12);

    bus.Write(0x1242, LDA_IM); // Little Endian loading, Instruction to execute
    bus.Write(0x1243, 0x15);
    
    CPU cpuCopy = bus.cpu;
    bus.Exec(8);

    ASSERT_EQ(bus.cpu.AC, 0x15);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0x1244);
    ASSERT_EQ(bus.cpu.SP, 0x0102);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}