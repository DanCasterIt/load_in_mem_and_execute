# load_in_mem_and_execute

This is a simple program that tryies to:

1) Generate a C source file
2) Compile it in pure instructions (no headers, no exe format)
3) Load it in memory
4) Jump at the memory address and execute it

Unfortunately I found no-way to let point 4 work.

## Output
```
>load_and_execute.exe

hello.o:     file format pe-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   48 83 ec 20             sub    $0x20,%rsp
   8:   89 4d 10                mov    %ecx,0x10(%rbp)
   b:   48 89 55 18             mov    %rdx,0x18(%rbp)
   f:   e8 00 00 00 00          callq  14 <main+0x14>
  14:   48 8d 0d 00 00 00 00    lea    0x0(%rip),%rcx        # 1b <main+0x1b>
  1b:   e8 00 00 00 00          callq  20 <main+0x20>
  20:   b8 00 00 00 00          mov    $0x0,%eax
  25:   48 83 c4 20             add    $0x20,%rsp
  29:   5d                      pop    %rbp
  2a:   c3                      retq
  2b:   90                      nop
  2c:   90                      nop
  2d:   90                      nop
  2e:   90                      nop
  2f:   90                      nop
>>>>>>>>>>>>>>>>>>>>>>hex dump of hello.bin>>>>>>>>>>>>>>>>>>>>>>
Binary file size is: 48 bytes
55 48 89 E5 48 83 EC 20 89 4D 10 48 89 55 18
E8 00 00 00 00 48 8D 0D 00 00 00 00 E8 00 00
00 00 B8 00 00 00 00 48 83 C4 20 5D C3 90 90
90 90 90
<<<<<<<<<<<<<<<<<<<<<<<hex dump of hello.bin<<<<<<<<<<<<<<<<<<<<<<<
Address lenght: 32 bits
File loaded in memory at 0x00BE1620
Trying to jump and execute...
input  = 0x00BE1620, output = 0x00000000
input  = 0x00BE1620, output = 0x00BE1620