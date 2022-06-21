#include <stdio.h>
#include <asm/unistd.h>

//source :
//https://stackoverflow.com/questions/9506353/how-to-invoke-a-system-call-via-syscall-or-sysenter-in-inline-assembly

// x86-64 Linux
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

//the following x86 write() functions aren't working at least on my Linux x64 system
//but the same "int 0x80" assembly code runs on my machine if compiled
//from an assembly file like x86_assembly_syscall.S instead of an inline C assembly function.
//At the moment I don't understand the reason why.

// i386 Linux
ssize_t x86_write(int fd, const void *buf, size_t size)
{
    ssize_t ret;
    asm volatile
    (
        "int $0x80"
        : "=a" (ret)
        : "0"(__NR_write), "b"(fd), "c"(buf), "d"(size)
        : "memory"    // the kernel dereferences pointer args
    );
    return ret;
}

//source :
//https://stackoverflow.com/questions/5131568/writing-a-linux-int-80h-system-call-wrapper-in-gnu-c-inline-assembly

int write(const char *str, int len) {
    __asm__ volatile ("int $0x80" 
     : "=a" (len)
     : "a" (4), "b" (1), "c" (str), "d" (len),
       "m"( *(const char (*)[])str )       // "dummy" input instead of memory clobber
     );
    return len;
}

void write2(char *str, int len) {
    __asm__ volatile (
        "movl $4, %%eax;"      // SYS_write
        "movl $1, %%ebx;"      // file descriptor = stdout_fd
        "movl %0, %%ecx;"
        "movl %1, %%edx;"
        "int $0x80"
        :: "g" (str), "g" (len)       // input values we MOV from
        : "eax", "ebx", "ecx", "edx", // registers we destroy
          "memory"                    // memory has to be in sync so we can read it
     );
}

int main()  {
    char str[] = "Hello world!\n";
    x64_write(1, str, 13); //should work on x64 Linux systems only, it works on mine
    x86_write(1, str, 13); //doesn't work at least on x64 systems
    write(str, 13);        //doesn't work at least on x64 systems
    write2(str, 13);       //doesn't work at least on x64 systems
    return 0;
}
