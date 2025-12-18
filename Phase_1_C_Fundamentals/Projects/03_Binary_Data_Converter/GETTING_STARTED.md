# Binary Data Converter - Getting Started Guide

Welcome! This is a complete project for learning binary data conversion, number systems, and network data transformation in C. Here's exactly where to start.

## 📋 Step 1: Choose Your Learning Style

### Option A: "I want to learn everything first"
1. Open **README.md** - Overview (5 min)
2. Open **LESSON_PLAN.md** - Complete learning with exercises (3-4 hours)
3. Complete all 6 modules and practice all exercises
4. Then: Open **PROJECT_GUIDE.md**

### Option B: "I want to dive in quickly"
1. Open **PROJECT_GUIDE.md** - Read problem statement (20 min)
2. Skim **LESSON_PLAN.md** - Focus on needed modules only
3. Then: Start editing **03_starter.c**

### Option C: "I want to see the solution first"
1. Open **03_c_solution.c** - Read the code
2. Open **03_c_solution_explained.md** - Detailed explanation
3. Then: Try to write your own version from scratch

**Recommendation:** Option A if you have time, Option C if you're experienced.

---

## 🎯 Step 2: Quick Overview (5 minutes)

**What you're building:** A program that converts numbers between different bases and transforms network data formats

**Input:** Numbers and network data in various formats
```bash
./converter 255 all
./converter 192.168.1.1 ip-binary
./converter 0xDEADBEEF endian
```

**Output:** Human-readable converted data
```
=== Binary Data Converter ===
Input value: 255 (decimal)

Conversions:
  Decimal:      255
  Binary:       11111111
  Hexadecimal:  0xFF
  Octal:        0377
```

**Skills you'll gain:**
- Number system conversions (decimal, binary, hex, octal)
- Binary manipulation with bitwise operations
- Endianness understanding and byte order conversion
- Network data transformations (IP addresses, ports, MAC addresses)
- Data format conversions (BCD, packed flags, etc.)
- Portable network code practices

---

## 🚀 Step 3: Build and Test (2 minutes)

### First Time Setup

```bash
# 1. Generate sample test data
make

# 2. Compile reference solution
make converter_solution

# 3. Run the solution
./converter_solution 255 all

# 4. See the expected output - this is your target!
```

### Your Workflow

```bash
# Edit your solution
nano 03_starter.c   # or use your editor

# Build your version
make converter

# Run it
./converter 255 all

# Compare with reference
make compare
```

---

## 📚 Step 4: Learn the Concepts

You **MUST** understand these before writing code:

### 1. Number Systems
- Decimal (base 10): 0-9, what you normally use
- Binary (base 2): 0-1, how computers represent data
- Hexadecimal (base 16): 0-F, shorthand for binary
- Octal (base 8): 0-7, less common but sometimes used

**Quick exercise:** Convert decimal 255 to binary, hex, and octal

Answer:
- Binary: 11111111
- Hex: 0xFF
- Octal: 0377

### 2. Bitwise Operations for Conversions
```c
// Extract individual bits for binary conversion
unsigned char byte = 0b11010110;
int bit_5 = (byte >> 5) & 1;  // Gets bit 5: 1

// Shift operations move bits left/right
// AND operation masks to select bits
// Combining them: shift then mask = extract
```

**Read:** LESSON_PLAN.md Module 2 (Bitwise Operations for Conversion)

### 3. Endianness (Byte Order)
```c
// Network data uses big-endian (most significant byte first)
// Your computer might use little-endian
// YOU MUST CONVERT!

unsigned int value = 0x12345678;
// Big-endian bytes:    12 34 56 78
// Little-endian bytes: 78 56 34 12

// Use htonl() to convert host to network order
// Use ntohl() to convert network to host order
uint32_t network = htonl(value);
uint32_t host = ntohl(network);
```

**Read:** LESSON_PLAN.md Module 3 (Endianness Deep Dive)

