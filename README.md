# Vimes - Virtual Machines Experimentation Sandbox

**Vimes** is a collection of virtual machines (VMs) and related resources for studying their performance, ease of implementation, and ease of use. This sandbox includes a variety of VMs with different architectures, implementations, and assemblers, as well as benchmarks and utilities to help measure and compare their characteristics.

## Architectures

### V1

Inspired mostly by [p-code machine](https://en.wikipedia.org/wiki/P-code_machine) from Niklaus Wirth's book "Algorithms + Data Structures = Programs" (1976).

**Differences**:

- access restricted only to the local variables (level==0)
- two conditional jump instructions (JZ and JNZ)
- new operations:
  - MOD - modulo operation (a b -- c)
  - HALT - stop execution
- new instruction (EXT) that extends VM with new functionality
  - AST - assert equal (a b --)
  - ARG - get value of command-line program argument (a -- b)

