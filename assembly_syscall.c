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
