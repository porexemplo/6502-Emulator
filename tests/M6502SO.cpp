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

TEST_F(M6502SO, TXSCanTransferXToStackPointer) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TXS);
    bus.cpu.PS = 0xFF;
    bus.cpu.X = 0x10;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.SP, bus.cpu.X);
    EXPECT_EQ(bus.cpu.PS, 0x7D);
    EXPECT_EQ(bus.cpu.SP, 0x10);
}

TEST_F(M6502SO, TXSCanSetZeroFlag) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TXS);
    bus.cpu.X = 0x00;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.SP, bus.cpu.X);
    EXPECT_EQ(bus.cpu.P.Z, 1);
}

TEST_F(M6502SO, TXSCanSetNegativeFlag) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, TXS);
    bus.cpu.X = 0xFF;

    bus.Exec(2);

    EXPECT_EQ(bus.cpu.SP, bus.cpu.X);
    EXPECT_EQ(bus.cpu.P.N, 1);
}

TEST_F(M6502SO, PHACanPushAccumulator) {
    bus.Reset(0xFF10);
    bus.cpu.PS = 0xFF;
    bus.Write(0xFF10, PHA);
    bus.cpu.AC = 0x10;

    bus.Exec(3);

    EXPECT_EQ(bus.cpu.cycles, 0);
    EXPECT_EQ(bus.cpu.ReadWord(0x1FF), 0x10);
    EXPECT_EQ(bus.cpu.SP, 0xFE);
    EXPECT_EQ(bus.cpu.PS, 0xFF);
}

TEST_F(M6502SO, PHPCanPushProcessorStatus) {
    bus.Reset(0xFF10);
    bus.Write(0xFF10, PHP);
    bus.cpu.SP = 0xEF;
    bus.cpu.PS = 0x7D;

    bus.Exec(3);

    EXPECT_EQ(bus.cpu.cycles, 0);
    EXPECT_EQ(bus.cpu.ReadWord(0x1EF), 0x7D);
    EXPECT_EQ(bus.cpu.SP, 0xEE);
    EXPECT_EQ(bus.cpu.PS, 0x7D);
}