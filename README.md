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
    <img src="Note_Images/endian.png" width="50%">


## IEEE 754
* Numeric form
    $(-1)^sM2^E$
* Encoding
    * s: sign bit
    * exp: E
    * frac: M  
    <img src="Note_Images/ieee754.png" width="50%">
* Three kinds of values
    * Denormalized: exp = 0
    * Normalized: 0 < exp < 11..11
    * Special: exp = 11...11 (e.g. inf & NaN)  
    <img src="Note_Images/ieee754range.png" width="60%">
* Roundings

## x86-64
* History
    * 8086 -> 386 -> Pentium 4E -> Core 2 -> Core i7
    * IA32 -> x86-64
    * CISC architecture
    * Case study: Core-i7 6700K Skylake
        * Shared L3 cache (LLC)
        * Hyper-Threading: Shared caches, buses & ALUs
        <img src="Note_Images/corei7.png" width="70%">
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
    1. Compiler
        * Translates C files (.c) into assembly files (.s)
    2. Assembler
        * Translates assembly files (.s) into object files (.o)
        * Missing linkage between compilation units
    3. Linker
        * Resolve references between object files
        * Combine with static libraries (malloc, printf, etc)
    4. Dynamic linked libraries
        * Linking occurs at runtime
        * Does not take too much disk space  
    <img src="Note_Images/compilation.png" width="50%">  
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
            * No additional mechanism for recursion is needed  
            <img src="Note_Images/stack_frame.png" width="60%">
        * Register saving conditions
            * Caller saved
                * `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`, `%rax`, `%r10`, `%r11`
            * Callee saved
                * `%rbx`, `%r12`, `%r13`, `%r14`, `%rbp`
                * `%rsp` is also a special form of callee-saved
    * Memory management
    * ABI: Application Binary Interface
* Data
    1. Arrays  
        * 1D arrays  
            <img src="Note_Images/arrays.png" width="60%">  
        * Nested 2D arrays: `int A[R][C]`
            <img src="Note_Images/array_nested.png" width="80%">  
        * Multi-level 2D arrays:  
            <img src="Note_Images/array_multilevel.png" width="80%">  
    2. Structs
        * Represented as block of memory
            <img src="Note_Images/struct.png" width="80%">  
        * Fields are ordered according to declaration
        * Alignment:  
            * Within struct: Each element has alignment requirement K, where K is the size of this element
                <img src="Note_Images/alignment.png" width="80%">  
            * Overall: Each struct has alignment requirement K, where K is the largest alignment of any element in struct
                <img src="Note_Images/alignment_overall.png" width="80%">  
            * To save space, put large data types first
    3. Float operations
        * Arguments passed in `%xmm0`, `%xmm1`, ...
        * Result returned in `%xmm0`
        * Different mov instructions are used to move floats
* Address space
    * Currently using 47-bit addresses (highest address of 0x7fffffffffff)
    * Maximum stack size of 8MB on most machines  
        <img src="Note_Images/memory.png" width="60%">  
* Vulnerablities
    1. Buffer overflow
        * Triggered by functions manipulating strings of arbitrary length
        * `gets`, `strcpy`, `strcat`, `scanf`, `fscanf`, `sscanf`
    2. Return-oriented programming (ROT)
        * Make use of "gadgets" in text segment  
        * Trigger with `ret` instruction  
            <img src="Note_Images/rop.png" width="60%">  
* Protection
    1. Use routines limiting string lengths (user-level)
    2. Randomized stack offsets
    3. Nonexecutable code segments
    4. Stack canaries

## Code optimization
* Optimization by programmer
    1. Code motion: Reduce frequency of computations performed   
        <img src="Note_Images/code_motion.png" width="80%">   
        GCC will do this with -O1  
    2. Reduction in strength: Reduce costly operation with simpler one  
        <img src="Note_Images/reduction_in_strength.png" width="80%">  
        Here, int mul requires 3 clock cycles, int add requires 1 clock cycle 
    3. Share common subexpressions  
        <img src="Note_Images/share_common_subexpressions.png" width="80%">  
* Optimization blockers
    1. Procedures: Seen as a "black box"
        * Procedures may have side effects
        * May not return same result with same argument
        * Fix: Use inline functions (GCC with -O1 within single file)
    2. Memory aliasing: Two memory references specify single location
        * The following code does memory load and store every time, because compiler assume possibility of memory aliasing:  
            <img src="Note_Images/memory_aliasing.png" width=80%>  
        * Load and store take multiple clock cycles
        * Easily caused by direct access to storage structures
        * Fix: Define local variable to tell compiler not to check for aliasing
            <img src="Note_Images/aliasing_fix.png" width=60%>  
        * Get in habit of introducing local variables accumulating within loops
* Optimization (by programmer) limitations
    1. Most performed within procedures. Newer versions of GCC do interprocedual optimization, but not between codes in different files
    2. Based on static information
    3. Conservative: Must not change program behavior
* Instruction-level parallelism
    * Superscalar processor: Issue and execute multuple instructions per cycle, and instructions are scheduled dynamically
    * Some instruction have >1 clock cycle latency, but can be pipelined:  
        <img src="Note_Images/pipeline.png" width=70%>  
    * Unrolling
        * Break sequential dependency to break through latency bound (to approach throughput bound)  
            <img src="Note_Images/unrolling.png" width=30%>  
            ```
            for(int i = 0; i < limit; ++i)
                x = x + d[i];
            ```
            can be optimized to:
            ```
            for(int i = 0; i < limit; i += 2)
                x = (x + d[i]) + d[i + 1];
            ```
            but to break sequential dependency:
            ```
            for(int i = 0; i < limit; i += 2)
                x = x + (d[i] + d[i + 1]);
            ``` 
        * adding separate accumulators
    * Branch prediction
        * Backward branches are often loops, predict taken
        * Forward branches are often if, predict not taken
        * Average better than 95% accuracy

# Memory
* Storage technologies
    1. RAMs
        * Volatile: SRAM & DRAM (caches & main memories)
        * Nonvolatile: ROM, PROM, EPROM, EEPROM (firmware, ssd & disk caches)
    2. Rotating disks
    3. SSDs
        * Page can be written only after its block has been erased
* Locality
    * Temporal locality
    * Spatial locality
* Hierarchy  
    <img src="Note_Images/hierarchy.png" width=70%>  
* Caches
    * Each level in hierarchy serves as cache for the level below
    * Types of cache misses
        1. Cold miss: "Warm up" cache
        2. Capacity miss: Working set larger than cache size
        3. Conflict miss: Limited by positioning restrictions imposed by hardware
    * Examples of cache  
        <img src="Note_Images/cache_examples.png" width=70%>  


