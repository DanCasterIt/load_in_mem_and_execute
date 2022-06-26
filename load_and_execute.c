#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

#define JUMP_EXECUTE_MODE 0
#define COMPILE_MODE 1
#define SEPARATION "-----------------------------------------------------------\n"

void print_c_program(char *file_name);
void dump(void **ptr, int size, char name[]);

int main(int argc, char *argv[])	{
	char program_name[] = "memory_region_examination";
	char str[] = "Hello World!\n";
	int dim, entry_point;
	FILE *fd;
	uint8_t *prog;
	uintptr_t tmp;

#if COMPILE_MODE == 0
	//if using this compile option, the entry_point variable must be updated
	char options[] = "--freestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -Wall -Werror -O0 -Iinc";
	char linker_options[] = "-T link.ls -nostdlib -nostartfiles -nodefaultlibs";
#else
	//char options[] = "-fno-pie -ffunction-sections -fno-asynchronous-unwind-tables -Qn";
	//char linker_options[] = "-T link2.ls -Wl,-Ttext=0x1000 -nostdlib -Wl,--oformat=binary";
	char options[] = "-ffunction-sections -fno-asynchronous-unwind-tables -Qn";
	char linker_options[] = "-T link2.ls -nostdlib -Wl,--oformat=binary";
#endif
	char buffer[100 + strlen(options) + strlen(linker_options)];

	strcpy(buffer, program_name);
	strcat(buffer, ".c");
	printf(SEPARATION);
	print_c_program(buffer);
	printf(SEPARATION);

	sprintf(buffer, "gcc %s %s %s.c -o %s.bin", options, linker_options, program_name, program_name);
	printf("%s\n\n", buffer);
	if(system(buffer) > 0)
		return -1;
	sprintf(buffer, "gcc -c %s %s %s.c -o %s.o", options, linker_options, program_name, program_name);
	printf("%s\n\n", buffer);
	if(system(buffer) > 0)
		return -1;
	printf(SEPARATION);
	sprintf(buffer, "objdump -D --disassembler-options intel %s.o", program_name);
	printf("%s\n\n", buffer);
	if(system(buffer) > 0)
		return -1;
	printf(SEPARATION);
	//sprintf(buffer, "objdump -D -Mintel,i386 -b binary -m i386 %s.o", program_name);	//x86
	sprintf(buffer, "objdump -D -Mintel,x86-64 -b binary -m i386 %s.bin", program_name);	//x64
	printf("%s\n\n", buffer);
	if(system(buffer) > 0)
		return -1;

	strcpy(buffer, program_name);
	strcat(buffer, ".bin");
	fd = fopen(buffer, "rb");
	fseek(fd, 0, SEEK_END);
	dim = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	//heap and stack memory can't be executed, so no malloc() can be used.
	//Allocate some executable memory using mmap():
	//PROT_EXEC = executable memory
	//NULL = allocate memory
	prog = (uint8_t*)mmap(NULL, dim, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if(prog == MAP_FAILED)	{
		printf("ERROR: mmap() failed\n");
		return 0;
	}
	fread(prog, 1, dim, fd);
	fclose(fd);
	mprotect(prog, dim, PROT_READ | PROT_EXEC);

	printf(SEPARATION);
	dump((void **)str, 13, "\"Hello World!\\n\"");
	printf(SEPARATION);
	dump((void **)prog, dim, buffer);
	printf(SEPARATION);

	tmp = (uintptr_t)prog;
	entry_point = 0x0;	//got the entry point visually from the objdump verbose
	                	//should be updated eachtime gcc decides to change it when recompiling the ".bin" file.
	printf("Address lenght: %u bits\n", (unsigned int)sizeof(uintptr_t) * 8);
	printf("File loaded in memory at 0x%0*X\n", (int)sizeof(uintptr_t) * 2, (unsigned int)tmp);
	printf("Entry point relative to the binary file at 0x%X\n", entry_point);
	tmp += entry_point;
	printf("Jumping at 0x%0*X to start executing...\n", (int)sizeof(uintptr_t) * 2, (unsigned int)tmp);

#if JUMP_EXECUTE_MODE == 0
	void (*func_ptr)(void) = (void (*)())tmp;
	func_ptr();
#elif JUMP_EXECUTE_MODE == 1
	//executes the program and exits with "Segmentation fault"
	//because 'goto' translates in a JMP instead of a CALL instruction
	void *ptr = (void *)tmp;
	goto *ptr;
#elif JUMP_EXECUTE_MODE == 2
	void (*func_ptr)(void) = (void (*)(void))tmp;
	func_ptr();
#elif JUMP_EXECUTE_MODE == 3
	typedef void (*func_t)(void);
	((func_t)tmp)();
#elif JUMP_EXECUTE_MODE == 4
	//executes the program and exits with "Segmentation fault"
	//because 'goto' translates in a JMP instead of a CALL instruction
	goto *(void*)prog;
#else
	asm(
		".intel_syntax;"
		"CALL %0;"
		".att_syntax;"
		: : "r"(tmp)
	);
#endif

	printf("Execution correctly returned.\n");
	printf(SEPARATION);

	munmap(prog, dim);
	return 0;
}

void print_c_program(char *file_name)	{
	int dim = 0;
	char *buffer;
	FILE *fd = fopen(file_name, "rb");

	printf("Loading %s:\n\n", file_name);

	//Can't use any standard library function like printf()
	//because that would create relocation problems:
	//we don't know where the C library is loaded in memory
	//and we don't know the offset to the printf() routine.
	//We might use the kernel's syscall write() instead
	//because it's accessed through an interrupt and doesn't
	//need to be called through relocation in legacy systems.
	//Unfortunately though, recent kernels are implementing
	//vDSO (virtual dynamic shared object), notably to
	//dynamically optimize system calls
	//(the kernel sets the VDSO to some code best for the current processor),
	//so in the assembly code, the write() syscall will be translated with
	//a CALL instruction to a vDSO like printf() with the standard C library.

	fseek(fd, 0, SEEK_END);
	dim = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	buffer = (void*)malloc(dim * sizeof(char));
	fread(buffer, 1, dim, fd);
	fclose(fd);

	printf("%s\n", buffer);
	free(buffer);
}

void dump(void **ptr, int size, char name[])	{
    int i;
    char str[0x10 * 3] = {'\0'};

    printf("Hex dump of %s:\n", name);
    printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for(i = 0; i < size; i++)      {
        if(i % 0x10 == 0 && i != 0)      printf("%s\n", str);
        if(i % 0x10 == 0)	printf("%*X:", 4, (unsigned int)i);
        sprintf(&str[(i % 0x10) * 3], " %02X", (unsigned int)(((uint8_t*)(&*ptr))[i]));
    }
    printf("%s\n", str);
}
