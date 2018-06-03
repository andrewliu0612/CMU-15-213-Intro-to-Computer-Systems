# CMU-15-213-Intro-to-Computer-Systems
Notes and labs for the course 15-213 Introduction to Computer Systems at CMU

# Integer representation
* Data types
    * char, short, int, long, float, double, pointer
    * Word size equals length of pointer datatype
* Bit-level operations
* Signed / unsigned conversion
* Byte ordering
    * Big Endian: Sun, PPC Mac, Internet
    * Little Endian: x86, ARM  
    <img src="Note_Images/endian.png" width="50%">


# IEEE 754
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

# x86-64
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

# Machine-level Programming
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

# Code optimization
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
* Cache memories
    * Concept of locality  
        <img src="Note_Images/locality.png" width=60%>  
    * General organization  
        <img src="Note_Images/address.png" width=20%>
        <img src="Note_Images/organization.png" width=50%>  
        1. Direct mapped cache has (E / associativity = 1)  
            <img src="Note_Images/direct_mapped_cache.png" width=50%>  
        2. E-way set associative cache (Here E / associativity = 2)  
            <img src="Note_Images/e_way_associative_cache.png" width=50%>   
    * Metrics
        1. Miss rate
        2. Hit time
        3. Miss penalty
    * Write cache-friendly code
        1. Make the common cases go first
        2. Minimize the misses in inner loops
        3. Try to maximize spatial locality by reading objects sequentially with stride 1
        4. Try to maximize temporal locality by using an object as often as possible once it's read from memory
    * Example of matrix multiplication
        * In which order to arrange the loops? Do miss rate analysis!
        * It turns out: kij/ikj > ijk/jik > jki/kji
        * Use blocking: multiplying by sub-matrices

# Linking
* Why linkers?
    1. Modularity
    2. Efficiency (separate complilation)
* Two kind of linking
    1. Static linking
    2. Dynamic linking
* What does linker do?
    1. Symbol resolution
        * Functions, `global` vars, `static` vars
        * Definitions are stored in __symbol table__, an array of entries (name, size, location)
        * Three kind of symbols:
            1. Global symbols: non-static functions and non-static vars
            2. External symbols: defined in other modules
            3. Local symbols: static functions and static vars
            * Note: Do not confuse local symbols with local variables. Local variables are allocated in stack at runtime, and have nothing to do with linker. 
        * Symbol resolution
            * Symbols are strong or weak:
                1. Strong: functions and initialized globals
                2. Weak: uninitialized globals
            * Multiple strong symbols are not allowed
            * Choose the strong symbol over weak symbols
            * If there are multiple weak symbols, choose arbitrary one 
                * May cause undefined behavior over different compilers
                * Fix: use `static` and explicit `extern` 
    2. Relocation
        * Merge text and data segment
        * Relative location -> absolute location
        * Updates symbol table
            * Relocation entries are used to aid symbol resolving:  
            `a: R_X86_64_32 array`
* Three kinds of object files
    1. Relocatable object file (.o file)
    2. Executable object file (a.out file)
    3. Shared object file (.so file or .dll file)
* ELF format (Executable and Linkable Format)  
    * All 3 object files use ELF format  
    <img src="Note_Images/elf.png" width=65%>
    <img src="Note_Images/elf_2.png" width=35%>  
* Static libraries (.a archive files)
    * Concatenate related relocatable object files into a single file with an index (called an archive)
    * During linking, only referenced .o files are linked
    * Command line order matters!
        * During scan, keep a list of currently unresolved references
        * If any entries in the unresolved list at end of scan, then error
        * Fix: put libraries at the end of command line
    * Commonly used libraries:
        * `libc.a` (the C standard library)
        * `limb.a` (the C math library)
    * Disadvantages
        * Duplication in storage
        * Bug fixes require relink
        * Fix: shared libraries
* Shared libraries
    * Dynamic linking can happen at:
        1. Load time
            * Handled by the dynamic linker
            * `libc.so` usually dynamically linked
        2. Run time
            * `dlopen()` interface in linux
* Library interpositioning
    * Can happen at:
        1. Compile time
        2. Link time
        3. Load/run time
    * Can be used for:
        1. Detecting memory leaks
        2. Generating address traces
    
# Exception Control Flows (ECF)
* ECFs exists in all levels:
    1. Exceptions (low level)
        * Processor responses to external events
        * Exception tables
    2. Context switch
    3. Signals
    4. Nonlocal jumps
* Exceptions (equivalent to user-kernel transition)  
    <img src="Note_Images/exceptions.png" width=50%>  
    1. Asynchronous (Interrupts)
        * Indicated by INT pin
        * Control flow returns to next instruction
    2. Synchronous
        1. Traps
            * Intentional (syscall, breakpoints)
            * Control flow returns to next instruction
        2. Faults
            * Unintentional but possibly recoverable
            * Control flow returns to current instruction or aborts
        3. Aborts
            * Unintentional and unrecoverable
* Context switches

# Processes
* From a programmer's perspective, a process can be:
    1. Running: Executing or will be scheduled
    2. Stopped: Suspended and will not be scheduled until further notice
    3. Terminated: Stopped permanently (zombie)
        * Process terminates when: 
            1. `SIGTERM` received
            2. Return from `main()`
            3. Called `exit()`
