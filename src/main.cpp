#include "Bus.hpp"

int main() {
    Bus bus;

    bus.Reset();
    bus.Write(0xFFFC, JSR); // Instruction to execute
    bus.Write(0xFFFD, 0x42); // Address to jump (Byte 1)
    bus.Write(0xFFFE, 0x12); // Address to jump (Byte 2)

    bus.Write(0x1242, LDA_IM); // Instruction to execute
    bus.Write(0x1243, 0x15); // Value to load into the accumulator

    // Should load 0x84 into the accumulator

    bus.Exec(8);
    return 0;
}