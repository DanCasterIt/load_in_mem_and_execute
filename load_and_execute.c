#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//typedef void (*func_t)(void);

void generate_c_program();

int main(int argc, char *argv[])	{
	generate_c_program();

	system("gcc hello.c -c -Wl,--oformat=binary");
	system("objcopy -O binary -j .text hello.o hello.bin");
	system("objdump -d hello.o");

	//system("gcc -nostdinc -m32 -masm=intel -Wall -c hello.c -o hello.o");
	//system("objdump --disassemble --disassembler-options intel hello.o");
	//system("objcopy --only-section=.text --output-target binary hello.o hello.bin");

	FILE *fd = fopen("hello.bin", "rb");
	fseek(fd, 0, SEEK_END);
	int dim = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	printf(">>>>>>>>>>>>>>>>>>>>>>hex dump of hello.bin>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("Binary file size is: %d bytes\n", dim);
	uint8_t *prog = (uint8_t*)malloc(dim);
	//uint8_t prog[dim];
	fread(prog, 1, dim, fd);
	fclose(fd);
	int i;
	for(i = 0; i < dim; i++)	{
		printf("%02X", (unsigned int)prog[i]);
		if((i + 1) % 0xF) printf(" ");
		else printf("\n");
	}
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<hex dump of hello.bin<<<<<<<<<<<<<<<<<<<<<<<\n");
	uintptr_t tmp = (uintptr_t)prog;
	printf("Address lenght: %ld bits\n", sizeof(uintptr_t) * 8);
	printf("File loaded in memory at 0x%08X\n", (unsigned int)tmp);
	printf("Trying to jump and execute...\n");
	
	//void (*foo)(void) = (void (*)())tmp;
	//foo();
	
	//void *ptr = (void *)tmp;
	//goto *ptr;

	//unsigned long address=tmp; 
	//void (*func_ptr)(void) = (void (*)(void))address;
	//func_ptr();
	
	//((func_t)tmp)();
	
	//goto *(void*)prog;

	uintptr_t input, output = 0;
	input = tmp;
	printf("input  = 0x%08X, output = 0x%08X\n", input, output);
	asm(
		".intel_syntax;"
		"MOV EAX, %1;"
		//"JMP EAX;"
		"MOV %0, EAX;"
		".att_syntax;"
		: "=r"(output)
		: "r"(input)
		: "eax"
	);
	printf("input  = 0x%08X, output = 0x%08X\n", input, output);

	free(prog);
	return 0;
}

void generate_c_program()	{
	FILE *fd = fopen("hello.c", "w");
	fprintf(fd, "#include <stdio.h>\n\n");
	fprintf(fd, "int main(int argc, char *argv[])\t{\n");
	fprintf(fd, "\tprintf(\"Hello, world\\n\");\n");
	fprintf(fd, "\treturn 0;\n}");
	fclose(fd);
}
