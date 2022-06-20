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

vDSO and interrupt based syscalls difference:
https://stackoverflow.com/questions/12806584/what-is-better-int-0x80-or-syscall-in-32-bit-code-on-linux

vDSO (virtual dynamic shared object):
https://en.wikipedia.org/wiki/VDSO

Linking assembly (ASM) sources:
https://stackoverflow.com/questions/15419201/system-write-integer

## Output
```
> ./load_and_execute 
-----------------------------------------------------------
Generating simple_c_program.c:

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])        {
        char str[] = "Hello World!\n";
        write(1, str, 13);
        return 0;
}
-----------------------------------------------------------

simple_c_program.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   48 83 ec 30             sub    rsp,0x30
   8:   89 7d dc                mov    DWORD PTR [rbp-0x24],edi
   b:   48 89 75 d0             mov    QWORD PTR [rbp-0x30],rsi
   f:   64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
  16:   00 00 
  18:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  1c:   31 c0                   xor    eax,eax
  1e:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f57206f6c6c6548
  25:   20 57 6f 
  28:   48 89 45 ea             mov    QWORD PTR [rbp-0x16],rax
  2c:   c7 45 f2 72 6c 64 21    mov    DWORD PTR [rbp-0xe],0x21646c72
  33:   66 c7 45 f6 0a 00       mov    WORD PTR [rbp-0xa],0xa
  39:   48 8d 45 ea             lea    rax,[rbp-0x16]
  3d:   ba 0d 00 00 00          mov    edx,0xd
  42:   48 89 c6                mov    rsi,rax
  45:   bf 01 00 00 00          mov    edi,0x1
  4a:   e8 00 00 00 00          call   4f <main+0x4f>
  4f:   b8 00 00 00 00          mov    eax,0x0
  54:   48 8b 4d f8             mov    rcx,QWORD PTR [rbp-0x8]
  58:   64 48 33 0c 25 28 00    xor    rcx,QWORD PTR fs:0x28
  5f:   00 00 
  61:   74 05                   je     68 <main+0x68>
  63:   e8 00 00 00 00          call   68 <main+0x68>
  68:   c9                      leave  
  69:   c3                      ret    
-----------------------------------------------------------
Hex dump of "Hello World!\n":
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A
-----------------------------------------------------------
Hex dump of simple_c_program.bin:
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 55 48 89 E5 48 83 EC 30 89 7D DC 48 89 75 D0 64
  10: 48 8B 04 25 28 00 00 00 48 89 45 F8 31 C0 48 B8
  20: 48 65 6C 6C 6F 20 57 6F 48 89 45 EA C7 45 F2 72
  30: 6C 64 21 66 C7 45 F6 0A 00 48 8D 45 EA BA 0D 00
  40: 00 00 48 89 C6 BF 01 00 00 00 E8 00 00 00 00 B8
  50: 00 00 00 00 48 8B 4D F8 64 48 33 0C 25 28 00 00
  60: 00 74 05 E8 00 00 00 00 C9 C3
-----------------------------------------------------------
Address lenght: 64 bits
File loaded in memory at 0x00000000BF2C9000
Executing the program...
Execution correctly returned.
-----------------------------------------------------------
```
## Known issues
When doing
```
> ./simple_c_program 
Hello World!
```
we can see the output "Hello World!", but if calling simple_c_program from load_and_executewe can't because of modern kernels use vDSO.
