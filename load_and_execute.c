#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>

#define JUMP_EXECUTE_MODE 0

int main(int argc, char *argv[])	{
	if(argc < 2)	{
		printf("No input binary file specified to be loaded and executed.\n");
		return -1;
	}

	FILE *fd = fopen(argv[1], "rb");
	if(!fd)	{
		printf("Couldn't find a file named %s\n", argv[1]);
		return -1;
	}

	// Get the file size
	fseek(fd, 0, SEEK_END);
	int dim = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	// Allocate some executable memory using mmap():
	// PROT_EXEC = executable memory
	// NULL = allocate memory
	uint8_t *prog = (uint8_t*)mmap(NULL, dim, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if(prog == MAP_FAILED)	{
		printf("ERROR: mmap() failed\n");
		return 0;
	}

	// Load the file content into the mapped memory
	fread(prog, 1, dim, fd);
	fclose(fd);

	// Change mapped memory permissions
	mprotect(prog, dim, PROT_READ | PROT_EXEC);

	uintptr_t tmp = (uintptr_t)prog;
	int entry_point = 0x0;  // Got the entry point visually from the objdump verbose.
	                        // It should be updated each time gcc decides to change it when recompiling the ".bin" file.
	                        // If using the link.ls linker script, the .text.main section will always be put at the beginning
	                        // of the executable, so the entrypoint is always 0.

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
	munmap(prog, dim);
	return 0;
}
