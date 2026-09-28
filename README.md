# C Compiler in C

A modular C-to-x86-64-assembly compiler written from scratch in C.

## 🦖 The Logo

```
  CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
 C::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CC
  CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

           "C" for C Compiler - Handcrafted, Not AI-generated
```

## Overview

This project implements a complete C compiler that:

1. **Lexical Analysis** - Converts source code into a linked list of tokens
   - Keywords: `if`, `else`, `int`, `return`, `void`, `while`, `for`
   - Operators: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `!`
   - Literals: integers, floating-point, strings
   - Punctuation: `;`, `,`, `(`, `)`, `{`, `}`, `[`, `]`

2. **Parsing** - Recursive descent parser building an AST with proper operator precedence
   - Expression: `=` assignment
   - Comparison: `<`, `>`, `<=`, `>=`, `==`, `!=`
   - Term: `+`, `-`
   - Factor: `*`, `/`, `%`
   - Unary: `-`, `!`
   - Primary: numbers, identifiers, parentheses, function calls

3. **Code Generation** - Produces x86-64 assembly language
   - Uses rbp as frame pointer
   - Stack-based local variable allocation
   - Proper function prologue/epilogue
   - Integer arithmetic and comparison

## Architecture

```
src/
├── main.c          - Entry point: reads source, orchestrates lexer/parser/codegen
├── lexer.c         - Lexical analyzer: source -> token linked list
├── parser.c        - Recursive descent parser: tokens -> AST
└── codegen.c       - x86-64 code generator: AST -> assembly

include/
├── lexer.h         - Token type definitions and lexer interface
├── parser.h        - AST node definitions and parser interface
└── codegen.h       - Code generation interface
```

## Building

```bash
make              # compile the compiler
make run          # compile and run test
make clean        # remove build artifacts
```

## Usage

```bash
./c_compiler source.c       # compile source.c to output.s
./c_compiler test_program.c # compile test program
```

The generated `output.s` can be assembled and linked:

```bash
nasm -f elf64 output.s -o output.o
ld output.o -o program
./program
```

## Example

```c
#include <stdio.h>

int main(void) {
    int a = 5;
    int b = 3;
    int c = a + b;
    printf("Hello: %d\n", c);
    return 0;
}
```

Compiles to x86-64 assembly that outputs:
```
Hello: 8
```

## License

MIT License - see LICENSE.md for details.

## Implementation Philosophy

- **Modular design**: Each phase (lexer, parser, codegen) is isolated in its own file
- **Memory safety**: Every allocation has a corresponding free; NULL checks throughout
- **Error handling**: Graceful recovery with descriptive error messages
- **Simple data structures**: Linked lists for tokens, tree structure for AST - appropriate for compiler phase
- **Human-readable output**: Assembly is readable and educational, not optimized