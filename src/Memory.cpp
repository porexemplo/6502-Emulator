#include "Memory.hpp"

Memory::Memory() {
    for (uint32_t i = 0; i < SIZE; i++) {
        data[i] = 0;
    }
}

uint8_t Memory::operator[](Word address) const {
    return data[address];
}

uint8_t& Memory::operator[](Word address) {
    return data[address];
}