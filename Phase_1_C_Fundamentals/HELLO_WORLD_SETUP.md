# Hello World: Your First C Program in VSCode

This lesson walks you through writing, compiling, and running your first C program using VSCode. By the end, you'll have a working development environment.

---

## Prerequisites

### 1. Install a C Compiler

You need a C compiler to turn your source code into a runnable program.

**Windows:**
- Download and install **MinGW** (Minimalist GNU for Windows)
- Visit: https://www.mingw-w64.org/
- Download the installer and run it
- During installation, ensure you select `gcc` and `make`
- Add MinGW to your PATH (the installer can do this)
- Verify installation:
  ```bash
  gcc --version
  ```

**macOS:**
- Install Xcode Command Line Tools:
  ```bash
  xcode-select --install
  ```
- Verify:
  ```bash
  gcc --version
  ```

**Linux (Ubuntu/Debian):**
- Install build essentials:
  ```bash
  sudo apt-get install build-essential
  ```
- Verify:
  ```bash
  gcc --version
  ```

### 2. Install VSCode Extensions

Open VSCode and install these extensions (search in the Extensions tab):

1. **C/C++ by Microsoft** - Syntax highlighting and IntelliSense
2. **Code Runner by Jun Han** - Easy run button in editor
3. **Makefile Tools by Microsoft** - For managing builds later

---

## Step 1: Create Your First C Program

### 1.1 Create a project folder

Create a new directory for your C learning program:
```bash
mkdir ~/C_Learning
cd ~/C_Learning
```

### 1.2 Open VSCode

```bash
code .
```

This opens VSCode in your project folder.

### 1.3 Create a new file

In VSCode:
- Click **File → New File**
- Name it `hello.c`

### 1.4 Write your first program

Type this code into `hello.c`:

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

**Explanation:**
- `#include <stdio.h>` - Include the Standard Input/Output library (needed for `printf`)
- `int main()` - The entry point of your program (execution starts here)
- `printf()` - Print formatted text to the console
- `return 0;` - Return 0 to the operating system (0 = success)

Save the file (Ctrl+S or Cmd+S).

---

## Step 2: Compile Your Program

### Method 1: Using VSCode's Code Runner (Easiest)

1. Right-click in the editor and select **Run Code**
2. VSCode will compile and run automatically
3. The output appears in the terminal at the bottom:
   ```
   Hello, World!
   ```

**What happened:**
- VSCode called `gcc hello.c -o hello`
- This compiled `hello.c` into an executable named `hello`
- Then it ran `./hello`

### Method 2: Using the Terminal (Manual, Better Understanding)

