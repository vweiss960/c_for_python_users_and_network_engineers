# C Development Environment Setup Guide

This guide walks you through installing everything needed to compile and run C programs on your system.

---

## Windows Setup

### Option 1: MinGW (Recommended for Windows)

**MinGW** = Minimalist GNU for Windows. Gives you gcc and make on Windows.

#### Installation Steps:

1. **Download MinGW installer**
   - Visit: https://www.mingw-w64.org/downloads/
   - Download the **online installer** (mingw-w64-install.exe)

2. **Run the installer**
   - Select "Version": Latest (e.g., 13.1)
   - Select "Architecture": x86_64 (64-bit)
   - Select "Threads": posix
   - Select "Exception handling": seh
   - Click "Install"

3. **Choose installation location**
   - Default: `C:\Program Files\mingw-w64\x86_64-13.1.0-posix-seh-rt_v11-rev0`
   - Click "Next" and let it install

4. **Add to PATH**
   - Press `Win + X`, select "System"
   - Click "Advanced system settings"
   - Click "Environment Variables"
   - Under "System variables", select "Path" and click "Edit"
   - Click "New" and add: `C:\Program Files\mingw-w64\x86_64-13.1.0-posix-seh-rt_v11-rev0\bin`
   - Click "OK" multiple times

5. **Verify installation**
   - Open Command Prompt (cmd.exe) or PowerShell
   - Type: `gcc --version`
   - Should print version info (not "command not found")
   - Type: `make --version`
   - Should print version info

**Troubleshooting:**
- If gcc not found after adding to PATH, restart your computer
- The bin folder path depends on your MinGW version - adjust as needed
- If you can't find the bin folder, search for "gcc.exe" on your system

### Option 2: WSL (Windows Subsystem for Linux)

Advanced option - gives you native Linux tools on Windows.

1. **Enable WSL**
   - Open PowerShell as Administrator
   - Run: `wsl --install`
   - Restart your computer

2. **Open WSL terminal**
   - Search for "Ubuntu" in Start menu
   - Click to open Ubuntu terminal

3. **Install build tools**
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential
   ```

4. **Verify**
   ```bash
   gcc --version
   make --version
   ```

---

## macOS Setup

### Installation Steps:

1. **Install Xcode Command Line Tools**
   - Open Terminal
   - Run: `xcode-select --install`
   - Click "Install" when prompted
   - Accept the license

2. **Wait for installation**
   - This downloads about 1.2 GB
   - Takes 5-20 minutes depending on internet speed

3. **Verify installation**
   ```bash
   gcc --version
   make --version
   ```

   Should print version info for both.

**If gcc command not found:**
```bash
# This might be needed on some Macs
xcode-select --reset
xcode-select --install
```

---

## Linux Setup

### Ubuntu / Debian:

```bash
# Update package lists
sudo apt-get update

# Install build tools (includes gcc, make, and more)
sudo apt-get install build-essential

# Verify
gcc --version
make --version
```

### Fedora / CentOS / RHEL:

```bash
# Install development tools
sudo dnf groupinstall "Development Tools"

# Verify
gcc --version
make --version
```

### Arch Linux:

```bash
# Install base-devel (includes gcc and make)
sudo pacman -S base-devel

