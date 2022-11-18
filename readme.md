# Run the following commands:

```
make assembler
./assemble <input file> <output file> [-symbols | --verbose]
```

Examples:

To run the assembler in quiet mode:

```
./assemble example_input.txt example_output.txt
```

To run the assembler with console logging enabled:

```
./assemble example_input.txt example_output.txt --verbose
```

To see symbol/label addresses run:

```
./assemble example_input.txt example_output.txt -symbols
```

Verbose mode can be used with either symbols output or regular output and is useful for debugging.

# Supported instruction set:

- lw        rt, offset(rs)
- sw        rt, offset(rs)
- lui       rt, imm16
- add       rd, rs, rt
- addi      rt, rs, imm16
- addu      rd, rs, rt
- addiu     rt, rs, imm16
- and       rd, rs, rt
- andi      rt, rs, imm16
- mul       rd, rs, rt
- mult      rs, rt
- nop   
- nor       rd, rs, rt
- sll       rd, rt, sa
- slt       rd, rs, rt
- slti      rt, rs, imm16
- sra       rd, rt, sa
- srav      rd, rt, rs
- sub       rd, rs, rt
- beq       rs, rt, offset
- blez      rs, offset
- bgtz      rs, offset
- bne       rs, rt, offset
- j         target
- syscall   
- move      rd, rs
- blt       rs, rt, offset
- la        rt, label
- li        rt, imm16
- lw        rt, label