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

// With the current `load_and_execute` settings when trying to overwrite data in `.bss`, `.data`
// or everything outside the function scope in the `.bin` file, generates a "Segmentation fault"
// error because `load_and_execute` changes the mapped memory permissions right after the content
// of the `.bin` file has been uploaded in memory using `mprotect(prog, dim, PROT_READ | PROT_EXEC)`

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
