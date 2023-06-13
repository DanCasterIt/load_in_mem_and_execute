# load_in_mem_and_execute
This is a simple program that:
1) Generates a C source file
2) Compiles it in pure instructions (no headers, no ELF executable format)
3) Loads it in memory
4) Jumps at the memory address where it is stored and executes it

## How to compile
```make all```
or just run
```./build_and_run_example.sh```

## Output
```
$ ./build_and_run_example.sh 
+ pushd /home/daniele/load_in_mem_and_execute
~/load_in_mem_and_execute ~/load_in_mem_and_execute
+ make BLOB_NAME=memory_region_examination all
gcc -o load_and_execute load_and_execute.o
gcc -o memory_region_examination.bin memory_region_examination.o -ffunction-sections -fno-asynchronous-unwind-tables -Qn -T link2.ls -nostdlib -Wl,--oformat=binary
+ cat memory_region_examination.c
#include <stdio.h>
#include <asm/unistd.h>

#define ALIGH_CONSTANT 0x10
#define ATTRIBUTES __attribute__ ((aligned (ALIGH_CONSTANT)))
#define STRING "Hello World!\n"
#define LEN 13

#define IN str9
#define OUT funcstr0

char                        str0[]      ATTRIBUTES = STRING;    //goes to .data
char                        str1[LEN]   ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
char                        str2[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
const char                  str3[]      ATTRIBUTES = STRING;    //goes to .rodata (read only data)
const char                  str4[LEN]   ATTRIBUTES = {'\0'};    //goes to .rodata (read only data)
const char                  str5[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
volatile char               str6[]      ATTRIBUTES = STRING;    //goes to .data
volatile char               str7[LEN]   ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
volatile char               str8[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
volatile const char         str9[]      ATTRIBUTES = STRING;    //goes to .data
volatile const char         str10[LEN]  ATTRIBUTES = {'\0'};    //goes to .data
volatile const char         str11[LEN]  ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
static char                 str12[]     ATTRIBUTES = STRING;    //goes to .data
static char                 str13[LEN]  ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
static char                 str14[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
static const char           str15[]     ATTRIBUTES = STRING;    //goes to .rodata (read only data)
static const char           str16[LEN]  ATTRIBUTES = {'\0'};    //goes to .rodata (read only data)
static const char           str17[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
static volatile char        str18[]     ATTRIBUTES = STRING;    //goes to .data
static volatile char        str19[LEN]  ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
static volatile char        str20[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
static volatile const char  str21[]     ATTRIBUTES = STRING;    //goes to .data
static volatile const char  str22[LEN]  ATTRIBUTES = {'\0'};    //goes to .data
static volatile const char  str23[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)

//with the current compilation environment in load_and_execute,
//trying to overwrite data in .bss, .data or everything outside
//the function scope generates a "Segmentation fault" error.

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
    char                        funcstr0[]      ATTRIBUTES = STRING;    //goes nowhere, mentioned only in symbol table
    char                        funcstr1[LEN]   ATTRIBUTES = {'\0'};    //goes nowhere, mentioned only in symbol table
    char                        funcstr2[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
    const char                  funcstr3[]      ATTRIBUTES = STRING;    //goes nowhere, mentioned only in symbol table
    const char                  funcstr4[LEN]   ATTRIBUTES = {'\0'};    //goes nowhere, mentioned only in symbol table
    const char                  funcstr5[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
    volatile char               funcstr6[]      ATTRIBUTES = STRING;    //goes nowhere, mentioned only in symbol table
    volatile char               funcstr7[LEN]   ATTRIBUTES = {'\0'};    //goes nowhere, mentioned only in symbol table
    volatile char               funcstr8[LEN]   ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
    volatile const char         funcstr9[]      ATTRIBUTES = STRING;    //goes nowhere, mentioned only in symbol table
    volatile const char         funcstr10[LEN]  ATTRIBUTES = {'\0'};    //goes nowhere, mentioned only in symbol table
    volatile const char         funcstr11[LEN]  ATTRIBUTES;             //goes nowhere, mentioned only in symbol table
    static char                 funcstr12[]     ATTRIBUTES = STRING;    //goes to .data
    static char                 funcstr13[LEN]  ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
    static char                 funcstr14[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
    static const char           funcstr15[]     ATTRIBUTES = STRING;    //goes to .rodata (read only data)
    static const char           funcstr16[LEN]  ATTRIBUTES = {'\0'};    //goes to .rodata (read only data)
    static const char           funcstr17[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
    static volatile char        funcstr18[]     ATTRIBUTES = STRING;    //goes to .data
    static volatile char        funcstr19[LEN]  ATTRIBUTES = {'\0'};    //goes to .bss (Block Started by Symbol)
    static volatile char        funcstr20[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
    static volatile const char  funcstr21[]     ATTRIBUTES = STRING;    //goes to .data
    static volatile const char  funcstr22[LEN]  ATTRIBUTES = {'\0'};    //goes to .data
    static volatile const char  funcstr23[LEN]  ATTRIBUTES;             //goes to .bss (Block Started by Symbol)
    int i;
    for(i = 0; i < 13; i++)
        OUT[i] = IN[i];
    x64_write(1, OUT, 13);
    return 0;
}
+ make BLOB_NAME=memory_region_examination dumpBlob
objdump -D --disassembler-options intel memory_region_examination.o

memory_region_examination.o:     file format elf64-x86-64


Disassembly of section .data:

0000000000000000 <str0>:
   0:   48                      rex.W
   1:   65 6c                   gs ins BYTE PTR es:[rdi],dx
   3:   6c                      ins    BYTE PTR es:[rdi],dx
   4:   6f                      outs   dx,DWORD PTR ds:[rsi]
   5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
   8:   72 6c                   jb     76 <str22+0x6>
   a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
   d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000010 <str6>:
  10:   48                      rex.W
  11:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  13:   6c                      ins    BYTE PTR es:[rdi],dx
  14:   6f                      outs   dx,DWORD PTR ds:[rsi]
  15:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  18:   72 6c                   jb     86 <funcstr22.2301+0x6>
  1a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  1d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000020 <str9>:
  20:   48                      rex.W
  21:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  23:   6c                      ins    BYTE PTR es:[rdi],dx
  24:   6f                      outs   dx,DWORD PTR ds:[rsi]
  25:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  28:   72 6c                   jb     96 <funcstr21.2300+0x6>
  2a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  2d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000030 <str10>:
        ...

0000000000000040 <str12>:
  40:   48                      rex.W
  41:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  43:   6c                      ins    BYTE PTR es:[rdi],dx
  44:   6f                      outs   dx,DWORD PTR ds:[rsi]
  45:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  48:   72 6c                   jb     b6 <funcstr12.2291+0x6>
  4a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  4d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000050 <str18>:
  50:   48                      rex.W
  51:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  53:   6c                      ins    BYTE PTR es:[rdi],dx
  54:   6f                      outs   dx,DWORD PTR ds:[rsi]
  55:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  58:   72 6c                   jb     c6 <funcstr14.2293+0x6>
  5a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  5d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000060 <str21>:
  60:   48                      rex.W
  61:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  63:   6c                      ins    BYTE PTR es:[rdi],dx
  64:   6f                      outs   dx,DWORD PTR ds:[rsi]
  65:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  68:   72 6c                   jb     d6 <funcstr13.2292+0x6>
  6a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  6d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000070 <str22>:
        ...

0000000000000080 <funcstr22.2301>:
        ...

0000000000000090 <funcstr21.2300>:
  90:   48                      rex.W
  91:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  93:   6c                      ins    BYTE PTR es:[rdi],dx
  94:   6f                      outs   dx,DWORD PTR ds:[rsi]
  95:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  98:   72 6c                   jb     106 <funcstr13.2292+0x36>
  9a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  9d:   00 00                   add    BYTE PTR [rax],al
        ...

00000000000000a0 <funcstr18.2297>:
  a0:   48                      rex.W
  a1:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  a3:   6c                      ins    BYTE PTR es:[rdi],dx
  a4:   6f                      outs   dx,DWORD PTR ds:[rsi]
  a5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  a8:   72 6c                   jb     116 <funcstr13.2292+0x46>
  aa:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  ad:   00 00                   add    BYTE PTR [rax],al
        ...

00000000000000b0 <funcstr12.2291>:
  b0:   48                      rex.W
  b1:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  b3:   6c                      ins    BYTE PTR es:[rdi],dx
  b4:   6f                      outs   dx,DWORD PTR ds:[rsi]
  b5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  b8:   72 6c                   jb     126 <funcstr13.2292+0x56>
  ba:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...

Disassembly of section .bss:

0000000000000000 <str1>:
        ...

0000000000000010 <str7>:
        ...

0000000000000020 <str13>:
        ...

0000000000000030 <str14>:
        ...

0000000000000040 <str17>:
        ...

0000000000000050 <str19>:
        ...

0000000000000060 <str20>:
        ...

0000000000000070 <str23>:
        ...

0000000000000080 <funcstr23.2302>:
        ...

0000000000000090 <funcstr20.2299>:
        ...

00000000000000a0 <funcstr19.2298>:
        ...

00000000000000b0 <funcstr17.2296>:
        ...

00000000000000c0 <funcstr14.2293>:
        ...

00000000000000d0 <funcstr13.2292>:
        ...

Disassembly of section .rodata:

0000000000000000 <str3>:
   0:   48                      rex.W
   1:   65 6c                   gs ins BYTE PTR es:[rdi],dx
   3:   6c                      ins    BYTE PTR es:[rdi],dx
   4:   6f                      outs   dx,DWORD PTR ds:[rsi]
   5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
   8:   72 6c                   jb     76 <str22+0x6>
   a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
   d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000010 <str4>:
        ...

0000000000000020 <str15>:
  20:   48                      rex.W
  21:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  23:   6c                      ins    BYTE PTR es:[rdi],dx
  24:   6f                      outs   dx,DWORD PTR ds:[rsi]
  25:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  28:   72 6c                   jb     96 <funcstr21.2300+0x6>
  2a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
  2d:   00 00                   add    BYTE PTR [rax],al
        ...

0000000000000030 <str16>:
        ...

0000000000000040 <funcstr16.2295>:
        ...

0000000000000050 <funcstr15.2294>:
  50:   48                      rex.W
  51:   65 6c                   gs ins BYTE PTR es:[rdi],dx
  53:   6c                      ins    BYTE PTR es:[rdi],dx
  54:   6f                      outs   dx,DWORD PTR ds:[rsi]
  55:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
  58:   72 6c                   jb     c6 <funcstr14.2293+0x6>
  5a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...

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
   4:   48 81 ec d0 00 00 00    sub    rsp,0xd0
   b:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f57206f6c6c6548
  12:   20 57 6f 
  15:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  19:   c7 45 e8 72 6c 64 21    mov    DWORD PTR [rbp-0x18],0x21646c72
  20:   66 c7 45 ec 0a 00       mov    WORD PTR [rbp-0x14],0xa
  26:   48 c7 45 d0 00 00 00    mov    QWORD PTR [rbp-0x30],0x0
  2d:   00 
  2e:   c7 45 d8 00 00 00 00    mov    DWORD PTR [rbp-0x28],0x0
  35:   c6 45 dc 00             mov    BYTE PTR [rbp-0x24],0x0
  39:   48 89 45 b0             mov    QWORD PTR [rbp-0x50],rax
  3d:   c7 45 b8 72 6c 64 21    mov    DWORD PTR [rbp-0x48],0x21646c72
  44:   66 c7 45 bc 0a 00       mov    WORD PTR [rbp-0x44],0xa
  4a:   48 c7 45 a0 00 00 00    mov    QWORD PTR [rbp-0x60],0x0
  51:   00 
  52:   c7 45 a8 00 00 00 00    mov    DWORD PTR [rbp-0x58],0x0
  59:   c6 45 ac 00             mov    BYTE PTR [rbp-0x54],0x0
  5d:   48 89 45 80             mov    QWORD PTR [rbp-0x80],rax
  61:   c7 45 88 72 6c 64 21    mov    DWORD PTR [rbp-0x78],0x21646c72
  68:   66 c7 45 8c 0a 00       mov    WORD PTR [rbp-0x74],0xa
  6e:   48 c7 85 70 ff ff ff    mov    QWORD PTR [rbp-0x90],0x0
  75:   00 00 00 00 
  79:   c7 85 78 ff ff ff 00    mov    DWORD PTR [rbp-0x88],0x0
  80:   00 00 00 
  83:   c6 85 7c ff ff ff 00    mov    BYTE PTR [rbp-0x84],0x0
  8a:   48 89 85 50 ff ff ff    mov    QWORD PTR [rbp-0xb0],rax
  91:   c7 85 58 ff ff ff 72    mov    DWORD PTR [rbp-0xa8],0x21646c72
  98:   6c 64 21 
  9b:   66 c7 85 5c ff ff ff    mov    WORD PTR [rbp-0xa4],0xa
  a2:   0a 00 
  a4:   48 c7 85 40 ff ff ff    mov    QWORD PTR [rbp-0xc0],0x0
  ab:   00 00 00 00 
  af:   c7 85 48 ff ff ff 00    mov    DWORD PTR [rbp-0xb8],0x0
  b6:   00 00 00 
  b9:   c6 85 4c ff ff ff 00    mov    BYTE PTR [rbp-0xb4],0x0
  c0:   c7 45 fc 00 00 00 00    mov    DWORD PTR [rbp-0x4],0x0
  c7:   eb 1e                   jmp    e7 <main+0xe7>
  c9:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
  cc:   48 63 d0                movsxd rdx,eax
  cf:   48 8d 05 00 00 00 00    lea    rax,[rip+0x0]        # d6 <main+0xd6>
  d6:   0f b6 14 02             movzx  edx,BYTE PTR [rdx+rax*1]
  da:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
  dd:   48 98                   cdqe   
  df:   88 54 05 e0             mov    BYTE PTR [rbp+rax*1-0x20],dl
  e3:   83 45 fc 01             add    DWORD PTR [rbp-0x4],0x1
  e7:   83 7d fc 0c             cmp    DWORD PTR [rbp-0x4],0xc
  eb:   7e dc                   jle    c9 <main+0xc9>
  ed:   48 8d 45 e0             lea    rax,[rbp-0x20]
  f1:   ba 0d 00 00 00          mov    edx,0xd
  f6:   48 89 c6                mov    rsi,rax
  f9:   bf 01 00 00 00          mov    edi,0x1
  fe:   e8 00 00 00 00          call   103 <main+0x103>
 103:   b8 00 00 00 00          mov    eax,0x0
 108:   c9                      leave  
 109:   c3                      ret    
-----Sections merged together in a single binary blob------
objdump -D -Mintel,x86-64 -b binary -m i386 memory_region_examination.bin

memory_region_examination.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:   55                      push   rbp
   1:   48 89 e5                mov    rbp,rsp
   4:   48 81 ec d0 00 00 00    sub    rsp,0xd0
   b:   48 b8 48 65 6c 6c 6f    movabs rax,0x6f57206f6c6c6548
  12:   20 57 6f 
  15:   48 89 45 e0             mov    QWORD PTR [rbp-0x20],rax
  19:   c7 45 e8 72 6c 64 21    mov    DWORD PTR [rbp-0x18],0x21646c72
  20:   66 c7 45 ec 0a 00       mov    WORD PTR [rbp-0x14],0xa
  26:   48 c7 45 d0 00 00 00    mov    QWORD PTR [rbp-0x30],0x0
  2d:   00 
  2e:   c7 45 d8 00 00 00 00    mov    DWORD PTR [rbp-0x28],0x0
  35:   c6 45 dc 00             mov    BYTE PTR [rbp-0x24],0x0
  39:   48 89 45 b0             mov    QWORD PTR [rbp-0x50],rax
  3d:   c7 45 b8 72 6c 64 21    mov    DWORD PTR [rbp-0x48],0x21646c72
  44:   66 c7 45 bc 0a 00       mov    WORD PTR [rbp-0x44],0xa
  4a:   48 c7 45 a0 00 00 00    mov    QWORD PTR [rbp-0x60],0x0
  51:   00 
  52:   c7 45 a8 00 00 00 00    mov    DWORD PTR [rbp-0x58],0x0
  59:   c6 45 ac 00             mov    BYTE PTR [rbp-0x54],0x0
  5d:   48 89 45 80             mov    QWORD PTR [rbp-0x80],rax
  61:   c7 45 88 72 6c 64 21    mov    DWORD PTR [rbp-0x78],0x21646c72
  68:   66 c7 45 8c 0a 00       mov    WORD PTR [rbp-0x74],0xa
  6e:   48 c7 85 70 ff ff ff    mov    QWORD PTR [rbp-0x90],0x0
  75:   00 00 00 00 
  79:   c7 85 78 ff ff ff 00    mov    DWORD PTR [rbp-0x88],0x0
  80:   00 00 00 
  83:   c6 85 7c ff ff ff 00    mov    BYTE PTR [rbp-0x84],0x0
  8a:   48 89 85 50 ff ff ff    mov    QWORD PTR [rbp-0xb0],rax
  91:   c7 85 58 ff ff ff 72    mov    DWORD PTR [rbp-0xa8],0x21646c72
  98:   6c 64 21 
  9b:   66 c7 85 5c ff ff ff    mov    WORD PTR [rbp-0xa4],0xa
  a2:   0a 00 
  a4:   48 c7 85 40 ff ff ff    mov    QWORD PTR [rbp-0xc0],0x0
  ab:   00 00 00 00 
  af:   c7 85 48 ff ff ff 00    mov    DWORD PTR [rbp-0xb8],0x0
  b6:   00 00 00 
  b9:   c6 85 4c ff ff ff 00    mov    BYTE PTR [rbp-0xb4],0x0
  c0:   c7 45 fc 00 00 00 00    mov    DWORD PTR [rbp-0x4],0x0
  c7:   eb 1e                   jmp    0xe7
  c9:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
  cc:   48 63 d0                movsxd rdx,eax
  cf:   48 8d 05 ea 00 00 00    lea    rax,[rip+0xea]        # 0x1c0
  d6:   0f b6 14 02             movzx  edx,BYTE PTR [rdx+rax*1]
  da:   8b 45 fc                mov    eax,DWORD PTR [rbp-0x4]
  dd:   48 98                   cdqe   
  df:   88 54 05 e0             mov    BYTE PTR [rbp+rax*1-0x20],dl
  e3:   83 45 fc 01             add    DWORD PTR [rbp-0x4],0x1
  e7:   83 7d fc 0c             cmp    DWORD PTR [rbp-0x4],0xc
  eb:   7e dc                   jle    0xc9
  ed:   48 8d 45 e0             lea    rax,[rbp-0x20]
  f1:   ba 0d 00 00 00          mov    edx,0xd
  f6:   48 89 c6                mov    rsi,rax
  f9:   bf 01 00 00 00          mov    edi,0x1
  fe:   e8 07 00 00 00          call   0x10a
 103:   b8 00 00 00 00          mov    eax,0x0
 108:   c9                      leave  
 109:   c3                      ret    
 10a:   55                      push   rbp
 10b:   48 89 e5                mov    rbp,rsp
 10e:   89 7d ec                mov    DWORD PTR [rbp-0x14],edi
 111:   48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
 115:   48 89 55 d8             mov    QWORD PTR [rbp-0x28],rdx
 119:   b8 01 00 00 00          mov    eax,0x1
 11e:   8b 7d ec                mov    edi,DWORD PTR [rbp-0x14]
 121:   48 8b 75 e0             mov    rsi,QWORD PTR [rbp-0x20]
 125:   48 8b 55 d8             mov    rdx,QWORD PTR [rbp-0x28]
 129:   0f 05                   syscall 
 12b:   48 89 45 f8             mov    QWORD PTR [rbp-0x8],rax
 12f:   48 8b 45 f8             mov    rax,QWORD PTR [rbp-0x8]
 133:   5d                      pop    rbp
 134:   c3                      ret    
        ...
 13d:   00 00                   add    BYTE PTR [rax],al
 13f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 142:   6c                      ins    BYTE PTR es:[rdi],dx
 143:   6c                      ins    BYTE PTR es:[rdi],dx
 144:   6f                      outs   dx,DWORD PTR ds:[rsi]
 145:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 148:   72 6c                   jb     0x1b6
 14a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...
 15d:   00 00                   add    BYTE PTR [rax],al
 15f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 162:   6c                      ins    BYTE PTR es:[rdi],dx
 163:   6c                      ins    BYTE PTR es:[rdi],dx
 164:   6f                      outs   dx,DWORD PTR ds:[rsi]
 165:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 168:   72 6c                   jb     0x1d6
 16a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...
 18d:   00 00                   add    BYTE PTR [rax],al
 18f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 192:   6c                      ins    BYTE PTR es:[rdi],dx
 193:   6c                      ins    BYTE PTR es:[rdi],dx
 194:   6f                      outs   dx,DWORD PTR ds:[rsi]
 195:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 198:   72 6c                   jb     0x206
 19a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 19d:   00 00                   add    BYTE PTR [rax],al
 19f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 1a2:   6c                      ins    BYTE PTR es:[rdi],dx
 1a3:   6c                      ins    BYTE PTR es:[rdi],dx
 1a4:   6f                      outs   dx,DWORD PTR ds:[rsi]
 1a5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 1a8:   72 6c                   jb     0x216
 1aa:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 1ad:   00 00                   add    BYTE PTR [rax],al
 1af:   00 48 65                add    BYTE PTR [rax+0x65],cl
 1b2:   6c                      ins    BYTE PTR es:[rdi],dx
 1b3:   6c                      ins    BYTE PTR es:[rdi],dx
 1b4:   6f                      outs   dx,DWORD PTR ds:[rsi]
 1b5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 1b8:   72 6c                   jb     0x226
 1ba:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 1bd:   00 00                   add    BYTE PTR [rax],al
 1bf:   00 48 65                add    BYTE PTR [rax+0x65],cl
 1c2:   6c                      ins    BYTE PTR es:[rdi],dx
 1c3:   6c                      ins    BYTE PTR es:[rdi],dx
 1c4:   6f                      outs   dx,DWORD PTR ds:[rsi]
 1c5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 1c8:   72 6c                   jb     0x236
 1ca:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...
 1dd:   00 00                   add    BYTE PTR [rax],al
 1df:   00 48 65                add    BYTE PTR [rax+0x65],cl
 1e2:   6c                      ins    BYTE PTR es:[rdi],dx
 1e3:   6c                      ins    BYTE PTR es:[rdi],dx
 1e4:   6f                      outs   dx,DWORD PTR ds:[rsi]
 1e5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 1e8:   72 6c                   jb     0x256
 1ea:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 1ed:   00 00                   add    BYTE PTR [rax],al
 1ef:   00 48 65                add    BYTE PTR [rax+0x65],cl
 1f2:   6c                      ins    BYTE PTR es:[rdi],dx
 1f3:   6c                      ins    BYTE PTR es:[rdi],dx
 1f4:   6f                      outs   dx,DWORD PTR ds:[rsi]
 1f5:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 1f8:   72 6c                   jb     0x266
 1fa:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 1fd:   00 00                   add    BYTE PTR [rax],al
 1ff:   00 48 65                add    BYTE PTR [rax+0x65],cl
 202:   6c                      ins    BYTE PTR es:[rdi],dx
 203:   6c                      ins    BYTE PTR es:[rdi],dx
 204:   6f                      outs   dx,DWORD PTR ds:[rsi]
 205:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 208:   72 6c                   jb     0x276
 20a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...
 22d:   00 00                   add    BYTE PTR [rax],al
 22f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 232:   6c                      ins    BYTE PTR es:[rdi],dx
 233:   6c                      ins    BYTE PTR es:[rdi],dx
 234:   6f                      outs   dx,DWORD PTR ds:[rsi]
 235:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 238:   72 6c                   jb     0x2a6
 23a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 23d:   00 00                   add    BYTE PTR [rax],al
 23f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 242:   6c                      ins    BYTE PTR es:[rdi],dx
 243:   6c                      ins    BYTE PTR es:[rdi],dx
 244:   6f                      outs   dx,DWORD PTR ds:[rsi]
 245:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 248:   72 6c                   jb     0x2b6
 24a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
 24d:   00 00                   add    BYTE PTR [rax],al
 24f:   00 48 65                add    BYTE PTR [rax+0x65],cl
 252:   6c                      ins    BYTE PTR es:[rdi],dx
 253:   6c                      ins    BYTE PTR es:[rdi],dx
 254:   6f                      outs   dx,DWORD PTR ds:[rsi]
 255:   20 57 6f                and    BYTE PTR [rdi+0x6f],dl
 258:   72 6c                   jb     0x2c6
 25a:   64 21 0a                and    DWORD PTR fs:[rdx],ecx
        ...
+ ./load_and_execute memory_region_examination.bin
Address lenght: 64 bits
File loaded in memory at 0x000000003C1A0000
Entry point relative to the binary file at 0x0
Jumping at 0x000000003C1A0000 to start executing...
Hello World!
Execution correctly returned.
+ echo 'Hello World!'
+ hd
00000000  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a           |Hello World!.|
0000000d
+ hd memory_region_examination.bin
00000000  55 48 89 e5 48 81 ec d0  00 00 00 48 b8 48 65 6c  |UH..H......H.Hel|
00000010  6c 6f 20 57 6f 48 89 45  e0 c7 45 e8 72 6c 64 21  |lo WoH.E..E.rld!|
00000020  66 c7 45 ec 0a 00 48 c7  45 d0 00 00 00 00 c7 45  |f.E...H.E......E|
00000030  d8 00 00 00 00 c6 45 dc  00 48 89 45 b0 c7 45 b8  |......E..H.E..E.|
00000040  72 6c 64 21 66 c7 45 bc  0a 00 48 c7 45 a0 00 00  |rld!f.E...H.E...|
00000050  00 00 c7 45 a8 00 00 00  00 c6 45 ac 00 48 89 45  |...E......E..H.E|
00000060  80 c7 45 88 72 6c 64 21  66 c7 45 8c 0a 00 48 c7  |..E.rld!f.E...H.|
00000070  85 70 ff ff ff 00 00 00  00 c7 85 78 ff ff ff 00  |.p.........x....|
00000080  00 00 00 c6 85 7c ff ff  ff 00 48 89 85 50 ff ff  |.....|....H..P..|
00000090  ff c7 85 58 ff ff ff 72  6c 64 21 66 c7 85 5c ff  |...X...rld!f..\.|
000000a0  ff ff 0a 00 48 c7 85 40  ff ff ff 00 00 00 00 c7  |....H..@........|
000000b0  85 48 ff ff ff 00 00 00  00 c6 85 4c ff ff ff 00  |.H.........L....|
000000c0  c7 45 fc 00 00 00 00 eb  1e 8b 45 fc 48 63 d0 48  |.E........E.Hc.H|
000000d0  8d 05 ea 00 00 00 0f b6  14 02 8b 45 fc 48 98 88  |...........E.H..|
000000e0  54 05 e0 83 45 fc 01 83  7d fc 0c 7e dc 48 8d 45  |T...E...}..~.H.E|
000000f0  e0 ba 0d 00 00 00 48 89  c6 bf 01 00 00 00 e8 07  |......H.........|
00000100  00 00 00 b8 00 00 00 00  c9 c3 55 48 89 e5 89 7d  |..........UH...}|
00000110  ec 48 89 75 e0 48 89 55  d8 b8 01 00 00 00 8b 7d  |.H.u.H.U.......}|
00000120  ec 48 8b 75 e0 48 8b 55  d8 0f 05 48 89 45 f8 48  |.H.u.H.U...H.E.H|
00000130  8b 45 f8 5d c3 00 00 00  00 00 00 00 00 00 00 00  |.E.]............|
00000140  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a 00 00 00  |Hello World!....|
00000150  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000160  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a 00 00 00  |Hello World!....|
00000170  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000190  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a 00 00 00  |Hello World!....|
*
000001d0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000001e0  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a 00 00 00  |Hello World!....|
*
00000210  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000230  48 65 6c 6c 6f 20 57 6f  72 6c 64 21 0a 00 00 00  |Hello World!....|
*
0000025e
+ make BLOB_NAME=memory_region_examination clean
rm -f load_and_execute load_and_execute.o
rm -f memory_region_examination.bin memory_region_examination.o
+ popd
~/load_in_mem_and_execute
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

Program memory map and data segments:
https://en.wikipedia.org/wiki/Data_segment

## Known Issues
With the current `load_and_execute` settings when trying to overwrite data in `.bss`, `.data` or everything outside the function scope in the `.bin` file, generates a "Segmentation fault" error because `load_and_execute` changes the mapped memory permissions right after the content of the `.bin` file has been uploaded in memory using `mprotect(prog, dim, PROT_READ | PROT_EXEC)`
