# bf2c

A Brainfuck to C transpiler

![](https://github.com/BigPeet/bf2c/actions/workflows/cmake-multi-platform.yml/badge.svg)

## What?

*[Brainfuck is an esoteric programming language created in 1993 by Swiss student Urban Müller.](https://en.wikipedia.org/wiki/Brainfuck)*
This is project is a [transpiler](https://en.wikipedia.org/wiki/Source-to-source_compiler) which translates Brainfuck code to equivalent C code.

The implementation is based on [this reference](https://brainfuck.org/brainfuck.html) on [brainfuck.org](https://brainfuck.org).

## Why?

Firstly, because I find Brainfuck hard to read.
Straight up compiling it to look at the resulting machine code or assembly does not make things easier.
Transpiling it into C helps me to read (not necessarily understand) Brainfuck programs more easily.

Secondly, because I thought is was a fun little exercise.
I used this project as motivation, justification and testbed for developing some general functionalities for me in C which I might use in different projects as well.
(This is also the reason why this project is more complex than it needs to be.)

## How ...

### ... install?

Besides the usual system libraries (libc et al.), bf2c is a standalone executable.
Simply download a [release](https://github.com/BigPeet/bf2c/releases) for your platform, put it where you want it and you are good to go.

### ... to use?

You can simply run bf2c as CLI tool:

```bash
# transpile hello.b and write output into hello.c
bf2c hello.b -o hello.c

# read from stdin and write to stdout
echo ">+++++++++[<++++++++>-]<.+.>++++++++++." | bf2c

# immediately compile and run the program
echo ">+++++++++[<++++++++>-]<.+.>++++++++++." | bf2c | gcc -xc - && ./a.out

# For more options, see "help"
bf2c --help
```

### ... to build?

This project is setup using [cmake](https://cmake.org/).
In addition to that, you should have a C toolchain installed.

```bash
git clone https://github.com/BigPeet/bf2c.git
cd bf2c
cmake . -B build      # configure
cmake --build build   # build
cmake --install build # optional install step, likely requires sudo
```

The built executable should be located in the `./build/app` directory.
