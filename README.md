# CMU-15-213-Intro-to-Computer-Systems
Notes and labs for the course 15-213 Introduction to Computer Systems at CMU

## Integer representation
* Data types
    * char, short, int, long, float, double, pointer
    * Word size equals length of pointer datatype
* Bit-level operations
* Signed / unsigned conversion
* Byte ordering
    * Big Endian: Sun, PPC Mac, Internet
    * Little Endian: x86, ARM  
    <img src="Note_Images/endian.png" width="60%">


## IEEE 754
* Numeric form
    $(-1)^sM2^E$
* Encoding
    * s: sign bit
    * exp: E
    * frac: M  
    <img src="Note_Images/ieee754.png" width="60%">
* Three kinds of values
    * Denormalized: exp = 0
    * Normalized: 0 < exp < 11..11
    * Special: exp = 11...11 (e.g. inf & NaN)  
    <img src="Note_Images/ieee754range.png" width="80%">
* Roundings

## x86-64
* History
    * 8086 -> 386 -> Pentium 4E -> Core 2 -> Core i7
    * IA32 -> x86-64
    * CISC architecture
    * Case study: Core-i7 6700K Skylake
        * Shared L3 cache (LLC)
        * Hyper-Threading: Shared caches, buses & ALUs
        <img src="Note_Images/corei7.png" width="80%">
* Registers  
    * Can reference low-order 4 bytes (backwards compatibility)
    * `%rsp` is stack top, others are general-purpose  
    <img src="Note_Images/registers.png" width="50%">
    * `%rip` is instruction pointer (not listed)  
    * A register for implicitly-set condition codes

## Machine-level Programming
* Addressing modes
    * Normal: `(R)` -> `Mem[Reg[R]]`
    * Displacement: `D(R)` -> `Mem[Reg[R] + D]`
    * Complete: `D(Rb,Ri,S)` -> `Mem[Reg[Rb] + S*Reg[Ri] + D]`
        * `(Rb,Ri)` -> `Mem[Reg[Rb] + Reg[Ri]]`
        * `D(Rb,Ri)` -> `Mem[Reg[Rb] + Reg[Ri] + D]`
        * `(Rb,Ri,S)` -> `Mem[Reg[Rb] + S*Reg[Ri]]`
* Some instructions
    * `movq Src, Dst`
        * Cannot do memory-memory transfer with a single instruction
        * Intel docs use `mov Dst, Src`
    * `leaq Src, Dst`
        * Src is address mode expression, set Dst to address denoted by expression
        * Similar to `p = &x[i]`
        * Used for arithmetics for form like `x + k * y`
        * Does not change condition codes
    * `addq/subq Src, Dst`
    * `imulq Src, Dst`
    * `salq/sarq/shrq Src, Dst`
    * `xorq/andq/orq Src, Dst`
    * `pushq src`
    * `popq dest`
    * `incr dest`
* Compiler, Assembler, Linker & Loader  
    * Compiler
        * Translates C files (.c) into assembly files (.s)
    * Assembler
        * Translates assembly files (.s) into object files (.o)
        * Missing linkage between compilation units
    * Linker
        * Resolve references between object files
        * Combine with static libraries (malloc, printf, etc)
    * Dynamic linked libraries
        * Linking occurs at runtime
        * Does not take too much disk space  
    <img src="Note_Images/compilation.png" width="70%">
* Controls
    * Jumping
        * `jmp`, `je`, `jne`, `js` ...
        * However, branches are very disruptive to instruction flow through pipelines
    * Conditional moves: `cmovle`
        * Do not require control transfer
        * Use `-fno-if-conversion` flag to supress implicit conversion
    * Switch statements
        * Jump table structure  
        <img src="Note_Images/jump_table.png" width="80%">
        * Make use of "fall through"
* Procedures
    * Passing control
        * Procedure call: `call label`
            * Push return address into stack
            * Jump to label
        * Procedure return: `ret`
            * Pop return address from stack
            * Jump to this address
        * Return address: Address of next instruction after the call statement
    * Passing data
        * First 6 arguments: `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`
        * Other arguments passed using stack
        * Return value: `%rax`
        * IA-32 pass all arguments in stack
        * Concept of stack frames:
            * Marked by `%rbp` (optional) and `%rsp`
            * No addition mechanism for recursion is needed  
            <img src="Note_Images/stack_frame.png" width="80%">
        * Register saving conditions
            * Caller saved
                * `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`, `%rax`, `%r10`, `%r11`
            * Callee saved
                * `%rbx`, `%r12`, `%r13`, `%r14`, `%rbp`
                * `%rsp` is also a special form of callee-saved
    * Memory management
    * ABI: Application Binary Interface
* Data
    * Arrays  
        * 1D arrays  
            <img src="Note_Images/arrays.png" width="80%">
        * Nested 2D arrays: `int A[R][C]`
            <img src="Note_Images/array_nested.png" width="80%">
        * Multi-level 2D arrays:  
            <img src="Note_Images/array_multilevel.png" width="80%">
    * Structs
        * Represented as block of memory
            <img src="Note_Images/struct.png" width="80%">
        * Fields are ordered according to declaration
        * Alignment:  
            * Within struct: Each element has alignment requirement K, where K is the size of this element
            <img src="Note_Images/alignment.png" width="80%">
            * Overall: Each struct has alignment requirement K, where K is the largest alignment of any element in struct
            <img src="Note_Images/alignment_overall.png" width="80%">
            * To save space, put large data types first
    * Float operations
        * Arguments passed in `%xmm0`, `%xmm1`, ...
        * Result returned in `%xmm0`
        * Different mov instructions are used to move floats