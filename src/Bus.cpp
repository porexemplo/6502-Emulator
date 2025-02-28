#include "Bus.hpp"
#include <iostream>

Bus::Bus() : cpu(memory) {}
Bus::~Bus() {}

void Bus::Write(uint32_t address, Byte data) {
    this->memory[address] = data;
}

Byte Bus::Read(uint32_t address) {
    return memory[address];
}

void Bus::Reset() {
    cpu.Reset();
}

void Bus::ClearMemory() {
    for (uint32_t i = 0; i < Memory::SIZE; i++) {
        memory[i] = 0;
    }
}

int32_t Bus::Exec(int32_t cycles) {
    return cpu.Execute(cycles);
}
