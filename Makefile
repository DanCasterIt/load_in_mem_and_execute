CC=gcc

LOADER_NAME := load_and_execute
BLOB_NAME := assembly_syscall

BLOB_LINKER_SCRIPT := link2.ls
BLOB_OPTIONS := -ffunction-sections -fno-asynchronous-unwind-tables -Qn
BLOB_LINKER_OPTIONS := -T ${BLOB_LINKER_SCRIPT} -nostdlib -Wl,--oformat=binary

all: loader binaryBlob

${LOADER_NAME}.o: ${LOADER_NAME}.c
	$(CC) -c -o $@ $<

${BLOB_NAME}.o: ${BLOB_NAME}.c
	$(CC) -c -o $@ $< ${BLOB_OPTIONS} ${BLOB_LINKER_OPTIONS}

loader: ${LOADER_NAME}.o
	$(CC) -o ${LOADER_NAME} ${LOADER_NAME}.o

binaryBlob: ${BLOB_NAME}.o
	$(CC) -o ${BLOB_NAME}.bin ${BLOB_NAME}.o ${BLOB_OPTIONS} ${BLOB_LINKER_OPTIONS}

dumpBlob:
	objdump -D --disassembler-options intel ${BLOB_NAME}.o
	@echo -----Sections merged together in a single binary blob------
	objdump -D -Mintel,x86-64 -b binary -m i386 ${BLOB_NAME}.bin

.PHONY: clean

clean: cleanLoader cleanBinaryBlob

cleanLoader:
	rm -f ${LOADER_NAME} ${LOADER_NAME}.o

cleanBinaryBlob:
	rm -f ${BLOB_NAME}.bin ${BLOB_NAME}.o
