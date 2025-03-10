/**
 * @brief This file contains test cases for increment and decrement instructions.
 * 
 * This test file is designed to verify the correct functionality of increment
 * and decrement operations in various scenarios.
 * 
 * @author Redwane Hammas
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502ID : public ::testing::Test {
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

TEST_F(M6502ID, INXCanIncrementNonZeroNonNegativeX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, INX);

    bus.cpu.X = 0x01;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x43);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, INXCanIncrementZeroX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, INX);

    bus.cpu.X = 0xFF;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x01);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, INXCanIncrementNegativeX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, INX);

    bus.cpu.X = 0x7F;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x81);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, INXCanIncrementToZero) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0xFF);
    bus.Write(0xFFFE, INX);

    bus.cpu.X = 0xFE;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0xFF);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, INYCanIncrementNonZeroNonNegativeY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, INY);

    bus.cpu.Y = 0x01;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x43);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, INYCanIncrementZeroY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, INY);

    bus.cpu.Y = 0xFF;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x01);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, INYCanIncrementNegativeY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, INY);

    bus.cpu.Y = 0x7F;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x81);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, INYCanIncrementToZero) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0xFF);
    bus.Write(0xFFFE, INY);

    bus.cpu.Y = 0xFE;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0xFF);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DEXCanDecrementNonZeroNonNegativeX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, DEX);

    bus.cpu.X = 0x43;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x41);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DEXCanDecrementZeroX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, DEX);

    bus.cpu.X = 0x01;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0xFF);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DEXCanDecrementNegativeX) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x82);
    bus.Write(0xFFFE, DEX);

    bus.cpu.X = 0x81;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x82);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x81);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DEXCanDecrementToZero) {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x01);
    bus.Write(0xFFFE, DEX);

    bus.cpu.X = 0x00;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x01);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DEYCanDecrementNonZeroNonNegativeY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, DEY);

    bus.cpu.Y = 0x43;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x41);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DEYCanDecrementZeroY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, DEY);

    bus.cpu.Y = 0x01;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0xFF);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DEYCanDecrementNegativeY) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0b10001001);
    bus.Write(0xFFFE, DEY);

    bus.cpu.Y = 0x81;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0b10001001);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0b10001000);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DEYCanDecrementToZero) {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x01);
    bus.Write(0xFFFE, DEY);

    bus.cpu.Y = 0x00;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x01);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPCanDecrementNonZeroNonNegative) {
    bus.Write(0xFFFC, DEC_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x42, 0x32);

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(5);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x42), 0x31);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPCanDecrementToZero) {
    bus.Write(0xFFFC, DEC_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x42, 0x01);

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(5);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x42), 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPCanDecrementNegative) {
    bus.Write(0xFFFC, DEC_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x42, 0x00);

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(5);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x42), 0xFF);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DECZPCanDecrementNegativeToZero) {
    bus.Write(0xFFFC, DEC_ZP);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x42, 0x81);

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(5);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x42), 0x80);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DECZPXCanDecrementNonZeroNonNegative) {
    bus.Write(0xFFFC, DEC_ZPX);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x44, 0x32);

    bus.cpu.X = 0x02;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(6);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x44), 0x31);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPXCanDecrementNonZeroNonNegativeWhenItWraps) {
    bus.Write(0xFFFC, DEC_ZPX);
    bus.Write(0xFFFD, 0xFC);
    bus.Write(0x80, 0x32);

    bus.cpu.X = 0x84;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(6);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x80), 0x31);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPXCanDecrementToZero) {
    bus.Write(0xFFFC, DEC_ZPX);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x44, 0x01);

    bus.cpu.X = 0x02;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(6);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x44), 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);
}

TEST_F(M6502ID, DECZPXCanDecrementNegative) {
    bus.Write(0xFFFC, DEC_ZPX);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0x44, 0x00);

    bus.cpu.X = 0x02;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(6);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x44), 0xFF);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);
}

TEST_F(M6502ID, DECABSCanDecrementNonZeroNonNegative) {
    bus.Reset(0xF000);
    bus.Write(0xF000, DEC_ABS);
    bus.Write(0xF001, 0x42);
    bus.Write(0xF002, 0x43);
    bus.Write(0x4342, 0x32);

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(6);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x4342), 0x31);
}

TEST_F(M6502ID, DECABSXCanDecrementNonZeroNonNegative) {
    bus.Reset(0xF000);
    bus.Write(0xF000, DEC_ABSX);
    bus.Write(0xF001, 0x42);
    bus.Write(0xF002, 0x43);
    bus.Write(0x4352, 0x32);

    bus.cpu.X = 0x10;

    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(7);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.Read(0x4352), 0x31);
}