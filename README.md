# load_in_mem_and_execute
This is a simple program that tryies to:

1) Generate a C source file
2) Compile it in pure instructions (no headers, no exe format)
3) Load it in memory
4) Jump at the memory address and execute it

## How to compile
```gcc load_and_execute.c -o load_and_execute```

## Interesting sources around the web
use mmap() to allocate executable memory:
https://stackoverflow.com/questions/19634587/c-function-pointer-can-i-jump-to-heap-memory-assembler-code

Linux x86 SMMU limitation, use MAP_ANON | MAP_PRIVATE instead of MAP_PRIVATE:
https://stackoverflow.com/questions/18829012/behaviour-of-prot-read-and-prot-write-with-mprotect

## Output
```
> ./load_and_execute 
-----------------------------------------------------------
Generating simple_c_program.c:

#include <stdio.h>

int main(int argc, char *argv[])        {
        int a = 1;
        while(a != 0)
                a++;
        return 0;
}
-----------------------------------------------------------

simple_c_program.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   89 7d ec                mov    DWORD PTR [rbp-0x14],edi
   7:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
   b:   c7 45 fc 01 00 00 00    mov    DWORD PTR [rbp-0x4],0x1
  12:   eb 04                   jmp    18 <main+0x18>
  14:   83 45 fc 01             add    DWORD PTR [rbp-0x4],0x1
  18:   83 7d fc 00             cmp    DWORD PTR [rbp-0x4],0x0
  1c:   75 f6                   jne    14 <main+0x14>
  1e:   b8 00 00 00 00          mov    eax,0x0
  23:   5d                      pop    rbp
  24:   c3                      ret    
-----------------------------------------------------------
Hex dump of simple_c_program.bin:
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 55 48 89 E5 89 7D EC 48 89 75 E0 C7 45 FC 01 00
  10: 00 00 EB 04 83 45 FC 01 83 7D FC 00 75 F6 B8 00
  20: 00 00 00 5D C3
-----------------------------------------------------------
Address lenght: 64 bits
File loaded in memory at 0x000000002B550000
Executing the program...
Execution correctly returned.
-----------------------------------------------------------
```