### 4. Network Data Transformations
```c
// IP addresses: "192.168.1.1" ↔ 0xC0A80101
// Ports: "80" (HTTP) ↔ 0x0050 (network byte order)
// MAC addresses: "00:1A:2B:3C:4D:5E" ↔ binary

// These require:
// - String parsing with validation
// - Byte order conversion
// - Format control for output
```

**Read:** LESSON_PLAN.md Module 4 (Network Data Transformations)

---

## 💻 Step 5: Implement Your Solution

### File Structure
- **03_starter.c** - Start editing this file
- **03_c_solution.c** - Reference solution (check if stuck)
- **03_c_solution_explained.md** - Detailed explanation of each part

### Implementation Checklist

- [ ] Complete LESSON_PLAN.md (all 6 modules)
- [ ] Understand decimal, binary, hex, octal representations
- [ ] Implement bitwise operations for conversions
- [ ] Implement string building for different bases
- [ ] Implement string parsing with validation
- [ ] Detect system endianness
- [ ] Implement byte swapping functions
- [ ] Implement IP address parsing and formatting
- [ ] Implement network port conversions
- [ ] Implement multi-format input/output
- [ ] Handle errors gracefully
- [ ] Output matches expected format

### Implement in This Order

1. **Number base conversions** (easiest)
   ```c
   // Convert decimal to binary string
   void format_binary(uint32_t value, char *buf, size_t len) {
       // TODO: Extract bits and build string
   }
   ```

2. **String parsing** (uses validation)
   ```c
   // Parse binary string back to decimal
   uint32_t parse_binary(const char *str) {
       // TODO: Validate and convert
       return 0;
   }
   ```

3. **Endianness utilities** (byte order conversions)
   ```c
   // Swap bytes manually (no htonl/ntohl)
   uint32_t swap_bytes(uint32_t value) {
       // TODO: Reverse byte order
       return 0;
   }
   ```

4. **Network data transformations** (combine previous)
   ```c
   // IP address parsing
   uint32_t parse_ip_string(const char *ip) {
       // TODO: Parse "192.168.1.1" → network bytes
   }
   ```

5. **Multi-format output** (user interface)
   ```c
   // Display value in multiple formats
   void show_all_conversions(uint32_t value) {
       // TODO: Call all format functions
   }
   ```

6. **Main program** (orchestrate it all)
   ```c
   int main(int argc, char *argv[]) {
       // TODO: Parse arguments and coordinate
       return 0;
   }
   ```

---

## 🔧 Tools You'll Use

### Build
```bash
make              # Build everything
make run          # Build and run
make test         # Run test suite
make clean        # Remove binaries
```

### Debug
```bash
# View binary value in different bases
./converter 42 all

# Test endianness on your system
./converter detect-endian

# Parse network data
./converter 192.168.1.1 ip-binary

# Compare outputs
make compare
```

### Test Data
```bash
# Single value in all bases
./converter 255 all

# Network data transformations
./converter 192.168.1.1 all
./converter 80 port

# Edge cases
./converter 0 all
./converter 4294967295 all  # Max uint32
```

---

## 📖 Documentation Files

| File | Purpose | Read Time |
|------|---------|-----------|
| **README.md** | Quick overview | 10 min |
| **LESSON_PLAN.md** | Complete learning guide | 3-4 hours |
| **PROJECT_GUIDE.md** | Problem statement & hints | 30 min |
| **03_c_solution.c** | Reference implementation | 30 min |
| **03_c_solution_explained.md** | Line-by-line explanation | 1 hour |
| **03_starter.c** | Your template (edit this!) | - |
| **generate_test_data.c** | Creates test data | 15 min |
| **Makefile** | Build automation | - |

---

## ✅ Success Criteria

Your solution is correct when:

- ✓ Compiles without warnings (`make converter`)
- ✓ Runs without crashing (`./converter 255 all`)
- ✓ Output matches reference solution (`make compare`)
- ✓ Handles errors gracefully (invalid input, overflow, etc.)
- ✓ Converts decimal correctly to binary, hex, octal
- ✓ Parses binary, hex, octal back to decimal
- ✓ Converts byte order properly
- ✓ Transforms network data (IPs, ports) correctly
- ✓ Validates all inputs before processing

---

## 🎓 Learning Outcomes

After completing this project, you'll understand:

- **Number representation:** How different bases work mathematically
- **Bitwise operations:** Using shifts and masks for conversions
- **Byte order:** Why endianness matters and how to handle it
- **Data transformation:** Converting between formats safely
- **Network protocols:** How IP, port, and MAC data is represented
- **String handling:** Parsing and formatting with C functions
- **Error handling:** Validating conversions and detecting overflows

These are essential skills for:
- Network programming (protocol implementations)
- Systems programming (kernel and driver code)
- Embedded systems (microcontroller code)
- Security research (cryptography, checksums)
- Data serialization (custom protocols)
- Cross-platform programming (portability)

---

## 🆘 Getting Help

### If you're stuck on concepts
- **Number systems:** See LESSON_PLAN.md Module 1
- **Bitwise operations:** See LESSON_PLAN.md Module 2
- **Byte order:** See LESSON_PLAN.md Module 3
- **Network data:** See LESSON_PLAN.md Module 4

### If you're stuck on implementation
- **How to do X:** Check PROJECT_GUIDE.md Step-by-Step section
- **What's wrong with my code:** Compare with 03_c_solution.c
- **Why did it fail:** Read 03_c_solution_explained.md
- **Common mistakes:** See PROJECT_GUIDE.md "Common Challenges"

### If you're stuck on specific functions
- Read the exact function in 03_c_solution.c
- Read the same function explained in 03_c_solution_explained.md
- Use debugger: `gdb ./converter` then `break function_name`

---

## 📋 Recommended Reading Order

### First Time Through (Recommended)
1. **README.md** (gives you the big picture)
2. **LESSON_PLAN.md** (learn the fundamentals)
3. **PROJECT_GUIDE.md** (understand the problem)
4. **03_starter.c** (look at structure)
5. Edit **03_starter.c** (implement your solution)
6. **03_c_solution_explained.md** (check your work)
7. **03_c_solution.c** (reference if very stuck)

### Experienced Developers
1. **PROJECT_GUIDE.md** (understand the problem)
2. **03_starter.c** (see hints)
3. Edit and implement
4. Check **03_c_solution.c** if needed

---

## 🚀 Next Steps

### After You Complete This Project

**Immediate next steps:**
1. Try the extensions in PROJECT_GUIDE.md
2. Add support for more data formats (BCD, time values, etc.)
3. Create a generic packing/unpacking utility
4. Build a network packet formatter tool

**Then move to:**
- **Phase 2:** Advanced memory management and data structures
- **Real tools:** Network analyzers, firmware tools, protocol implementations

---

## 💡 Pro Tips

1. **Test frequently** - Check conversions with known values
2. **Test edge cases** - Try 0, max values, invalid inputs
3. **Use printf for debugging** - Print intermediate values
4. **Verify byte order** - Compile a test to detect your system's endianness
5. **Round-trip test** - Convert A→B→A should equal A
6. **Check struct sizes** - sizeof() should match expectations
7. **Use bit patterns** - Test with 0xAAAAAAAA, 0x55555555, etc.

---

## 🎯 Quick Start (TL;DR)

```bash
# 1. Build everything
make

# 2. See what the output should look like
./converter_solution 255 all

# 3. Edit your solution
nano 03_starter.c

# 4. Build your version
make converter

# 5. Test it
./converter 255 all

# 6. Compare output
make compare

# 7. Read explanation if stuck
less 03_c_solution_explained.md
```

---

**You're ready! Start with README.md or jump straight to LESSON_PLAN.md if you know what you're doing.** 🎉

Questions? Read the relevant section above or check the documentation files.

Good luck! 🚀
