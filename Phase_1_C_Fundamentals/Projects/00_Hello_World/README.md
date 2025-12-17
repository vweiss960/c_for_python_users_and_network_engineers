# Project 0: Hello World - Setup Verification

## What This Is

The simplest possible C program. Use this to verify that your C compiler and development environment are properly installed and working.

## Files

- **hello.c** - The Hello World source code (4 lines!)
- **Makefile** - Build automation
- **README.md** - This file

## Quick Start

### Step 1: Compile and Run

```bash
make run
```

### Expected Output

```
Hello, World!
```

**If you see this, your C setup is working!**

---

## If It Doesn't Work

### Error: "make: command not found"

**Solution:** Install Make
- **Windows**: Install MinGW or use WSL (Windows Subsystem for Linux)
- **Mac**: Install Xcode Command Line Tools: `xcode-select --install`
- **Linux**: Install build-essential: `sudo apt-get install build-essential`

### Error: "gcc: command not found"

**Solution:** Install GCC (C Compiler)
- **Windows**: Install MinGW from https://www.mingw-w64.org/
- **Mac**: Install Xcode Command Line Tools: `xcode-select --install`
- **Linux**: Install build-essential: `sudo apt-get install build-essential`

### Error: "Permission denied"

**Solution:** On Mac/Linux, make the executable runnable:
```bash
chmod +x hello
./hello
```

### Error: Something else?

Try compiling manually to see the actual error:
```bash
gcc hello.c -o hello
```

This shows the exact compiler error message.

---

## What's in the Code?

```c
#include <stdio.h>      // Include standard input/output library

int main() {            // Main function (entry point)
    printf("Hello, World!\n");  // Print text with newline
    return 0;           // Return success (0)
}
```

**Line by line:**
- `#include <stdio.h>` - Include the standard I/O library (needed for printf)
- `int main()` - Main function where program execution starts
- `printf("Hello, World!\n");` - Print "Hello, World!" followed by newline
- `return 0;` - Exit successfully

---

## Build Process

When you run `make run`, here's what happens:

```
1. Check if hello.c is newer than hello executable
2. If yes, run: gcc -Wall -g hello.c -o hello
3. Then run: ./hello
4. Output: Hello, World!
```

The flags mean:
- `-Wall`: Show all warnings (helps catch mistakes)
- `-g`: Include debug symbols (for gdb debugger)
- `-o hello`: Output filename is "hello"

---

## Verify Your Setup Completely

Run these commands to check each component:

```bash
# Check gcc
gcc --version

# Check make
make --version

# Check compilation works
make clean
make

# Check it runs
make run

# Clean up
make clean
```

If all of these work, you're fully set up!

---

## Next Steps

Once this works:
1. **Read** `HELLO_WORLD_SETUP.md` for detailed setup info
2. **Try** Project 1: Python Script Rewrite
3. **Experiment:** Modify this program to print your name

### Simple Modifications to Try

**Print your name:**
```c
printf("Hello, My Name Is Vincent!\n");
```

**Print multiple lines:**
```c
printf("Hello, World!\n");
printf("This is my first C program!\n");
printf("It's working!\n");
```

**Print a simple pattern:**
```c
printf("*\n");
printf("**\n");
printf("***\n");
```

---

## Troubleshooting Checklist

- [ ] `make --version` shows version number
- [ ] `gcc --version` shows version number
- [ ] `make run` compiles without errors
- [ ] `make run` prints "Hello, World!"
- [ ] `make clean` removes the executable
- [ ] `make` rebuilds the executable

If all of these pass, you're ready for real C programming!

---

## Quick Commands Reference

```bash
make              # Compile hello.c → hello executable
make run          # Compile (if needed) and run
make clean        # Delete hello executable
make help         # Show available commands
```

That's it! This is the simplest C program you'll ever write, but it's the gateway to everything else.

Good luck! 🚀
