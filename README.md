# Vimes - Virtual Machines Experimentation Sandbox

**Vimes** is a collection of virtual machines (VMs) and related resources for studying their performance, ease of implementation, and ease of use. This sandbox includes a variety of VMs with different architectures, dispatch techniques, implementations, and assemblers, as well as benchmarks and utilities to help measure and compare their characteristics.

**Warning**: this is experimental / pre-alpha code.

# Architectures

## V1

Stack machine inspired mostly by [p-code machine](https://en.wikipedia.org/wiki/P-code_machine) from Niklaus Wirth's book "Algorithms + Data Structures = Programs" (1976).

**Differences**:

- access restricted only to the local variables (level==0)
- two conditional jump instructions (JZ and JNZ - for easier code generation)
- new operations:
  - MOD - modulo operation (replaces even-odd check from Wirth's VM)
  - HLT - stop execution
- new instruction (INC) - example of a superinstruction
- new instruction (EXT) that extends VM with new functionality
  - AST - assert equal (for easier testing)
  - ARG - get value of command-line program argument (for easier parametrization)
  - ARR, GET, SET - array operations

# Dispatch techniques

1. ✅ Switch-based
   - poor performance
   - simplest code

2. ✅ Replicated switch
   - performance just slightly better than switch-based
   - largest binary
   - requires code replication

3. ✅ Direct threading
   - best performance 🏆
   - requires code translation into native addresses
   - requires support for "labels as values" in the compiler

4. ✅ Indirect threading
   - good performance
   - requires support for "labels as values" in the compiler

5.  ❌Call-based
   - NOT IMPLEMENTED YET

# Details

## V1 architecture

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

- **HLT** - halt execution (--)
- **RET** - return from procedure call (--)
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
- **ARR** - allocate array of *a* items on the return stack and return it's reference (a -- b)
- **GET** - get value from memory cell *a* (a -- b)
- **SET** - set memory cell *b* to value *a* (a b --)



