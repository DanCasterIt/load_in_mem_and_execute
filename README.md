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

ssize_t x64_write(int fd, const void *buf, size_t size)
{
    ssize_t ret;
    asm volatile
    (
        "syscall"
        : "=a" (ret)
        //                 EDI      RSI       RDX
        : "0"(__NR_write), "D"(fd), "S"(buf), "d"(size)
        : "rcx", "r11", "memory"
    );
    return ret;
}

int main()  {
    char str[] = "Hello world!\n";
    x64_write(1, str, 13);
    return 0;
}

-----------------------------------------------------------
gcc -ffunction-sections -fno-asynchronous-unwind-tables -Qn -T link2.ls -nostdlib -Wl,--oformat=binary assembly_syscall.c -o assembly_syscall.bin

gcc -c -ffunction-sections -fno-asynchronous-unwind-tables -Qn -T link2.ls -nostdlib -Wl,--oformat=binary assembly_syscall.c -o assembly_syscall.o

-----------------------------------------------------------
objdump -D --disassembler-options intel assembly_syscall.o


assembly_syscall.o:     file format elf64-x86-64


Disassembly of section .text.x64_write:

0000000000000000 <x64_write>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   89 7d ec                mov    DWORD PTR [rbp-0x14],edi
   7:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
   b:   48 89 55 d8             mov    QWORD PTR [rbp-0x28],rdx
   f:   b8 01 00 00 00          mov    eax,0x1
  14:   8b 7d ec                mov    edi,DWORD PTR [rbp-0x14]
  17:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
  1b:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  1f:   0f 05                   syscall 
  21:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  25:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  29:   5d                      pop    rbp
  2a:   c3                      ret    

Disassembly of section .text.main:

0000000000000000 <main>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   48 83 ec 10             sub    rsp,0x10
   8:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f77206f6c6c6548
   f:   20 77 6f 
  12:   48 89 45 f2             mov    QWORD PTR [rbp-0xe],rax
  16:   c7 45 fa 72 6c 64 21    mov    DWORD PTR [rbp-0x6],0x21646c72
  1d:   66 c7 45 fe 0a 00       mov    WORD PTR [rbp-0x2],0xa
  23:   48 8d 45 f2             lea    rax,[rbp-0xe]
  27:   ba 0d 00 00 00          mov    edx,0xd
  2c:   48 89 c6                mov    rsi,rax
  2f:   bf 01 00 00 00          mov    edi,0x1
  34:   e8 00 00 00 00          call   39 <main+0x39>
  39:   b8 00 00 00 00          mov    eax,0x0
  3e:   c9                      leave  
  3f:   c3                      ret    
-----------------------------------------------------------
objdump -D -Mintel,x86-64 -b binary -m i386 assembly_syscall.bin


assembly_syscall.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   48 83 ec 10             sub    rsp,0x10
   8:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f77206f6c6c6548
   f:   20 77 6f 
  12:   48 89 45 f2             mov    QWORD PTR [rbp-0xe],rax
  16:   c7 45 fa 72 6c 64 21    mov    DWORD PTR [rbp-0x6],0x21646c72
  1d:   66 c7 45 fe 0a 00       mov    WORD PTR [rbp-0x2],0xa
  23:   48 8d 45 f2             lea    rax,[rbp-0xe]
  27:   ba 0d 00 00 00          mov    edx,0xd
  2c:   48 89 c6                mov    rsi,rax
  2f:   bf 01 00 00 00          mov    edi,0x1
  34:   e8 07 00 00 00          call   0x40
  39:   b8 00 00 00 00          mov    eax,0x0
  3e:   c9                      leave  
  3f:   c3                      ret    
  40:   55                      push   rbp
  41:   48 89 e5                mov    rbp,rsp
  44:   89 7d ec                mov    DWORD PTR [rbp-0x14],edi
  47:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
  4b:   48 89 55 d8             mov    QWORD PTR [rbp-0x28],rdx
  4f:   b8 01 00 00 00          mov    eax,0x1
  54:   8b 7d ec                mov    edi,DWORD PTR [rbp-0x14]
  57:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
  5b:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
  5f:   0f 05                   syscall 
  61:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
  65:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
  69:   5d                      pop    rbp
  6a:   c3                      ret    
-----------------------------------------------------------
Hex dump of "Hello World!\n":
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 48 65 6C 6C 6F 20 57 6F 72 6C 64 21 0A
-----------------------------------------------------------
Hex dump of assembly_syscall.bin:
       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0: 55 48 89 E5 48 83 EC 10 48 B8 48 65 6C 6C 6F 20
  10: 77 6F 48 89 45 F2 C7 45 FA 72 6C 64 21 66 C7 45
  20: FE 0A 00 48 8D 45 F2 BA 0D 00 00 00 48 89 C6 BF
  30: 01 00 00 00 E8 07 00 00 00 B8 00 00 00 00 C9 C3
  40: 55 48 89 E5 89 7D EC 48 89 75 E0 48 89 55 D8 B8
  50: 01 00 00 00 8B 7D EC 48 8B 75 E0 48 8B 55 D8 0F
  60: 05 48 89 45 F8 48 8B 45 F8 5D C3
-----------------------------------------------------------
Address lenght: 64 bits
File loaded in memory at 0x00000000A2817000
Entry point relative to the binary file at 0x0
Jumping at 0x00000000A2817000 to start executing...
Hello world!
Execution correctly returned.
-----------------------------------------------------------
```

## Interesting sources around the web
Use mmap() to allocate executable memory:
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

Linker file example and build script example for raw binary:
https://forum.osdev.org/viewtopic.php?p=140075

Why did ld turn my 5 lines of library-less C into a 100MB binary?
https://stackoverflow.com/questions/63106614/why-did-ld-turn-my-5-lines-of-library-less-c-into-a-100mb-binary

Build an x86 bootloader plus kernel (there's a linker script):
https://stackoverflow.com/questions/6716489/compiling-c-and-assembly-to-pure-machine-code
https://wiki.osdev.org/Bare_Bones

Remove the .eh_frame section from the compiled binary:
https://stackoverflow.com/questions/26300819/why-gcc-compiled-c-program-needs-eh-frame-section

Remove the .comment section from the compiled binary:
https://stackoverflow.com/questions/6263425/why-does-the-compiler-version-appear-in-my-elf-executable

Tell GCC to put the 'main' function at the start of the .text section:
https://stackoverflow.com/questions/19470666/gcc-how-to-tell-gcc-to-put-the-main-function-at-the-start-of-the-text-sectio

Disassembling A Flat Binary File Using objdump
https://stackoverflow.com/questions/14290879/disassembling-a-flat-binary-file-using-objdump