1. Open the integrated terminal: **View → Terminal** (or Ctrl+`)
2. Make sure you're in the directory with `hello.c`:
   ```bash
   pwd  # Shows current directory
   ```
3. Compile:
   ```bash
   gcc hello.c -o hello
   ```

**Breakdown:**
- `gcc` - The C compiler
- `hello.c` - Your source file
- `-o hello` - Output name (the executable)

4. Check what was created:
   ```bash
   ls
   ```
   You should see: `hello` and `hello.c`

5. Run the program:
   ```bash
   ./hello        # On macOS/Linux
   hello.exe      # On Windows (if using MinGW)
   ```

6. Output:
   ```
   Hello, World!
   ```

---

## Step 3: Understanding the Build Process

When you ran `gcc hello.c -o hello`, here's what happened:

```
┌─────────────┐
│  hello.c    │  (Source code)
│ (text file) │
└──────┬──────┘
       │
       ▼
    [gcc compiler]
       │
       ├─→ Preprocessing (#include, #define expansion)
       ├─→ Compilation (convert to assembly)
       ├─→ Assembly (convert to object code)
       └─→ Linking (combine with libraries)
       │
       ▼
┌─────────────┐
│ hello.exe   │  (Executable - machine code)
│(binary file)│
└─────────────┘
```

After compilation, `hello.c` is no longer needed to run the program. Only `hello` (the executable) is required.

---

## Step 4: Modify and Recompile

Let's make sure you can iterate quickly.

### 4.1 Modify the program

Change `hello.c` to:

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    printf("Welcome to C programming!\n");
    int age = 30;
    printf("I am %d years old.\n", age);
    return 0;
}
```

**New concepts:**
- `int age = 30;` - Declare an integer variable
- `%d` - Format specifier for integers (placeholder)

### 4.2 Recompile

```bash
gcc hello.c -o hello
```

### 4.3 Run

```bash
./hello
```

Output:
```
Hello, World!
Welcome to C programming!
I am 30 years old.
```

---

## Step 5: Understanding Errors

Let's intentionally create an error to see how the compiler helps you.

### 5.1 Introduce a mistake

Change your code to (note the missing semicolon):

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n")  // Missing semicolon!
    return 0;
}
```

### 5.2 Try to compile

```bash
gcc hello.c -o hello
```

You'll see an error:
```
hello.c: In function 'main':
hello.c:4:5: error: expected ';' before 'return'
    4 |     return 0;
      |     ^~~~~~~
```

**The compiler is helping you!** It tells you:
- Line 4, column 5
- What it expected (`;`)
- What it found instead (`return`)

### 5.3 Fix the error

Add the semicolon:
```c
printf("Hello, World!\n");  // Fixed!
```

Recompile:
```bash
gcc hello.c -o hello
```

Success! No errors.

---

## Step 6: Compiler Warnings

Sometimes the compiler will warn you about potential issues. These are different from errors (the program still compiles).

### 6.1 Create a warning

```c
#include <stdio.h>

int main() {
    int x;  // Variable declared but never used
    printf("Hello, World!\n");
    return 0;
}
```

### 6.2 Compile with warnings enabled

```bash
gcc -Wall hello.c -o hello
```

You'll see:
```
hello.c: In function 'main':
hello.c:4:9: warning: unused variable 'x' [-Wunused-variable]
    4 |     int x;
      |         ^
```

**Warnings are important!** They often point to bugs you didn't notice.

### 6.3 Fix the warning

Either use the variable or remove it:
```c
#include <stdio.h>

int main() {
    int x = 42;
    printf("The answer is %d\n", x);
    return 0;
}
```

Recompile:
```bash
gcc -Wall hello.c -o hello
./hello
```

---

## Step 7: VSCode Configuration for Smooth Development

### 7.1 Create a `.vscode/settings.json` file

This tells VSCode and Code Runner how to compile your C programs.

1. Create a `.vscode` folder in your project directory
2. Inside, create `settings.json`:

```json
{
    "code-runner.executorMap": {
        "c": "cd $dir && gcc -Wall -g $fileName -o $fileNameWithoutExt && ./$fileNameWithoutExt"
    },
    "code-runner.runInTerminal": true
}
```

**What this does:**
- `-Wall` - Show all warnings
- `-g` - Include debug info (for debugging with gdb)
- `$dir` - Current directory
- `$fileName` - Current file name
- `$fileNameWithoutExt` - File name without extension

### 7.2 Create a `Makefile` for larger projects

For now, create a simple one:

```makefile
CC = gcc
CFLAGS = -Wall -g

hello: hello.c
	$(CC) $(CFLAGS) hello.c -o hello

run: hello
	./hello

clean:
	rm hello
```

Now you can:
```bash
make hello        # Compile
make run          # Compile and run
make clean        # Delete executable
```

---

## Step 8: Debugging with GDB (Optional but Recommended)

GDB (GNU Debugger) lets you step through your code line by line.

### 8.1 Compile with debug info

```bash
gcc -g hello.c -o hello
```

The `-g` flag includes debugging information.

### 8.2 Start the debugger

```bash
gdb ./hello
```

You'll see the GDB prompt:
```
(gdb)
```

### 8.3 Basic GDB commands

```bash
(gdb) break main          # Set breakpoint at main function
(gdb) run                 # Run program
(gdb) next                # Execute next line
(gdb) step                # Step into function
(gdb) print x             # Print variable value
(gdb) continue            # Continue execution
(gdb) quit                # Exit debugger
```

**Example session:**
```
(gdb) break main
Breakpoint 1 at 0x401104
(gdb) run
Starting program: /home/user/hello
Breakpoint 1, main () at hello.c:4
4	    printf("Hello, World!\n");
(gdb) next
Hello, World!
5	    return 0;
(gdb) quit
```

---

## Quick Reference: Compilation Workflow

### Scenario 1: Single File Program

```bash
# Compile
gcc hello.c -o hello

# Run
./hello

# Clean up
rm hello
```

### Scenario 2: Program with Warnings Enabled

```bash
# Compile with warnings
gcc -Wall hello.c -o hello

# Run
./hello
```

### Scenario 3: Debugging

```bash
# Compile with debug info
gcc -g hello.c -o hello

# Debug
gdb ./hello

# Or run normally
./hello
```

### Scenario 4: Optimization

```bash
# Compile with optimization (for production)
gcc -O2 hello.c -o hello

# Run
./hello
```

**Common flags:**
- `-Wall` - Enable all warnings
- `-g` - Include debug information
- `-O2` - Optimize for speed
- `-o <name>` - Output filename
- `-c` - Compile without linking (creates .o file)

---

## Your First Program Checklist

- [ ] Installed a C compiler (gcc)
- [ ] Opened VSCode with your project folder
- [ ] Created `hello.c` with the basic program
- [ ] Successfully compiled with `gcc hello.c -o hello`
- [ ] Successfully ran `./hello`
- [ ] Modified the program and recompiled
- [ ] Understood compiler error messages
- [ ] Configured VSCode with `.vscode/settings.json`
- [ ] Created a `Makefile`
- [ ] (Optional) Used GDB to debug

---

## Troubleshooting

### Problem: "gcc: command not found"
**Solution:** Your compiler isn't installed or not in your PATH.
- Reinstall your C compiler
- Make sure it's added to your system PATH
- Restart VSCode after installation

### Problem: "Permission denied" when running
**Solution:** On macOS/Linux, add execute permission:
```bash
chmod +x hello
./hello
```

### Problem: "file not found" error
**Solution:** Make sure you're in the right directory:
```bash
pwd  # Check where you are
ls   # List files in current directory
cd ~/C_Learning  # Navigate to correct folder
```

### Problem: VSCode doesn't recognize C syntax
**Solution:** Install the C/C++ extension by Microsoft:
1. Open Extensions (Ctrl+Shift+X)
2. Search for "C/C++"
3. Click Install

### Problem: Code Runner doesn't work
**Solution:** Check that your compiler works manually:
```bash
gcc --version
```
If it doesn't work, reinstall your compiler.

---

## Next Steps

Now that you have a working development environment:

1. **Experiment:** Modify `hello.c` and see what happens
2. **Try the projects:** Start with Project 1 (Python Script Rewrite)
3. **Learn more:** Read more about C syntax in PHASE_1_LEARNING_GUIDE.md
4. **Use the debugger:** GDB will save you hours when things go wrong

Congratulations! You've written, compiled, and run your first C program. You're officially a C programmer! 🎉

---

## Additional Resources

- **GCC Manual:** https://gcc.gnu.org/onlinedocs/
- **GDB Tutorial:** https://www.sourceware.org/gdb/documentation/
- **Makefile Tutorial:** https://makefiletutorial.com/
- **VSCode C Setup:** https://code.visualstudio.com/docs/languages/cpp

