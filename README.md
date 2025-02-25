# Vole Machine Simulator

## Overview
The **Vole Machine Simulator** is a C++ program that simulates a simple machine capable of executing assembly-like instructions. It includes components such as a CPU, ALU, Control Unit (CU), Memory, and Registers, allowing users to load a program, execute instructions, and observe the machine's state.

## Features
- **Instruction Execution**: Supports various instructions such as arithmetic operations, logical operations, jumps, and memory operations.
- **Memory Management**: Implements a memory system with 256 memory cells.
- **Register System**: Includes 16 general-purpose registers.
- **Binary, Hexadecimal, and Floating-Point Conversions**: Supports two's complement, binary, and floating-point operations.
- **Interactive CLI**: Users can enter instructions manually or load a program file.
- **Simulation Output**: Displays register and memory states after execution.

## Files
- **`main.cpp`**: Entry point of the program; runs the UI.
- **`VoleMachine.h`**: Header file defining classes such as `Register`, `Memory`, `ALU`, `CU`, `CPU`, and `Machine`.
- **`VoleMachine.cpp`**: Implementation of the machine components.

## Installation & Compilation
### Prerequisites
- A C++ compiler (G++/Clang/MSVC)
- CMake (if using CLion or CMake build system)

### Compilation using g++
```sh
 g++ -o VoleMachineSimulator main.cpp VoleMachine.cpp -std=c++11
```

### Compilation using CMake
Ensure your `CMakeLists.txt` contains:
```cmake
add_executable(VoleMachineSimulator main.cpp VoleMachine.cpp)
```
Then, run:
```sh
cmake .
cmake --build .
```

## Usage
1. **Run the executable:**
   ```sh
   ./VoleMachineSimulator
   ```
2. **Choose an option from the menu:**
   - Load a program file.
   - Enter individual instructions manually.
   - View memory and register states.
   - Exit the simulator.
3. **Program files should follow the format:**
   Each instruction is represented by a 4-character hexadecimal string.

## Example Instruction Set
| Opcode | Description |
|--------|-------------|
| `1XYZ` | Load value at memory address `YZ` into register `X` |
| `2XVV` | Load immediate value `VV` into register `X` |
| `3X00` | Output register `X` to screen |
| `4XY0` | Move contents of register `X` to register `Y` |
| `5XYZ` | Add `Y` and `Z` (2's complement) and store in `X` |
| `6XYZ` | Add `Y` and `Z` (floating point) and store in `X` |
| `7XYZ` | Perform `OR` on `Y` and `Z`, store in `X` |
| `8XYZ` | Perform `AND` on `Y` and `Z`, store in `X` |
| `9XYZ` | Perform `XOR` on `Y` and `Z`, store in `X` |
| `BXY0` | Jump to address `XY` if register `X` == register 0 |
| `C000` | Halt the program |

## Troubleshooting
- Ensure your program file follows the correct instruction format.
- If encountering `undefined reference to WinMain`, ensure `main()` exists and your build system treats it as a console application.
- Run with debugging enabled using `g++ -g` or a debugger (e.g., GDB, CLion debugger).

## Class Diagram

Below is the class diagram illustrating the structure of the Restaurant Management System:
![Class Diagram](classes%20design.jpg)

## Author
- Salsabil Bahaa Eldin
- Doha Fathy
