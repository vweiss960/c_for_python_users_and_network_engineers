# Project 1: Python Script Rewrite
## Word Counter and Text Statistics

**Objective:** Take a simple Python script and rewrite it in C to understand the translation between the two languages, see performance differences, and learn fundamental C patterns.

**What You'll Learn:**
- File I/O in both Python and C
- Variables and types
- Loops and conditionals
- Functions
- String handling
- Performance comparison

---

## Project Structure

```
01_Python_Script_Rewrite/
├── PROJECT_GUIDE.md              (This file)
├── 01_starter.py                 (Python template - you edit this)
├── 01_python_solution.py          (Complete Python solution)
├── 01_c_solution.c                (Complete C solution)
├── 01_c_solution_explained.md     (Detailed C explanation)
├── sample_input.txt               (Sample file for testing)
├── Makefile                       (Build script for C version)
└── README.md                      (Quick reference)
```

---

## The Challenge

### Problem Statement

Write a program that:
1. Reads a text file
2. Counts total characters
3. Counts total words
4. Counts total lines
5. Finds the longest word
6. Displays results

**Example Input File (sample_input.txt):**
```
The quick brown fox jumps over the lazy dog.
This is a simple test file.
We are learning C programming language!
```

**Expected Output:**
```
=== Text Statistics ===
File: sample_input.txt
Total characters: 120
Total words: 23
Total lines: 3
Longest word: programming (11 characters)
```

---

## Your Task

### Step 1: Try the Python Version First (Optional)

If you're familiar with Python, `01_python_solution.py` is ready to run:

```bash
python3 01_python_solution.py sample_input.txt
```

This shows you what the program should do.

### Step 2: Study the Python Code

Open `01_python_solution.py` and understand:
- How files are opened and read
- How Python counts characters and words
- The logic flow

### Step 3: Translate to C

Using `01_starter.c` as a template, write the C version.

**Hints:**
- Use `fopen()` to open files
- Use `fgets()` to read lines
- Use `strlen()` for string length
- Use `isspace()` to detect whitespace (include `<ctype.h>`)
- Track longest word using a variable

### Step 4: Test and Compare

Compile and run your C version:

```bash
gcc -Wall -g 01_your_solution.c -o word_counter
./word_counter sample_input.txt
```

### Step 5: Check Against Solution

Compare your code with `01_c_solution.c` and read `01_c_solution_explained.md` for detailed explanations of each part.

---

## Compilation and Running

### Using gcc directly

```bash
# Compile with warnings enabled and debug info
gcc -Wall -g 01_c_solution.c -o word_counter

# Run with sample file
./word_counter sample_input.txt

# Or use your own file
./word_counter yourfile.txt
```

### Using the Makefile

```bash
# Build
make

# Run
make run

# Clean up
make clean
```

---

## Learning Milestones

- [ ] Understand the Python solution
- [ ] Identify the key differences between Python and C
- [ ] Write your own C version
- [ ] Successfully compile without errors
- [ ] Get correct output matching Python version
- [ ] Understand each line of the C solution
- [ ] Modify the program (e.g., add average word length)
- [ ] Time both versions on a large file (see which is faster)

---

## Extensions (After Completing the Basic Project)

1. **Add average word length calculation**
2. **Count vowels and consonants**
3. **Find the most common word**
4. **Display word frequency distribution**
5. **Add command-line options (-v for verbose, -s for stats only)**
6. **Handle different encodings**

---

## Files in This Project

### Python Files

- **`01_starter.py`** - Empty Python template (optional, for reference)
- **`01_python_solution.py`** - Complete, working Python solution

### C Files

- **`01_starter.c`** - C template with structure and hints (start here)
- **`01_c_solution.c`** - Complete, working C solution
- **`01_c_solution_explained.md`** - Line-by-line explanation of C solution

### Build and Test

- **`Makefile`** - Build automation
- **`sample_input.txt`** - Test data
- **`README.md`** - Quick start guide

---

## Tips for Success

1. **Read the Python solution first** - It's simpler and shows the logic
2. **Don't memorize** - Look up `fopen()`, `fgets()`, `strlen()` as needed
3. **Compile frequently** - Check for errors early
4. **Use `-Wall` flag** - Catches warnings that might be bugs
5. **Test with the sample file** - Then try with your own files
6. **Compare outputs** - Make sure Python and C give identical results
7. **Read the explanations** - Understand why C works differently

---

## Expected Difficulties (And Why They're Good)

### C will feel verbose

**Python:**
```python
with open("file.txt") as f:
    content = f.read()
```

**C:**
```c
FILE *file = fopen("file.txt", "r");
if (file == NULL) {
    perror("fopen failed");
    return 1;
}
// ... read file ...
fclose(file);
```

**Why?** C makes you handle errors explicitly. This is actually safer!

### C requires more variable tracking

**Python:** Variables change type automatically
**C:** Must declare types upfront

**Why?** Compiler catches type errors before runtime. Safer and faster!

### Memory and resources

**Python:** Garbage collection, automatic cleanup
**C:** You manage it (in this project, we just close files)

**Why?** Gives you control and predictability

---

## Performance Comparison

Run on a large file (try `dd if=/dev/zero bs=1M count=10 of=large_file.txt` to create one):

```bash
time python3 01_python_solution.py large_file.txt
time ./word_counter large_file.txt
```

You should see C is **5-10x faster** on I/O operations!

---

## How to Use This Project

### If you're just learning C:

1. Read `01_python_solution.py` (understand the logic)
2. Try to write `01_c_solution.c` yourself using `01_starter.c`
3. Use `01_c_solution_explained.md` to check your work

### If you're comfortable with C:

1. Try the challenge without looking at solutions
2. Reference `01_c_solution_explained.md` for specific details
3. Add one of the extensions

### If you're experienced:

1. Add all the extensions
2. Build a benchmarking framework
3. Try to optimize further (memory pooling, buffer sizes, etc.)

---

## Next Steps After This Project

Once you complete this:
- **Project 2:** Packet Header Parser - Parse binary network data
- **Project 3:** Binary Data Converter - Work with network byte order
- **Move to Phase 2:** Deep memory management and data structures

This project is foundational—you'll use these file I/O and string handling patterns in every networking program you write.

Good luck!
