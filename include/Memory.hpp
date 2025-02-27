#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Types.hpp" 

class Memory {
private:
    static constexpr uint32_t SIZE = 64 * 1024; // 64KB
    Byte data[SIZE];
public:
    Memory();

    Byte operator[](Word address) const;
    Byte& operator[](Word address);
};

#endif