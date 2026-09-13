# CMono

A C monorepo for my personal projects

## Installation and Setup

### Dependences

- Required
    - C Compiler
        - Supported Compilers are: **Clang**, **GCC**, **MSVC** (Windows only)
    - **Linux only:** XCB libraries (for GUI/windowing)
- Optional
    - **MinGW**: For cross-compiling Windows binaries on Linux
    - **Wine**: For running Windows builds on Linux during development/testing

### Building

- Compile the build tool:
```sh
clang build.c
```
- Build the project:
```sh
./a.out build # For Linux
./a.exe build # For Windows
```
- For more build system options/help:
```sh
./a.out --help # For Linux
./a.exe --help # For Windows
```

### Running

- Building and Running the program:
```sh
./a.out build-run  # For Linux
./a.exe build-run  # For Windows
```

## Reference

### Lib

- [stb](https://github.com/nothings/stb)
- [gb](https://github.com/gingerBill/gb)
- [Handmade Math](https://github.com/HandmadeMath/HandmadeMath)
- [kgflags]( http://github.com/kgabis/kgflags)
- [RGFW](https://github.com/ColleagueRiley/RGFW)
- [nob.h](https://github.com/tsoding/nob.h)

### Projects

- [Handmade Hero](https://github.com/cj1128/handmade-hero)
- [RAD Debugger](https://github.com/EpicGamesExt/raddebugger)
- [4coder](https://github.com/4coder-archive/4coder)

### Guide

- [docs.gl](https://docs.gl/)

### Articles/Posts

- [Pre-defined C/C++ Compiler Macros](https://github.com/cpredef/predef)
- [The Arena - Custom Memory Allocators in C](https://www.bytesbeneath.com/p/the-arena-custom-memory-allocators)

## Examples

- [OpenGL Xlib Example](https://github.com/vbsw/opengl-xlib-example)
- [Create an OpenGL context using Xlib and EGL](https://gist.github.com/pedrominicz/2d81559c5fb66d23d6bb627570956605)


