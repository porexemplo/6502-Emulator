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

void Bus::Reset(Word address) {
    cpu.Reset(address);
}

void Bus::ClearMemory() {
    for (uint32_t i = 0; i < Memory::SIZE; i++) {
        memory[i] = 0;
    }
}

int32_t Bus::Exec(int32_t cycles) {
    return cpu.Execute(cycles);
}

Word Bus::Load(Byte* program, uint32_t size) {
    if (program == nullptr) {
        std::cerr << "Program is null" << std::endl;
        return 0;
    }

    if (size > Memory::SIZE) {
        std::cerr << "Program is too large" << std::endl;
        return 0;
    }

    for (uint32_t i = 0; i < size; i++) {
        memory[0x1000 + i] = program[i];
    }

    return 0x1000;
}
