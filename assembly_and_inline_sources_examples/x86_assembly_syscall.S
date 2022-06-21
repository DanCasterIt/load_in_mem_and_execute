/*source:*/
/*https://stackoverflow.com/questions/1817577/what-does-int-0x80-mean-in-assembly-code*/

/*How to compile*/
/*as -o x86_assembly_syscall.o x86_assembly_syscall.S*/
/*ld -o x86_assembly_syscall.out x86_assembly_syscall.o*/

/*unlike in C_inline_assembly_syscall.c this "int 0x80" assembly program works on x64 systems as well*/

.data
    s:
        .ascii "hello world\n"
        len = . - s
.text
    .global _start
    _start:

        movl $4, %eax   /* write system call number */
        movl $1, %ebx   /* stdout */
        movl $s, %ecx   /* the data to print */
        movl $len, %edx /* length of the buffer */
        int $0x80

        movl $1, %eax   /* exit system call number */
        movl $0, %ebx   /* exit status */
        int $0x80
