#include "Bus.hpp"

int main() {
    Bus bus;

    bus.Reset();
    bus.Write(0xFFFC, LDA_ZP); // LDA Zero Page
    bus.Write(0xFFFD, 0x42); // Value to load
    bus.Write(0x0042, 0x84); // Value to load

    bus.Exec(3);
    return 0;
}