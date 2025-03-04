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