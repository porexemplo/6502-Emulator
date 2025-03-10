# 6502-Emulator
C++ CPU emulator (6502)

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Progress](#progress)
- [Contributing](#contributing)
- [License](#license)

## Introduction
This project is a C++ emulator (in progress) for the 6502 CPU, which was used in many classic computers and gaming consoles.

## Features
- Accurate emulation of the 6502 CPU
- Support for various 6502-based systems

## Installation
To install the emulator, clone the repository and build the project using your preferred C++ compiler.

```sh
git clone https://github.com/porexemplo/6502-Emulator
cd 6502-Emulator
make
```

## Execute Tests
To run the tests, execute the following command:

```sh
make test
```

## Usage
Include the bus in and load a program to emulate 6502 instructions.

```cpp
#include "Bus.hpp"

int main() {
    Bus bus;

    Word START = bus.Load(program, size);
    bus.Reset(START);

    for (uint32_t clock = 10000; clock > 0; clock--)
        clock -= bus.Exec(1);

    return 0;
}
```

## Example Programs
Here are a few example programs to get you started with the 6502 emulator:

### Example : Infinite Loop 
This program will store the value `0xFF` at memory addresses `0x0090` and `0x8000`, `XOR` the value at memory address `0x00CC` with the accumulator, and then jump back to the `START` label.

```assembly
LDA #$FF        ; Load the accumulator with the value 0xFF

START:
    STA $90     ; Store the accumulator value at memory address 0x0090
    STA $8000   ; Store the accumulator value at memory address 0x8000
    EOR $CC     ; XOR the accumulator with the value at memory address 0x00CC
    JMP START   ; Jump back to the START label
```

The compiled program can be loaded into the emulator using the following code:

```cpp
Byte program[] = {0xA9, 0xFF, 0x85, 0x90, 0x8D, 0x00, 0x80, 0x49, 0xCC, 0x4C, 0x02, 0x00};
```
## Progress
Implemented instructions:
- `LDA_IM`   - `0xA9` (Load Accumulator Immediate, 2 Cycles)
- `LDA_ZP`   - `0xA5` (Load Accumulator Zero Page, 3 Cycles)
- `LDA_ZPX`  - `0xB5` (Load Accumulator Zero Page X, 4 Cycles)
- `LDA_ABS`  - `0xAD` (Load Accumulator Absolute, 4 Cycles)
- `LDA_ABSX` - `0xBD` (Load Accumulator Absolute X, 4+ Cycles)
- `LDA_ABSY` - `0xB9` (Load Accumulator Absolute Y, 4+ Cycles)
- `LDA_INDX` - `0xA1` (Load Accumulator Indirect X, 6 Cycles)
- `LDA_INDY` - `0xB1` (Load Accumulator Indirect Y, 5+ Cycles)
- `LDX_IM`   - `0xA2` (Load X Register Immediate, 2 Cycles)
- `LDX_ZP`   - `0xA6` (Load X Register Zero Page, 3 Cycles)
- `LDX_ZPY`  - `0xB6` (Load X Register Zero Page Y, 4 Cycles)
- `LDX_ABS`  - `0xAE` (Load X Register Absolute, 4 Cycles)
- `LDX_ABSY` - `0xBE` (Load X Register Absolute Y, 4+ Cycles)
- `LDY_IM`   - `0xA0` (Load Y Register Immediate, 2 Cycles)
- `LDY_ZP`   - `0xA4` (Load Y Register Zero Page, 3 Cycles)
- `LDY_ZPX`  - `0xB4` (Load Y Register Zero Page X, 4 Cycles)
- `LDY_ABS`  - `0xAC` (Load Y Register Absolute, 4 Cycles)
- `LDY_ABSX` - `0xBC` (Load Y Register Absolute X, 4+ Cycles)
- `STA_ZP`   - `0x85` (Store Accumulator Zero Page, 3 Cycles)
- `STA_ZPX`  - `0x95` (Store Accumulator Zero Page X, 4 Cycles)
- `STA_ABS`  - `0x8D` (Store Accumulator Absolute, 4 Cycles)
- `STA_ABSX` - `0x9D` (Store Accumulator Absolute X, 5 Cycles)
- `STA_ABSY` - `0x99` (Store Accumulator Absolute Y, 5 Cycles)
- `STA_INDX` - `0x81` (Store Accumulator Indirect X, 6 Cycles)
- `STA_INDY` - `0x91` (Store Accumulator Indirect Y, 6 Cycles)
- `STX_ZP`   - `0x86` (Store X Register Zero Page, 3 Cycles)
- `STX_ZPY`  - `0x96` (Store X Register Zero Page Y, 4 Cycles)
- `STX_ABS`  - `0x8E` (Store X Register Absolute, 4 Cycles)
- `STY_ZP`   - `0x84` (Store Y Register Zero Page, 3 Cycles)
- `STY_ZPX`  - `0x94` (Store Y Register Zero Page X, 4 Cycles)
- `STY_ABS`  - `0x8C` (Store Y Register Absolute, 4 Cycles)
- `JSR`  - `0x20` (Jump to Subroutine Absolute, 6 Cycles)
- `RTS`  - `0x60` (Return from Subroutine, 6 Cycles)
- `JMP_ABS` - `0x4C` (Jump to Absolute, 3 Cycles)
- `JMP_IND` - `0x6C` (Jump to Indirect, 5 Cycles)
- `TSX` - `0xBA` (Transfer Stack Pointer to X, 2 Cycles)
- `TXS` - `0x9A` (Transfer X to Stack Pointer, 2 Cycles)
- `PHA` - `0x48` (Push Accumulator, 3 Cycles)
- `PHP` - `0x08` (Push Processor Status, 3 Cycles)
- `PLA` - `0x68` (Pull Accumulator, 4 Cycles (Sets Z and N))
- `PLP` - `0x28` (Pull Processor Status, 4 Cycles (Sets all flags))
- `AND_IM` - `0x29` (AND Immediate, 2 Cycles)
- `AND_ZP` - `0x25` (AND Zero Page, 3 Cycles)
- `AND_ZPX` - `0x35` (AND Zero Page X, 4 Cycles)
- `AND_ABS` - `0x2D` (AND Absolute, 4 Cycles)
- `AND_ABSX` - `0x3D` (AND Absolute X, 4+ Cycles)
- `AND_ABSY` - `0x39` (AND Absolute Y, 4+ Cycles)
- `AND_INDX` - `0x21` (AND Indirect X, 6 Cycles)
- `AND_INDY` - `0x31` (AND Indirect Y, 5+ Cycles)

- `ORA_IM` - `0x09` (ORA Immediate, 2 Cycles)
- `ORA_ZP` - `0x05` (ORA Zero Page, 3 Cycles)
- `ORA_ZPX` - `0x15` (ORA Zero Page X, 4 Cycles)
- `ORA_ABS` - `0x0D` (ORA Absolute, 4 Cycles)
- `ORA_ABSX` - `0x1D` (ORA Absolute X, 4+ Cycles)
- `ORA_ABSY` - `0x19` (ORA Absolute Y, 4+ Cycles)
- `ORA_INDX` - `0x01` (ORA Indirect X, 6 Cycles)
- `ORA_INDY` - `0x11` (ORA Indirect Y, 5+ Cycles)

- `EOR_IM` - `0x49` (EOR Immediate, 2 Cycles)
- `EOR_ZP` - `0x45` (EOR Zero Page, 3 Cycles)
- `EOR_ZPX` - `0x55` (EOR Zero Page X, 4 Cycles)
- `EOR_ABS` - `0x4D` (EOR Absolute, 4 Cycles)
- `EOR_ABSX` - `0x5D` (EOR Absolute X, 4+ Cycles)
- `EOR_ABSY` - `0x59` (EOR Absolute Y, 4+ Cycles)
- `EOR_INDX` - `0x41` (EOR Indirect X, 6 Cycles)
- `EOR_INDY` - `0x51` (EOR Indirect Y, 5+ Cycles)

- `BIT_ZP` - `0x24` (BIT Zero Page, 3 Cycles)
- `BIT_ABS` - `0x2C` (BIT Absolute, 4 Cycles)

- `TAX` - `0xAA` (Transfer Accumulator to X, 2 Cycles)
- `TAY` - `0xA8` (Transfer Accumulator to Y, 2 Cycles)
- `TXA` - `0x8A` (Transfer X to Accumulator, 2 Cycles)
- `TYA` - `0x98` (Transfer Y to Accumulator, 2 Cycles)

- `INX` - `0xE8` (Increment X Register, 2 Cycles)
- `INY` - `0xC8` (Increment Y Register, 2 Cycles)
- `DEX` - `0xCA` (Decrement X Register, 2 Cycles)
- `DEY` - `0x88` (Decrement Y Register, 2 Cycles)


## Contributing
Contributions are welcome! Please fork the repository and submit a pull request.

## License
This project is licensed under the MIT License.