* Creating process: `fork()`
    * `fork()` called once but returns twice
    * `exit()` and `execve()` called once but possibly never returns
    * Control flow can be modelled with process graphs via toposort:  
        <img src="Note_Images/process_graph1.png" width=30%>
        <img src="Note_Images/process_graph2.png" width=25%>
* Reaping child processes: `wait()`
    * Terminated processes become zombies, because its parent may use its exit status or OS tables
    * `wait()` and `waitpid()` reap zombie child processes
    * If parent don't reap:
        1. If parent doesn't terminate: Never diminishes (a kind of memory leak)
        2. If parent does terminate: Reaped by `init` process (pid == 1)
        * So only need to explicitly reap long-running processes
* Loading and running processes: `execve()`
    * `int execve(char *filename, char *argv[], char *envp[])`
    * Loads and runs in the current process
    * Overwrites code, data and stack
    * Retains PID, open files (e.g. `stdout`), and signal context
    * Called once and never return (except error)
* Process groups
    * Can be get and set by `getpgrp()` and `setpgid()`
    * Kill all process in a group with `kill -n -<pid>` 

# Signals
* Unix shell: An application that runs program on behalf of the user
    * Shell contains a basic loop and a `eval()` function
    * Two cases in `eval()`:
        1. Shell built-in command
        2. Not build-in, use `fork()` and `execve()`
    * __Motivation__: How to reap __both__ foreground and background jobs?
        * Basic loop: Only reaps foreground jobs
        * Fix: Signals
* Signals
    * Akin to exceptions and interrupts
    * Sent from signal (sometimes at the request of another process via `kill`)
    * Identified by an integer
    * Controlled by __per-process__ `pending` and `blocked` bit vectors
        * `pending` vector set and cleared __by kernel__ when signals is sent or received
        * `blocked` vector can be manipulated by `sigprocmask()` function
        * So, signals cannot be queued
    * __Send__: `pending` bit set
    * __Receive__: process reacts to the signal, clears `pending` bit
        1. Ignore
        2. Terminate
        3. Catch (using user-level function called _signal handler_)
    * Kernels checks for `pnb = pending & ~blocked` at beginning of a time-slice
        * If `pnb == 0`:
            * Pass control to next instruction in the process logical flow 
        * Else
            1. Choose lease non-zero bit in `pnb` and forces the process to receive the signal
            2. The receipt of the signal triggers some action by the process (clears `pending` bit)
            3. Repeat for all remaining nonzero bits
            4. Pass control to next instruction in the process logical flow
    * Default action can be one of:
        1. Termination
        2. Stop until restarted by `SIGCONT`
        3. Ignore
    * Override default action by installing `signal handlers`:
        * `handler_t *signal(int signum, handler_t *handler)`
        * `handler` can be one of:
            1. `SIG_IGN`: Ignore
            2. `SIG_DFL`: Revert to default
            3. Function pointer to a user-level signal handler
    * Signal handlers are a form of concurrency  
        <img src="Note_Images/concurrent_flows.png" width=40%>
        <img src="Note_Images/concurrent_flows2.png" width=50%>  
    * Signal handlers can be nested  
        <img src="Note_Images/nested_signal_handlers.png" width=60%>  
        * So we need __blocking__
            1. Implicit blocking: blocks pendings signals of same type
            2. Explicit blocking: `sigprogmask()` with supporting functions of:
                * `sigemptyset()`
                * `sigfillset()`
                * `sigaddset()`
                * `sigdelset()`
    * How to write safe handlers?
        1. Keep handlers as simple as possible
        2. Call only `async-signal-safe` function in handlers
            * `async-signal-safe` functions are _reentrent_ (access only local variables on stack), or cannot be interrupted by another signal handler
            * `printf()`, `malloc()` and `exit()` are __not__ safe
            * `write()` is the only signal-safe output function
        3. Save and restore `errno` on entry and exit
        4. Protect accesses to shared data structures by temporarily blocking __all__ signals in __both__ handler and `main()`
        5. Declare global variables to be `volatile`, to prevent from being optimized into registers
        6. Declare global __flags__ as `volatile sig_atomic_t`
            * Flag: variable only read or written (not `flag++` or `flag+=10`)
            * `volatile sig_atomic_t` are ints on most systems
    * Avoid race conditions
        * Cannot make `any` assumption regarding execution order
        * However, we can control when handlers run by blocking
    * Explicitly waiting for signals: suppose handler sets global variable `pid`:
        * Spin wait: `while(!pid) {}`
            * Wasteful
        * Pause: `while(!pid) pause()`
            * Race condition
        * Sleep: `while(!pid) sleep(1)`
            * Too slow
        * Solution: `sigsuspend`
            * `int sigsuspend(const sigset_t *mask)`
            * Equivalent to __atomic__:
                ```
                sigprocmask(SIG_BLOCK, &mask, &prev);
                pause()
                sigprocmask(SIG_BLOCK, &prev, NULL);    
                ```
    * Portable signal handling
        * Problem: Different versions of unix have different signal handling semantics
        * Solution: Use `sigaction`