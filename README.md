# z80sim

POSIX command-line simulator for the fictional Z80 machine used by the 
[seedling](https://github.com/nagydani/seedling) project.

## Dependencies

This program utilizes [z80ex](https://sourceforge.net/projects/z80ex/), a
fork of Gabriel Gambetta's [libz80](https://github.com/ggambetta/libz80)
library.  It was picked primarily because it is packaged in Debian and
Ubuntu, which makes compilation easy.  The alternatives are compared
below.

## Building z80sim

The utility is built using the following command:

```sh
$ sudo apt-get install libz80ex-dev
$ make
```

or directly:

```sh
$ gcc z80sim.c -o z80sim -lz80ex
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

## Z80 emulation libraries

`z80sim` only needs memory and port callbacks, a halt indication, and
instruction stepping, so several C libraries can drive it.  Their names
are easy to confuse, so here is what each one actually is.

| Name | Source repository | Official Debian package | Official Ubuntu package |
|---|---|---|---|
| `libz80` | [ggambetta/libz80](https://github.com/ggambetta/libz80) | none | none |
| `z80ex` | [SourceForge project](https://sourceforge.net/projects/z80ex/) (SVN); [GitHub mirror](https://github.com/lipro/z80ex) | [`libz80ex1`](https://packages.debian.org/trixie/libz80ex1), [`libz80ex-dev`](https://packages.debian.org/trixie/libz80ex-dev) | same names in universe ([source](https://launchpad.net/ubuntu/+source/z80ex)) |
| `Z80` | [redcode/Z80](https://github.com/redcode/Z80) | none | none |

The names mean:

* `libz80` is Gabriel Gambetta's original library.  It builds
  `libz80.so` and installs `z80.h` with a lowercase name.  It is not
  packaged by Debian or Ubuntu.  The same name is also used by the
  vendor package for `Z80` below, which is the main source of confusion.

* `z80ex` is a derived library by "boo_boo", with code from FUSE.  Its
  Debian source package is
  [`z80ex`](https://packages.debian.org/source/trixie/z80ex); the
  binary packages are `libz80ex1` and `libz80ex-dev`.  It installs
  `<z80ex/z80ex.h>` and links with `-lz80ex`.  It is in official Debian
  main and Ubuntu universe.  This is the library `z80sim` uses.

* `Z80` is Manuel Sainz de Baranda's independent library.  It installs
  `<Z80.h>`, links with `-lZ80`, and needs
  [Zeta](https://github.com/redcode/Zeta), a header-only portability
  library.  It is not in official Debian or Ubuntu.  The vendor's
  [zxe repository](https://github.com/redcode/Z80/wiki/Installation)
  provides it as `libz80` and `libz80-dev`; the installed library is
  `libZ80` with a capital Z.

### Tradeoffs

* **libz80** is the ancestor.  It is small and readable, covers the
  documented instruction set plus undocumented flags and indexed
  opcodes, and includes a disassembler (`Z80Debug`).  It does not model
  MEMPTR, Q, or the special RESET.  Upstream's last commit is from 2022
  (version 2.1.1), and it has no distribution package.  In practice
  `z80ex` supersedes it.

* **z80ex** is the easiest to install on a Debian or Ubuntu development
  machine, because it is in the official archives.  It adds MEMPTR,
  exact per-instruction T-state timings including I/O, all interrupt
  modes, a per-T-state callback, register access, and a separate
  disassembler.  It still does not model Q or the special RESET, and
  upstream's last release, 1.1.21, is from 2013.  It is GPL-2+, so a
  binary linked with it must be distributed under the GPL; `z80sim`
  itself is public domain, which is a licensing change worth knowing
  about.

* **Z80** is the accuracy and maintenance choice.  It models MEMPTR, Q,
  the special RESET, full interrupt mode 0, and model-specific bugs,
  and its documentation reports passing the major Z80 conformance
  suites.  It is LGPL-3.0-or-later, so linking does not impose the GPL
  on `z80sim`.  The cost is packaging: it is absent from official
  Debian and Ubuntu, needs Zeta, and uses CMake or pkg-config.
