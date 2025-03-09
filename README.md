## Project Overview

The goal of this project is to build a 32-bit RISC-V assembler (similar to the Venus assembler part) that converts RISC-V assembly code into machine code. The assembler reads an assembly program from an `input.asm` file and produces an `output.mc` file with the corresponding machine code.

### Key Features

- **Instruction Support:**

  - **R Format:** `add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem`
  - **I Format:** `addi, andi, ori, lb, ld, lh, lw, jalr`
  - **S Format:** `sb, sw, sd, sh`
  - **SB Format:** `beq, bne, bge, blt`
  - **U Format:** `auipc, lui`
  - **UJ Format:** `jal`
  - _Pseudo instructions support is skipped._
  - Floating point operations are skipped.

- **Assembler Directives:**  
  Supports directives such as `.text`, `.data`, `.byte`, `.half`, `.word`, `.dword`, and `.asciz`.

- **Memory Segments:**

  - Code Segment starts at `0x00000000`
  - Data Segment starts at `0x10000000`
  - Heap Segment at `0x10008000`
  - Stack Segment at `0x7FFFFFFC`

- **Output Format:**  
  Each line in the machine code file (`output.mc`) has the following format:

  `<address> <machine_code> , <assembly_instruction> # <binary_breakdown>`

  For example:

  0x0 0x003100B3 , add x1,x2,x3 # 0110011-000-0000000-00001-00010-00011-NULL

---

## File Structure

```
RISC-V_assembler/
│
├── src/
│   ├── main.cpp       // Main driver code
│   ├── parser.h       // Header for parsing routines
│   ├── parser.cpp     // Implementation for parsing .asm files
│   ├── encoder.h      // Header for encoding routines (machine code conversion)
│   └── encoder.cpp    // Implementation for encoding instructions
│
└── test/
    └── input.asm      // Sample input assembly file
```

---

## Build Instructions

1. **Navigate** to the project directory:

   ```
   cd RISC-V_assembler
   ```

2. Ensure you've **g++/gcc compiler** installed in your computer. **Compile** the project using g++ with C++20/17/14/11:

   ```
   g++ -o assembler src/main.cpp src/parser.cpp src/encoder.cpp
   ```

3. **Run the Assembler:**

   - On Windows:
     ```
     .\assembler.exe test\input.asm
     ```
   - On Unix-like systems:
     ```
     ./assembler test/input.asm
     ```

   This will generate an `output.mc` file in your project root containing the machine code output.

---

## Sample Input and Output

### Sample Input (`test/input.asm`):

```
.data
word1: .word 5
word2: .word 10, 15, 20
.word 10
half1: .half 3
string1: .asciiz "Hello"
.byte 255

.text
main:
    add x1, x2, x3
    andi x5, x6, 10
    beq x1, x2, target
    jal x0, target
    jalr x0, x3, target  # This should replace `target` with its absolute address
    addi x1, x0, 3
target:
    addi x4, x5, 10
#target is 4 bytes a head of jal and 3 instructions ahead of beq
```

### Expected Output (`output.mc`):

```
0x0 0x3211443 , add,x1,x2,x3 # 0000000-000-11000-10000-00001-0110011
0x4 0x10711699 , andi,x5,x6,10 # 0000000-010-10001-10111-00101-0010011
0x8 0x2131043 , beq,x1,x2,16 # 0000000-000-10000-01000-01000-1100011
0xc 0x12583023 , jal,x0,12 # 0000000-011-00000-00000-00000-1101111
0x10 0x25272423 , jalr,x0,x3,24 # 0000000-110-00000-11010-00000-1100111
0x14 0x3145875 , addi,x1,x0,3 # 0000000-000-11000-00000-00001-0010011
0x18 0x10650131 , addi,x4,x5,10 # 0000000-010-10001-01000-00100-0010011
0x1c 0xFFFFFFFF , # Program End


# Data Segment
0x1000001c 0x255
0x1000001a 0x111
0x1000000c 0x20
0x10000008 0x15
0x1000001b 0x0
0x10000004 0x10
0x10000017 0x101
0x10000000 0x5
0x10000016 0x72
0x10000010 0x10
0x10000014 0x3
0x10000018 0x108
0x10000019 0x108
```
