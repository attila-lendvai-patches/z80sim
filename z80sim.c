#include <errno.h>
#include <z80ex/z80ex.h>
#include <stdio.h>

#define STDIOPORT 0x00
#define HALT 0x76

static Z80EX_BYTE memory[0x10000];

// Memory callbacks
static Z80EX_BYTE getMem(Z80EX_CONTEXT* cpu, Z80EX_WORD address, int m1, void *user) {
    return memory[address];
}

static void putMem(Z80EX_CONTEXT* cpu, Z80EX_WORD address, Z80EX_BYTE value, void *user) {
    memory[address] = value;
}

// I/O callbacks
static Z80EX_BYTE getChar(Z80EX_CONTEXT* cpu, Z80EX_WORD port, void *user) {
    return ((port & 0xFF) != STDIOPORT) ? 0 : (Z80EX_BYTE)getchar();
}

static void putChar(Z80EX_CONTEXT* cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user) {
    if ((port & 0xFF) == STDIOPORT) {
        putchar(value);
        fflush(stdout);
    }
}

// INT callback (not used here, return 0)
static Z80EX_BYTE intRead(Z80EX_CONTEXT* cpu, void *user) {
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *f;
    int i, c;

    if(argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return -1;
    }

    if((f = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "%s not found\n", argv[1]);
        return errno;
    }

    for(i = 0; (i < 0x10000) && ((c = getc(f)) != EOF); i++)
        memory[i] = (Z80EX_BYTE)c;

    fprintf(stderr, "read %d bytes to memory\n", i);

    while(i < 0x10000)
        memory[i++] = HALT;
    fclose(f);

    // Create CPU with all required callbacks
    Z80EX_CONTEXT* cpu = z80ex_create(
        getMem, NULL,     // mem read
        putMem, NULL,     // mem write
        getChar, NULL,    // port read
        putChar, NULL,    // port write
        intRead, NULL     // INT read
    );

    z80ex_reset(cpu);

    // Execute instructions
    while(!z80ex_doing_halt(cpu))
        z80ex_step(cpu);

    fflush(stdout);
    z80ex_destroy(cpu);
    fprintf(stderr, "returning normally\n");
    return 0;
}
