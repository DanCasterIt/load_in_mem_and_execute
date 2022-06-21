#include <stdio.h>
#include <stdint.h>

#define EXAMPLE 5

int main(char argv, char *argc[])	{
#if EXAMPLE == 1
	uint16_t input = 1, output = 0;
	printf("input = %u, output = %u = %x\n", input, output, output);
	asm(
		".intel_syntax;"
		"MOV AX, %1;"
		"SHL AX, 0x0003;"
		"MOV %0, AX;"
		".att_syntax;"
		: "=r"(output)
		: "r"(input)
		: "eax"
	);
	printf("input = %u, output = %u = %x\n", input, output, output);
#elif EXAMPLE == 2
	asm(
		".intel_syntax;"
		"mov ax, 0b1;"
		".att_syntax;"
	);
#elif EXAMPLE == 3
	int temp;
	printf("temp = %d\n", temp);
	__asm__(
		".intel_syntax;"
		"mov eax, %1;"
		"mov %0, eax;"
		".att_syntax;"
		: "=r"(temp)
		: "r"(1) 
		: "eax"
	);
	printf("temp = %d\n", temp);
#elif EXAMPLE == 4
	int no = 100, val ;
	printf("no = %d, val = %d\n", no, val);
	asm ("mov %1, %%bx;"
		"mov %%bx, %0;"
		: "=r" ( val )        /* output */
		: "r" ( no )         /* input */
		: "%bx"         /* clobbered register */
	);
	printf("no = %d, val = %d\n", no, val);
#else
	int no = 100, val ;
	printf("no = %d, val = %d\n", no, val);
	asm ("movl %1, %%ebx;"
		"movl %%ebx, %0;"
		: "=r" ( val )        /* output */
		: "r" ( no )         /* input */
		: "%ebx"         /* clobbered register */
	);
	printf("no = %d, val = %d\n", no, val);
	return 0;
#endif
}
