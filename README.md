# z80sim

POSIX command-line simulator for the fictional Z80 machine used by the 
[seedling](https://github.com/nagydani/seedling) project.

## Dependencies

This program utilizes a fork of Gabriel Gambetta's
[libz80](https://github.com/ggambetta/libz80) library, namely
[z80ex](https://sourceforge.net/projects/z80ex/).

## Building z80sim

The utility is built using the following command:
```
$ sudo apt-get install libz80ex-dev
$ gcc z80sim.c -o z80sim -lz80
```

## Usage

The executable takes a single filename argument for the file to be loaded into the RAM of the 
simulated machine, beginning with address `0`. The rest of the RAM is filled with `0x76`, the 
opcode for the [HALT](http://z80-heaven.wikidot.com/instructions-set:halt) instruction. Upon 
executing this opcode, the simulator exits with 0 status code. Reads and writes using 
[IN](http://z80-heaven.wikidot.com/instructions-set:in) and 
[OUT](http://z80-heaven.wikidot.com/instructions-set:out) instructions from and to port `0` 
allow reading and writing the standard input and output of the host operating system, one byte 
at a time.

After initializing the RAM, the simulated Z80 processor is reset, beginning execution at `0`. 
It never receives any interrupts.
