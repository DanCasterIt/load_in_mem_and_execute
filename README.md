# load_in_mem_and_execute
This is a simple program that tryies to:

1) Generate a C source file
2) Compile it in pure instructions (no headers, no exe format)
3) Load it in memory
4) Jump at the memory address and execute it

## How to compile
```gcc load_and_execute.c -o load_and_execute```

## Output
```
> ./load_and_execute
-----------------------------------------------------------
Loading assembly_syscall.c:

#include <stdio.h>
#include <asm/unistd.h>

int main()  {
    int fd = 1;
    size_t size = 13;
    char str[] = "Hello world!\n";
    const void *buf = (const void *)str;
    asm volatile(
        "syscall"
        //                  EDI      RSI       RDX
        :: "a"(__NR_write), "D"(fd), "S"(buf), "d"(size)
        : "rcx", "r11", "memory"
    );
    return 0;
}

-----------------------------------------------------------

assembly_syscall.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   48 83 ec 30             sub    rsp,0x30
   8:   64 48 8b 04 25 28 00    mov    rax,QWORD PTR fs:0x28
   f:   00 00 
  11:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  15:   31 c0                   xor    eax,eax
  17:   c7 45 d4 01 00 00 00    mov    DWORD PTR [rbp-0x2c],0x1
  1e:   48 c7 45 d8 0d 00 00    mov    QWORD PTR [rbp-0x28],0xd
  25:   00 
  26:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f77206f6c6c6548
  2d:   20 77 6f 
  30:   48 89 45 ea             mov    QWORD PTR [rbp-0x16],rax
  34:   c7 45 f2 72 6c 64 21    mov    DWORD PTR [rbp-0xe],0x21646c72
  3b:   66 c7 45 f6 0a 00       mov    WORD PTR [rbp-0xa],0xa
  41:   48 8d 45 ea             lea    rax,[rbp-0x16]
  45:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  49:   b8 01 00 00 00          mov    eax,0x1
  4e:   8b 7d d4                mov    edi,DWORD PTR [rbp-0x2c]
  51:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
  55:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  59:   0f 05                   syscall 
  5b:   b8 00 00 00 00          mov    eax,0x0
  60:   48 8b 4d f8             mov    rcx,QWORD PTR [rbp-0x8]
  64:   64 48 33 0c 25 28 00    xor    rcx,QWORD PTR fs:0x28
  6b:   00 00 
  6d:   74 05                   je     74 <main+0x74>
  6f:   e8 00 00 00 00          call   74 <main+0x74>
  74:   c9                      leave  
  75:   c3                      ret    
-----------------------------------------------------------
Hex dump of "Hello World!\n":
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A
-----------------------------------------------------------
Hex dump of assembly_syscall.bin:
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 55 48 89 E5 48 83 EC 30 64 48 8B 04 25 28 00 00
  10: 00 48 89 45 F8 31 C0 C7 45 D4 01 00 00 00 48 C7
  20: 45 D8 0D 00 00 00 48 B8 48 65 6C 6C 6F 20 77 6F
  30: 48 89 45 EA C7 45 F2 72 6C 64 21 66 C7 45 F6 0A
  40: 00 48 8D 45 EA 48 89 45 E0 B8 01 00 00 00 8B 7D
  50: D4 48 8B 75 E0 48 8B 55 D8 0F 05 B8 00 00 00 00
  60: 48 8B 4D F8 64 48 33 0C 25 28 00 00 00 74 05 E8
  70: 00 00 00 00 C9 C3
-----------------------------------------------------------
Address lenght: 64 bits
File loaded in memory at 0x000000005A25C000
Executing the program...
Hello world!
Execution correctly returned.
-----------------------------------------------------------
```

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

C inline assembly "syscall" instruction for x64 machines usage example ("int 0x80" x86 version available as well):
https://stackoverflow.com/questions/9506353/how-to-invoke-a-system-call-via-syscall-or-sysenter-in-inline-assembly

Assembly "syscall" instruction for x64 machines usage example:
https://github.com/cirosantilli/x86-assembly-cheat/blob/279ab1add4302a4e43d31db9e3da688fc2ddbf9c/x86-64/linux/hello_world.asm#L8

x86's "int 0x80" assembly code example:
https://stackoverflow.com/questions/1817577/what-does-int-0x80-mean-in-assembly-code