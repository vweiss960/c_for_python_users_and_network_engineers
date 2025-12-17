# Project 1: Python Script Rewrite - Quick Start Guide

## What This Project Is

A beginner-friendly exercise that translates a simple Python text analysis program into C. You'll learn:
- How to read files in C
- String and character handling
- Basic data structures (structs)
- Function design

## Files

| File | Purpose |
|------|---------|
| `PROJECT_GUIDE.md` | Detailed project explanation and learning goals |
| `01_python_solution.py` | Working Python version (reference) |
| `01_starter.c` | C template with TODOs (start here) |
| `01_c_solution.c` | Complete C solution |
| `01_c_solution_explained.md` | Line-by-line explanation of C code |
| `sample_input.txt` | Test data for your program |
| `Makefile` | Build automation |
| `README.md` | This file |

## Quick Start

### Step 1: See What's Expected

```bash
# Run Python version to see the expected output
python3 01_python_solution.py sample_input.txt
```

**Output:**
```
=== Text Statistics ===
File: sample_input.txt
Total characters: 224
Total words: 35
Total lines: 5
Longest word: successfully (12 characters)
```

### Step 2: Try Writing Your Own C Version

Open `01_starter.c` and implement the three TODO functions:
1. `extract_word()` - Extract a word from a line
2. `process_line()` - Process a line to count words
3. `count_text_statistics()` - Read file and gather stats

**Hints in the starter file** explain what each function should do.

### Step 3: Compile and Test

```bash
# Compile
gcc -Wall -g 01_starter.c -o word_counter

# Test
./word_counter sample_input.txt
```

### Step 4: Check Your Work

Compare your output with the Python version:

```bash
make compare
```

### Step 5: Study the Solution

If you get stuck, read `01_c_solution_explained.md` for detailed explanations of:
- File I/O in C
- String handling
- Why C requires certain patterns
- Common pitfalls

## Using the Makefile

```bash
make              # Build the program
make run          # Build and run with sample file
make python_run   # Run Python version for comparison
make compare      # Run both versions side-by-side
make clean        # Remove compiled files
make debug        # Run under gdb debugger
make help         # Show all available commands
```

## Key Concepts Learned

- **File I/O**: fopen(), fgets(), fclose()
- **Strings**: Null-terminated char arrays
- **Functions**: Breaking logic into reusable pieces
- **Structs**: Grouping related data
- **Pointers**: Passing data by reference
- **Error handling**: Checking return values

## Common Issues

**"File not found"**
- Make sure sample_input.txt exists in current directory
- Use full path if running from different directory

**Compilation errors**
- Ensure all #include statements are present
- Check for missing semicolons or braces

**Wrong output**
- Verify null termination of strings
- Check that strncpy() is used with bounds checking
- Make sure longest_word is properly null-terminated

## Extensions (Once Basic Version Works)

- [ ] Calculate average word length
- [ ] Count vowels and consonants
- [ ] Find the most common word
- [ ] Display top 5 longest words
- [ ] Handle different file encodings

## Performance Challenge

After completing the project, benchmark both versions on a large file:

```bash
# Create a 1MB test file (Linux/Mac)
dd if=/dev/zero bs=1K count=1000 | tr '\0' '\n' | head -n 50000 > large_test.txt

# Time both versions
time python3 01_python_solution.py large_test.txt
time ./word_counter large_test.txt
```

C should be **5-10x faster**!

## Next Steps

After mastering this project:
- **Project 2**: Packet Header Parser - Parse binary network data
- **Project 3**: Binary Data Converter - Work with network byte order
- **Phase 2**: Deep dive into memory management and data structures

## Learning Resources

- `01_c_solution_explained.md` - This solution explained line-by-line
- `PYTHON_TO_C_CODING_TRANSITIONS.md` - Python vs C reference
- `PHASE_1_LEARNING_GUIDE.md` - Phase 1 overview

## Help

- Read the hints in `01_starter.c` comments
- Look at `01_c_solution.c` if completely stuck
- Consult `01_c_solution_explained.md` for detailed explanations
- Check `PROJECT_GUIDE.md` for conceptual help

---

**Ready to start?** Open `01_starter.c` and begin with `extract_word()`!
