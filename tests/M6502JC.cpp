#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502JC : public ::testing::Test {
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

static void CheckUnchangedFlags(CPU& cpu, CPU& cpuCopy) {
    ASSERT_EQ(cpu.C, cpuCopy.C);
    ASSERT_EQ(cpu.I, cpuCopy.I);
    ASSERT_EQ(cpu.D, cpuCopy.D);
    ASSERT_EQ(cpu.B, cpuCopy.B);
    ASSERT_EQ(cpu.V, cpuCopy.V);
    ASSERT_EQ(cpu.N, cpuCopy.N);
    ASSERT_EQ(cpu.Z, cpuCopy.Z);
}

TEST_F(M6502JC, JSR_CanJumpToSubroutine) {
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
    ASSERT_EQ(bus.cpu.SP, 0x00FD);

    CheckUnchangedFlags(bus.cpu, cpuCopy);
}

TEST_F(M6502JC, CanJumpToSubroutineAndJumpBack) {
    bus.Reset(0xFF00);

    bus.Write(0xFF00, JSR);
    bus.Write(0xFF01, 0x00);
    bus.Write(0xFF02, 0x80);
    bus.Write(0xFF03, LDA_IM);
    bus.Write(0xFF04, 0x12);
    
    bus.Write(0x8000, RTS);
    
    CPU cpuCopy = bus.cpu;
    bus.Exec(6+6+2);

    ASSERT_EQ(bus.cpu.AC, 0x12);

    CheckUnchangedFlags(bus.cpu, cpuCopy);
}