# kshell

A small Unix shell written in C++. It reads commands, resolves programs on `PATH` (or by path), splits arguments, and runs them with `fork` / `execv` / `wait`. The CMake target builds an executable named **`ksh`**.

## Requirements

- Unix-like OS (Linux, *BSD, macOS with POSIX APIs)
- C++17 compiler
- CMake 3.22 or newer

## Build

```bash
cmake -S . -B build
cmake --build build
```

The binary is `build/ksh`.

## Usage

**Interactive shell** (colored `username hostname <` prompt):

```bash
./build/ksh
```

**Single command** (non-interactive, then exit):

```bash
./build/ksh -c "ls -la"
```

Type `exit` or press Ctrl+D to leave interactive mode.


## Limitations

No pipes, redirection, globs, environment-variable expansion, or job control. Only simple space-separated words; quoting is not supported. The builtin command set is minimal (`exit` plus EOF handling).
