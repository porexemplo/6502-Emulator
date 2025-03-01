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

TEST_F(M6502Test, LDA_IM_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x42);

    CPU cpuCopy = bus.cpu;
    bus.Exec(2);

    ASSERT_EQ(bus.cpu.AC, 0x42);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

TEST_F(M6502Test, LDA_ZP_CanLoadZeroPageValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0042, 0x15);

    CPU cpuCopy = bus.cpu;
    bus.Exec(3);

    ASSERT_EQ(bus.cpu.AC, 0x15);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

TEST_F(M6502Test, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWithZeroPageX) {
    bus.Write(0xFFFC, LDA_ZPX);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x0045, 0x15);

    bus.cpu.X = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(4);

    ASSERT_EQ(bus.cpu.AC, 0x15);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

TEST_F(M6502Test, LDA_ZPX_CanLoadZeroPageValueIntoAccumulatorWhenItWraps) {
    bus.Write(0xFFFC, LDA_ZPX);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x007F, 0x37);

    bus.cpu.X = 0xFF;
    CPU cpuCopy = bus.cpu;
    bus.Exec(4);

    ASSERT_EQ(bus.cpu.AC, 0x37);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0xFFFE);
    ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);
}

TEST_F(M6502Test, LDA_ABS_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_ABS);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4480, 0x11); // Little Endian loading

    CPU cpuCopy = bus.cpu;
    int32_t cyclesTaken = bus.Exec(4);

    ASSERT_EQ(bus.cpu.AC, 0x11);
    ASSERT_FALSE(bus.cpu.Z);
    ASSERT_FALSE(bus.cpu.N);

    ASSERT_EQ(bus.cpu.cycles, 0);
    // ASSERT_EQ(bus.cpu.PC, 0x1240);
    // ASSERT_EQ(bus.cpu.SP, 0x0100);

    CheckUnchangedRegisters(bus.cpu, cpuCopy);

    ASSERT_EQ(cyclesTaken, 4);
}

TEST_F(M6502Test, LDA_ABSX_CanLoadValueIntoAccumulator) {
    bus.Write(0xFFFC, LDA_ABSX);

    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);

    bus.Write(0x4482, 0x11); // Little Endian loading

    bus.cpu.X = 0x02;

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