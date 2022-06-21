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