# Verify
gcc --version
make --version
```

---

## Verify Complete Setup

After installation, test everything:

### Test 1: Check gcc

```bash
gcc --version
```

**Expected output:**
```
gcc (GCC) 13.1.0
...
```

If you see a version number, gcc is installed correctly.

### Test 2: Check make

```bash
make --version
```

**Expected output:**
```
GNU Make 4.3
...
```

If you see a version number, make is installed correctly.

### Test 3: Compile a simple program

Navigate to the Hello World project:

```bash
cd "Phase_1_C_Fundamentals/Projects/00_Hello_World"
gcc hello.c -o hello
```

**Expected output:** No errors, hello executable created

### Test 4: Run it

```bash
./hello          # On Mac/Linux
hello.exe        # On Windows (if using cmd.exe)
hello            # On Windows (in PowerShell, might work)
```

**Expected output:**
```
Hello, World!
```

### Test 5: Use Make

```bash
make clean
make run
```

**Expected output:**
```
gcc -Wall -g hello.c -o hello
./hello
Hello, World!
```

If all 5 tests pass, your C development environment is completely set up!

---

## Troubleshooting Common Issues

### "gcc: command not found"

**On Windows:**
- Did you restart after adding to PATH? (required)
- Is the bin folder path correct?
- Try searching your system for gcc.exe
- Reinstall MinGW if necessary

**On Mac:**
- Run `xcode-select --install` again
- Try: `sudo xcode-select --reset`
- Check disk space (needs at least 5 GB)

**On Linux:**
- Run `sudo apt-get install build-essential` again
- Check if gcc is in PATH: `which gcc`

### "make: command not found"

**Windows with MinGW:**
- Make sure you added the bin folder to PATH (not just the root folder)
- The path should end in `\bin`
- Restart computer after changing PATH

**Mac:**
- Run `xcode-select --install` (make is included)
- Restart Terminal window and try again

**Linux:**
- Ubuntu/Debian: `sudo apt-get install make`
- Fedora: `sudo dnf install make`

### Compilation fails with weird errors

**Try this diagnostic:**
```bash
# Test basic compilation
gcc -v hello.c -o hello
```

The `-v` flag gives verbose output showing exact compiler steps. Look for errors in the output.

### Program compiles but won't run

**On Windows:**
- Make sure your file path doesn't have spaces
- Use PowerShell instead of cmd.exe
- Or move project to simpler path like `C:\Code\`

**On Mac/Linux:**
- Make sure file is executable: `chmod +x hello`
- Make sure you're in the right directory: `pwd`

---

## IDE Setup (Optional)

If you want a graphical IDE instead of command line:

### Visual Studio Code (Recommended for Beginners)

1. **Download:** https://code.visualstudio.com/
2. **Install extensions:**
   - C/C++ by Microsoft
   - Code Runner by Jun Han
   - Makefile Tools by Microsoft
3. **Open the project folder:**
   - File → Open Folder
   - Navigate to `00_Hello_World`
   - Click "Select Folder"
4. **Click the "Run" button** or press Ctrl+Alt+N

### Visual Studio Community (Free)

1. **Download:** https://visualstudio.microsoft.com/downloads/
2. **During installation, select:** "Desktop development with C++"
3. **Create new C console project**
4. **Compile and run** from the IDE

### Clion (JetBrains - Free for Students)

1. **Download:** https://www.jetbrains.com/clion/
2. **Open the project**
3. **Use built-in build/run buttons**

---

## What Each Tool Does

| Tool | What it does | Where from |
|------|--------------|-----------|
| **gcc** | C compiler (source → machine code) | MinGW/Xcode/build-essential |
| **make** | Build automation (run gcc automatically) | MinGW/Xcode/build-essential |
| **gdb** | Debugger (step through code) | MinGW/Xcode/build-essential |
| **ar** | Archive tool (create libraries) | MinGW/Xcode/build-essential |

All are included in the standard toolchain installation.

---

## Next Steps After Setup

Once everything is working:

1. **Run Hello World:** `make run` in 00_Hello_World
2. **Read HELLO_WORLD_SETUP.md** for program breakdown
3. **Move to Project 1:** Python Script Rewrite
4. **Start Phase 1 Learning:** Read PHASE_1_LEARNING_GUIDE.md

---

## Quick Reference Commands

```bash
# Compile a C file
gcc -Wall -g hello.c -o hello

# Run a program
./hello              # Mac/Linux
hello.exe            # Windows (cmd.exe)
hello                # Windows (PowerShell)

# Use Make to automate compilation
make                 # Compile
make run             # Compile and run
make clean           # Clean up

# Check tool versions
gcc --version
make --version

# Open project in VSCode
code .
```

---

## Help Resources

- **GCC Manual:** https://gcc.gnu.org/onlinedocs/
- **Make Manual:** https://www.gnu.org/software/make/manual/
- **MinGW:** https://www.mingw-w64.org/
- **Xcode:** https://developer.apple.com/xcode/
- **VS Code:** https://code.visualstudio.com/docs/languages/cpp

---

You're all set! Time to write some C! 🚀
