#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Types.hpp" 

class Memory {
public:
    static constexpr uint32_t SIZE = 64 * 1024; // 64KB
    Byte data[SIZE];
    Memory();

    Byte operator[](Word address) const;
    Byte& operator[](Word address);
};

#endif