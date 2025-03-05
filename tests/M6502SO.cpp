/**
 * @file M6502SO.cpp
 * @brief Unit tests for the stack operation instructions of the 6502 emulator.
 * 
 * This file contains tests for verifying the correct implementation of stack
 * operation instructions in the 6502 emulator.
 * 
 * @author Redwane HAMMAS
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502SO : public ::testing::Test {
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

TEST_F(M6502SO, TSXCanTransferStackPointerToX) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TSX);
    bus.cpu.PS = 0xFF;
    bus.cpu.SP = 0x10;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.X, bus.cpu.SP);
    EXPECT_EQ(bus.cpu.PS, 0x7D);
    EXPECT_EQ(bus.cpu.X, 0x10);
}

TEST_F(M6502SO, TSXCanSetZeroFlag) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TSX);
    bus.cpu.SP = 0x00;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.X, bus.cpu.SP);
    EXPECT_EQ(bus.cpu.P.Z, 1);
}

TEST_F(M6502SO, TSXCanSetNegativeFlag) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TSX);
    bus.cpu.SP = 0xFF;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.X, bus.cpu.SP);
    EXPECT_EQ(bus.cpu.P.N, 1);
}