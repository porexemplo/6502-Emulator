#include <gtest/gtest.h>
#include "Bus.hpp"

class M6502ST : public ::testing::Test {
protected:
    Bus bus;

    void SetUp() override {
        bus.Reset();
    }

    void TearDown() override {
        bus.Reset();
        bus.ClearMemory();
    }

    void TestStoreRegisterZP(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles = 3);
    void TestStoreRegisterABS(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles = 4);
    void TestStoreRegisterZPX(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles = 4);
    void TestStoreRegisterZPY(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles = 4);
};

void M6502ST::TestStoreRegisterZP(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles) {
    *reg = valueToStore;
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x0080, 0x00);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(bus.Read(0x0080), valueToStore);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

void M6502ST::TestStoreRegisterABS(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles) {
    *reg = valueToStore;
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0xFFFE, 0x44);
    bus.Write(0x4480, 0x00);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(bus.Read(0x4480), valueToStore);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

void M6502ST::TestStoreRegisterZPX(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles) {
    *reg = valueToStore;
    bus.cpu.X = 0x03;

    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x0083, 0x00);

    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(bus.Read(0x0083), valueToStore);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

void M6502ST::TestStoreRegisterZPY(Byte opcode, Byte* reg, Byte valueToStore, int32_t cycles) {
    *reg = valueToStore;
    bus.Write(0xFFFC, opcode);
    bus.Write(0xFFFD, 0x80);
    bus.Write(0x0083, 0x00);

    bus.cpu.Y = 0x03;
    CPU cpuCopy = bus.cpu;
    bus.Exec(cycles);

    ASSERT_EQ(bus.Read(0x0083), valueToStore);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}


TEST_F(M6502ST, STAZP_CanStoreValueInZeroPage) {
    TestStoreRegisterZP(STA_ZP, &bus.cpu.AC, 0x42);
}

TEST_F(M6502ST, STXZP_CanStoreValueInZeroPage) {
    TestStoreRegisterZP(STX_ZP, &bus.cpu.X, 0x42);
}

TEST_F(M6502ST, STYZP_CanStoreValueInZeroPage) {
    TestStoreRegisterZP(STY_ZP, &bus.cpu.Y, 0x42);
}

TEST_F(M6502ST, STAABS_CanStoreValueInAbsolute) {
    TestStoreRegisterABS(STA_ABS, &bus.cpu.AC, 0x42);
}

TEST_F(M6502ST, STXABS_CanStoreValueInAbsolute) {
    TestStoreRegisterABS(STX_ABS, &bus.cpu.X, 0x42);
}

TEST_F(M6502ST, STYABS_CanStoreValueInAbsolute) {
    TestStoreRegisterABS(STY_ABS, &bus.cpu.Y, 0x42);
}

TEST_F(M6502ST, STAZPX_CanStoreValueInZeroPageX) {
    TestStoreRegisterZPX(STA_ZPX, &bus.cpu.AC, 0x42);
}

TEST_F(M6502ST, STYZPX_CanStoreValueInZeroPageX) {
    TestStoreRegisterZPX(STY_ZPX, &bus.cpu.Y, 0x42);
}

TEST_F(M6502ST, STXZPY_CanStoreValueInZeroPageY) {
    TestStoreRegisterZPY(STX_ZPY, &bus.cpu.X, 0x42);
}

TEST_F(M6502ST, STA_ABSX_CanStoreValueInAbsoluteX) {
    bus.Write(0xFFFC, STA_ABSX);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, 0x80);
    
    bus.cpu.X = 0x0F;
    bus.cpu.AC = 0x42;

    CPU cpuCopy = bus.cpu;
    bus.Exec(5);

    ASSERT_EQ(bus.Read(0x800F), 0x42);
    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

TEST_F(M6502ST, STA_ABSY_CanStoreValueInAbsoluteX) {
    bus.Write(0xFFFC, STA_ABSY);
    bus.Write(0xFFFD, 0x00);
    bus.Write(0xFFFE, 0x80);
    
    bus.cpu.Y = 0x0F;
    bus.cpu.AC = 0x42;

    CPU cpuCopy = bus.cpu;
    bus.Exec(5);

    ASSERT_EQ(bus.Read(0x800F), 0x42);
    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

TEST_F(M6502ST, STA_INDX_CanStoreValueInIndirectX) {
    bus.Write(0xFFFC, STA_INDX);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.X = 0x04;

    bus.Write(0x0006, 0x80);
    bus.Write(0x0007, 0x44);
    bus.Write(0x4480, 0x00);

    bus.cpu.AC = 0x42;

    CPU cpuCopy = bus.cpu;
    bus.Exec(6);

    ASSERT_EQ(bus.Read(0x4480), 0x42);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}

TEST_F(M6502ST, STA_INDY_CanStoreValueInIndirectY) {
    bus.Write(0xFFFC, STA_INDY);
    bus.Write(0xFFFD, 0x02);
    bus.cpu.Y = 0x04;

    bus.Write(0x0002, 0x80);
    bus.Write(0x0003, 0x44);
    bus.Write(0x4484, 0x00);

    bus.cpu.AC = 0x42;

    CPU cpuCopy = bus.cpu;
    bus.Exec(6);

    ASSERT_EQ(bus.Read(0x4484), 0x42);

    ASSERT_EQ(bus.cpu.cycles, 0);

    ASSERT_EQ(bus.cpu.P, cpuCopy.P);
}