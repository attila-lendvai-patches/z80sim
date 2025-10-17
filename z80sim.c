#include <errno.h>
#include <z80.h>
#include <stdio.h>

#define STDIOPORT 0x00
#define HALT 0x76

static byte memory[ 0x10000 ];

static byte getChar(size_t param, ushort address) {
	return ((address & 0xFF) != STDIOPORT) ? 0 : (byte)getchar();
}

static void putChar(size_t param, ushort address, byte data) {
	if ((address & 0xFF) == STDIOPORT) {
		putchar(data);
		fflush(stdout);
	}
}

static byte getMem(size_t param, ushort address) {
	return memory[address];
}

static void putMem(size_t param, ushort address, byte data) {
	memory[address] = data;
}

static Z80Context context;

int main(int argc, char *argv[]) {
	FILE *f; int i, c;
	if(argc !=2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		return -1;
	}
	if((f = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "%s not found\n", argv[1]);
		return errno;
	}
	for (i = 0; (i < 0x10000) && ((c = getc(f)) != EOF); i++)
            memory[i] = (byte)c;
        fprintf(stderr, "read %d bytes to memory\n", i);
	while(i < 0x10000) memory[i++] = HALT;
	fclose(f);
	context.memRead = getMem;
	context.memWrite = putMem;
	context.ioRead = getChar;
	context.ioWrite = putChar;
        fprintf(stderr, "reset\n");
	Z80RESET(&context);
	while(!context.halted)
		Z80Execute(&context);
	fflush(stdout);
        fprintf(stderr, "returning normally\n");
	return 0;
}
