

# Kanban

## Active

- first benchmarks
- c: direct threading

## Next

- c: option to disable ic limiter (+15% boost)
- c: option to disable ic

## To do

- c: VM core into separate file
- c: VM configuration to struct 
- c: error handling 
- asm: inline comments (for forth-like stack notation)
- rethink assert / check
- all tests into files + assertions
- c: mmap code (don'ht allocate mem)

## Done

- bench: .md reporting
- bench: simple tsv reporting
- c: gcc compilation
- c: implementation - replicated switch
- vm: INC VAR VAL (loops optimization)
- c: fixed INC
- rename HALT -> HLT
- c: ic uses 64 bits
- c: EXT ARG via environ (P1, P2, ... P7)
- asm: ';' as comment (better handling in text editors)
- c: CLI parameters (path, mem_size)
- c: passing code to VM
- asm: generating binary pcode
- c: dt calculation
- c: comments for the registers
- c: memory allocation from the heap
- input/output/system/os to separate opcode (ARG, DOT, AST, EMIT)
- ARG as OPR
- vm / languages - directory structure
- print VMSTATS after HALT/ERROR (ip, sp, rp, fp, ic, tos, time-t0)
- autogenerate opcodes.h
- switch-based VM in C
- fix Fibonacci Numbers
- finish Ackermann's Function
- passing parameters into the program (easier testing)
- finish Random Number Generator

# Idea

1. Python VM
2. Python ASM
3. ASM tests
4. ASM benchmarks
5. Code exporter
6. C VM (variants)
7. More ASM benchmarks
8. C code generation (variants)