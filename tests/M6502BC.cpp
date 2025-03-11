/**
 * @author Redwane Hammas
 * @brief Unit test file for 6502 Emulator Branching instructions.
 * 
 * This file contains unit tests for the branching instructions of the 6502 CPU emulator.
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502BC : public ::testing::Test {
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

TEST_F(M6502BC, BEQCanBranchForwardWhenZeroIsSet) {
    bus.Reset(0xF000);
    bus.cpu.P.Z = true;

    bus.Write(0xF000, BEQ);
    bus.Write(0xF001, 0x02);

    bus.Exec(3);

    ASSERT_EQ(bus.cpu.PC, 0xF004);
}

TEST_F(M6502BC, BEQDoesNotBranchForwardWhenZeroIsNotSet) {
    bus.Reset(0xF000);
    bus.cpu.P.Z = false;

    bus.Write(0xF000, BEQ);
    bus.Write(0xF001, 0x02);

    bus.Exec(2);

    ASSERT_EQ(bus.cpu.PC, 0xF002);
}

TEST_F(M6502BC, BEQCanBranchForwardWhenItCrossesPageBoundary) {
    bus.Reset(0xF1FD);
    bus.cpu.P.Z = true;

    bus.Write(0xF1FD, BEQ);
    bus.Write(0xF1FE, 0x01);

    bus.Exec(5);

    ASSERT_EQ(bus.cpu.PC, 0xF200);
}

TEST_F(M6502BC, BEQCanBranchBackwardWhenZeroIsSet) {
    bus.Reset(0xF0CC);
    bus.cpu.P.Z = true;

    bus.Write(0xF0CC, BEQ);
    bus.Write(0xF0CD, static_cast<Byte>(-0x02));

    bus.Exec(3);

    ASSERT_EQ(bus.cpu.PC, 0xF0CC);
}

TEST_F(M6502BC, BEQDoesNotCanBranchBackwardWhenZeroIsNotSet) {
    bus.Reset(0xF0CC);
    bus.cpu.P.Z = false;

    bus.Write(0xF0CC, BEQ);
    bus.Write(0xF0CD, static_cast<Byte>(-0x02));

    bus.Exec(2);

    ASSERT_EQ(bus.cpu.PC, 0xF0CE);
}