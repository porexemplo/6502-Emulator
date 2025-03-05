/**
 * @file M6502JC.cpp
 * @brief Unit tests for the 6502 CPU emulator's jumping instructions.
 * 
 * This file contains tests for various jumping instructions of the 6502 CPU emulator.
 * The tests are written using the Google Test framework.
 * 
 * @author Redwane HAMMAS
 * 
 * @class M6502JC
 * @brief Test fixture for setting up and tearing down the test environment.
 * 
 * @test JSR_CanJumpToSubroutine
 * Tests the JSR (Jump to Subroutine) instruction.
 * 
 * @test CanJumpToSubroutineAndJumpBack
 * Tests the JSR (Jump to Subroutine) and RTS (Return from Subroutine) instructions.
 * 
 * @test JMP_ABSCanJumpToANewLocationInTheProgram
 * Tests the JMP (Jump) instruction with absolute addressing.
 * 
 * @test JMP_INDCanJumpToANewLocationInTheProgram
 * Tests the JMP (Jump) instruction with indirect addressing.
 */

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

TEST_F(M6502JC, JSR_CanJumpToSubroutine) {
    bus.Write(0xFFFC, JSR);
    bus.Write(0xFFFD, 0x42);
    bus.Write(0xFFFE, 0x12);

    bus.Write(0x1242, LDA_IM); // Little Endian loading, Instruction to execute
    bus.Write(0x1243, 0x15);
    
    CPU cpuCopy = bus.cpu;
    bus.Exec(8);

    ASSERT_EQ(bus.cpu.AC, 0x15);

    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PC, 0x1244);
    ASSERT_EQ(bus.cpu.SP, 0x00FD);
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
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
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
}

TEST_F(M6502JC, JMP_ABSCanJumpToANewLocationInTheProgram) {
    bus.Reset(0xFF00);

    bus.Write(0xFF00, JMP_ABS);
    bus.Write(0xFF01, 0x00);
    bus.Write(0xFF02, 0x80);    
    bus.Write(0x8000, RTS);
    
    CPU cpuCopy = bus.cpu;
    bus.Exec(3);

    ASSERT_EQ(bus.cpu.PC, 0x8000);
    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
    ASSERT_EQ(bus.cpu.SP, cpuCopy.SP);
}

TEST_F(M6502JC, JMP_INDCanJumpToANewLocationInTheProgram) {
    bus.Reset(0xFF00);

    bus.Write(0xFF00, JMP_IND);
    bus.Write(0xFF01, 0x00);
    bus.Write(0xFF02, 0x80);
    bus.Write(0x8000, 0x00);
    bus.Write(0x8001, 0x81);
    
    bus.Write(0x8100, 0x21);
    
    CPU cpuCopy = bus.cpu;
    bus.Exec(5);

    ASSERT_EQ(bus.cpu.PC, 0x8100);
    ASSERT_EQ(bus.cpu.cycles, 0);
    ASSERT_EQ(bus.cpu.PS, cpuCopy.PS);
    ASSERT_EQ(bus.cpu.SP, cpuCopy.SP);
}