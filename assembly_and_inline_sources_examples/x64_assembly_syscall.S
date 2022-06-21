/*source:*/
/*https://github.com/cirosantilli/x86-assembly-cheat/blob/279ab1add4302a4e43d31db9e3da688fc2ddbf9c/x86-64/linux/hello_world.asm#L8*/

/*How to compile*/
/*as -o x64_assembly_syscall.o x64_assembly_syscall.S*/
/*ld -o x64_assembly_syscall.out x64_assembly_syscall.o*/

global _start
_start:
    ; Write.
    mov rax, 1
    mov rdi, 1
    mov rsi, hello_world
    mov rdx, hello_world_len
    syscall

    ; Exit.
    mov rax, 60
    mov rdi, 0
    syscall

hello_world db "hello world", 10
hello_world_len equ $ - hello_world