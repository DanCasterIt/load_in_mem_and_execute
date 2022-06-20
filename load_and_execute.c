#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>

#define JUMP_EXECUTE_MODE 0
#define COMPILE_MODE 1
#define SEPARATION "-----------------------------------------------------------\n"

void generate_c_program(char *file_name);
void dump(void **ptr, int size, char name[]);

int main(int argc, char *argv[])	{
	char program_name[] = "simple_c_program";
	char buffer[39 + strlen(program_name)];
	char str[] = "Hello World!\n";
	int dim;
	FILE *fd;
	uint8_t *prog;
	uintptr_t tmp;

	strcpy(buffer, program_name);
	strcat(buffer, ".c");
	printf(SEPARATION);
	generate_c_program(buffer);
	printf(SEPARATION);

#if COMPILE_MODE == 0
	sprintf(buffer, "gcc %s.c -c -Wl,--oformat=binary", program_name);
	system(buffer);
	sprintf(buffer, "objcopy -O binary -j .text %s.o %s.bin", program_name, program_name);
	system(buffer);
	sprintf(buffer, "objdump -d %s.o", program_name);
	system(buffer);
#else
	sprintf(buffer, "gcc -masm=intel -Wall -c %s.c -o %s.o", program_name, program_name);
	system(buffer);
	sprintf(buffer, "objdump --disassemble --disassembler-options intel %s.o", program_name);
	system(buffer);
	sprintf(buffer, "objcopy --only-section=.text --output-target binary %s.o %s.bin", program_name, program_name);
	system(buffer);
#endif

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
	printf("Address lenght: %u bits\n", (unsigned int)sizeof(uintptr_t) * 8);
	printf("File loaded in memory at 0x%0*X\n", (int)sizeof(uintptr_t) * 2, (unsigned int)tmp);
	printf("Executing the program...\n");

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

void generate_c_program(char *file_name)	{
	int dim = 0;
	char buffer[200] = {'\0'};
	FILE *fd = fopen(file_name, "w+");

	printf("Generating %s:\n\n", file_name);

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
	fprintf(fd, "#include <stdio.h>\n");
	fprintf(fd, "#include <unistd.h>\n\n");
	fprintf(fd, "int main(int argc, char *argv[])\t{\n");
	fprintf(fd, "\tchar str[] = \"Hello World!\\n\";\n");
	fprintf(fd, "\twrite(1, str, 13);\n"); //translates into something like "call   4f <main+0x4f>"
	fprintf(fd, "\treturn 0;\n");
	fprintf(fd, "}");

	dim = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	fread(buffer, 1, dim, fd);

	printf("%s\n", buffer);
	fclose(fd);
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
