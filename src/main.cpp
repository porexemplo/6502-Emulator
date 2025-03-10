#include "Bus.hpp"

int main() {
    Bus bus;
    
    bus.Reset();

    // Byte program[] = {0xA9, 0xFF, 0x85, 0x90, 0x8D, 0x00, 0x80, 0x49, 0xCC, 0x4C, 0x02, 0x00};
    Byte program[] = {0xA9, 0x00, 0x85, 0x42, 0xE6, 0x42, 0xA6, 0x42, 0xE8, 0x4C, 0x04, 0x10};

    uint32_t size = sizeof(program) / sizeof(Byte);

    Word START = bus.Load(program, size);

    bus.Reset(START);

    for (uint32_t clock = 10000; clock > 0; clock--) {
        clock -= bus.Exec(1);
        printf("A: %02X X: %02X Y: %02X SP: %02X PC: %04X\n", bus.cpu.AC, bus.cpu.X, bus.cpu.Y, bus.cpu.SP, bus.cpu.PC);
        printf("Cycles: %d\n", bus.cpu.cycles);
    }
    return 0;
}