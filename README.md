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
  - HLT - stop execution
- new instruction (EXT) that extends VM with new functionality
  - AST - assert equal (a b --)
  - ARG - get value of command-line program argument (a -- b)



#### Instructions

- **LIT a** - load constant *a*
- **LOD a** - load variable *a*
- **STO a** - store variable *a*
- **CAL a** - call procedure *a*
- **JMP a** - jump to *a* (relative)
- **JZ a** - jump to *a* (relative) if top-of-stack is zero
- **JNZ a** - jump to *a* (relative) if top-of-stack is not zero
- **INT a** - grow ret-stack by *a*
- **INC a b** - increase variable *a* by *b* (superinstruction)
- **OPR a** - perform operation *a* (details below)
- **EXT a** - perform extension operation *a* (details below)

##### Operations

- **HLT** - halt execution
- **RET** - return from procedure call
- **ADD** - add two values on the stack (a b -- c)
- **SUB** - subtract two values on the stack (a b -- c)
- **MUL** - multiply two values on the stack (a b -- c)
- **DIV** - divide two values on the stack (a b -- c)
- **MOD** - modulo operation on two values on the stack (a b -- c)
- **LT** - a is less then b (a b -- c)
- **GT** - a is greater then b (a b -- c) 
- **EQ** - a equals b (a b -- c)
- **NE** - a not equals b (a b -- c)
- **LE** - a is less than b (a b -- c)
- **GE** - a is greater than b (a b -- c)

###### Extension

- **AST** - assert equal (a b --)
- **DOT** - print top-of-stack (a -- a)
- **ARG** - load program parameter (a -- b)



