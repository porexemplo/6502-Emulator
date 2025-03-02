#ifndef BUS_HPP
#define BUS_HPP

#include "Types.hpp"
#include "Memory.hpp"
#include "Cpu.hpp"
#include <vector>

class Bus {
public:
    Bus();
    ~Bus();

    // Devices on the bus
    Memory memory;
    CPU cpu;

    void Write(uint32_t adress, Byte data);
    Byte Read(uint32_t adress);

    void Reset();
    void Reset(Word address);
    void ClearMemory();
    int32_t Exec(int32_t cycles);
};

#endif