/**
 * @file
 * @author Redwane Hammas
 * @brief Test file for the register transfers opcodes.
 */

#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502RT : public ::testing::Test {
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

TEST_F(M6502RT, TAXCanTransferNonZeroNonNegativeAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, TAX);

    bus.cpu.X = 0x00;
    bus.cpu.P.Z = true;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x42);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TAXCanTransferZeroAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, TAX);

    bus.cpu.X = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TAXCanTransferNegativeAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, TAX);

    bus.cpu.X = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x80);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);  
}

TEST_F(M6502RT, TAYCanTransferNonZeroNonNegativeAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, TAY);

    bus.cpu.Y = 0x00;
    bus.cpu.P.Z = true;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x42);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TAYCanTransferZeroAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, TAY);

    bus.cpu.Y = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TAYCanTransferNegativeAccumulatorToX) {
    bus.Write(0xFFFC, LDA_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, TAY);

    bus.cpu.Y = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x80);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);  
}

TEST_F(M6502RT, TXACanTransferNonZeroNonNegativeXToAccumulator)  {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, TXA);

    bus.cpu.AC = 0x00;
    bus.cpu.P.Z = true;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x42);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TXACanTransferZeroXToAccumulator)  {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, TXA);

    bus.cpu.AC = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TXACanTransferNegativeXToAccumulator)  {
    bus.Write(0xFFFC, LDX_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, TXA);

    bus.cpu.AC = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.X, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x80);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);  
}

TEST_F(M6502RT, TYACanTransferNonZeroNonNegativeYToAccumulator)  {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, TYA);

    bus.cpu.AC = 0x00;
    bus.cpu.P.Z = true;
    bus.cpu.P.N = true;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x42);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x42);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TYACanTransferZeroYToAccumulator)  {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, TYA);

    bus.cpu.AC = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x00);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x00);

    ASSERT_EQ(bus.cpu.P.Z, true);
    ASSERT_EQ(bus.cpu.P.N, false);  
}

TEST_F(M6502RT, TYACanTransferNegativeYToAccumulator)  {
    bus.Write(0xFFFC, LDY_IM);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, TYA);

    bus.cpu.AC = 0x42;
    bus.cpu.P.Z = false;
    bus.cpu.P.N = false;

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.Y, 0x80);

    bus.Exec(2);
    ASSERT_EQ(bus.cpu.AC, 0x80);

    ASSERT_EQ(bus.cpu.P.Z, false);
    ASSERT_EQ(bus.cpu.P.N, true);  
}