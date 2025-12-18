# Binary Data Converter - Quick Start Guide

## What This Project Is About

Convert between different number representations (decimal, binary, hexadecimal, octal) and transform network data formats. You'll learn to manipulate binary data at the bit level, understand endianness, and create portable data serialization code.

## Getting Started

### Step 1: Read the Lesson Plan First ⭐ IMPORTANT

**Never skip this!** Start with:
```bash
cat LESSON_PLAN.md
```

This teaches you:
- Number system foundations and conversions
- Binary manipulation in C
- Endianness and byte order (big-endian vs little-endian)
- Network data transformations (IP, ports, MAC addresses)
- Bit-level field extraction and packing
- Data format conversions (BCD, flags, etc.)

Complete all 6 modules and practice exercises before touching the code.

### Step 2: Read the Project Guide

```bash
cat PROJECT_GUIDE.md
```

This explains:
- The actual problem you're solving
- Specification for each conversion type
- Step-by-step implementation guide
- Testing and validation strategies
- Real-world applications

### Step 3: Look at the Starter Template

```bash
cat 03_starter.c
```

This provides:
- Struct and type definitions
- Helper function stubs with TODO comments
- Main program skeleton
- Hints and guidance

### Step 4: Write Your Solution

Using the starter template, implement:
1. Base conversion functions (decimal ↔ binary, hex, octal)
2. String parsing with validation
3. Endianness detection and conversion
4. Network data transformations (IP addresses, ports)
5. Multi-format input/output support

### Step 5: Test and Compare

```bash
gcc -Wall -g 03_starter.c -o converter
./converter 255 binary hex octal

# Compare with reference solution
./03_c_solution 255 binary hex octal
```

## File Layout

| File | Purpose |
|------|---------|
| `LESSON_PLAN.md` | **Start here!** 6 modules with exercises |
| `PROJECT_GUIDE.md` | Implementation guide and specifications |
| `03_starter.c` | Your template (fill this in) |
| `03_c_solution.c` | Reference solution (check your work) |
| `03_c_solution_explained.md` | Line-by-line explanation |
| `generate_test_data.c` | Create test data and vectors |
| `Makefile` | Build automation |
| `README.md` | This file |

## Quick Example

Your converter should work like this:

```bash
$ ./converter 255 all
=== Binary Data Converter ===
Input value: 255 (decimal)

Conversions:
  Decimal:      255
  Binary:       11111111
  Hexadecimal:  0xFF
  Octal:        0377

$ ./converter 192.168.1.1 ip-binary
Convert IP address to binary representation:
  192.168.1.1 (string)
  0xC0A80101 (hex, network byte order)
  11000000101010000000000100000001 (binary)

$ ./converter 0xDEADBEEF endian
Show endianness conversions:
  Original (big-endian):    0xDEADBEEF
  Swapped (little-endian):  0xEFBEADDE
  System uses: little-endian on this machine
```

## Key Concepts You Need

### Number Base Conversions
```c
// Understanding different bases
uint32_t value = 255;
// Decimal: 255
// Binary:  11111111
// Hex:     0xFF
// Octal:   0377

// Each position represents a power of the base
// Binary (base 2):  1×2^7 + 1×2^6 + 1×2^5 + 1×2^4 + 1×2^3 + 1×2^2 + 1×2^1 + 1×2^0 = 255
// Hex (base 16):    15×16^1 + 15×16^0 = 240 + 15 = 255
// Octal (base 8):   3×8^2 + 7×8^1 + 7×8^0 = 192 + 56 + 7 = 255
```

### Bitwise Operations for Conversion
```c
// Extract individual bits
unsigned char byte = 0b11010110;
int bit_5 = (byte >> 5) & 1;  // Extract bit 5

// Convert to binary string
char binary[9];
for (int i = 7; i >= 0; i--) {
    binary[7 - i] = ((byte >> i) & 1) ? '1' : '0';
}
binary[8] = '\0';
```

### Endianness (Byte Order)
```c
// Big-endian: most significant byte first (network order)
// Little-endian: least significant byte first (x86/ARM)

// The value 0x12345678 in different byte orders:
// Big-endian bytes:    12 34 56 78
// Little-endian bytes: 78 56 34 12

// Always convert with htonl/ntohl
uint32_t value = 0x12345678;
uint32_t network_order = htonl(value);
uint32_t host_order = ntohl(network_order);
```

### IP Address Conversion
```c
// String to binary
uint32_t ip = inet_addr("192.168.1.1");  // Returns network byte order

// Binary to string
struct in_addr addr;
addr.s_addr = ip;
char* str = inet_ntoa(addr);
```

## Build & Run

### Simple Build
```bash
gcc -Wall -g 03_starter.c -o converter
./converter 255 all
```

### Using Makefile
```bash
make           # Build
make run       # Run test
make test      # Run test suite
make clean     # Clean up
```

## Debugging Tips

### Print All Bases
```c
void print_all_bases(unsigned int value) {
    printf("Decimal: %u\n", value);
    printf("Hex: 0x%x\n", value);
    printf("Octal: 0%o\n", value);
    printf("Binary: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}
```

### Detect System Endianness
```c
int is_big_endian(void) {
    uint32_t value = 0x12345678;
    unsigned char *bytes = (unsigned char *)&value;
    return bytes[0] == 0x12;  // Returns 1 if big-endian, 0 if little-endian
}
```

### View Values in Different Formats
```bash
# Use printf format specifiers
printf("Decimal: %d\n", value);   // Decimal
printf("Hex: 0x%x\n", value);     // Hexadecimal
printf("Octal: 0%o\n", value);    // Octal
```

## Learning Order

1. ✅ **Complete LESSON_PLAN.md** - All 6 modules with exercises
2. ✅ **Read PROJECT_GUIDE.md** - Understand the problem and specifications
3. ✅ **Study 03_c_solution_explained.md** - Before you code
4. ✅ **Write 03_starter.c** - Implement your version
5. ✅ **Test with different values** - Try various inputs
6. ✅ **Compare with 03_c_solution** - Check your work
7. ✅ **Try the extensions** - Add extra features

## Common Mistakes

### ❌ Forgetting endianness
```c
// WRONG - doesn't convert byte order
uint32_t ip = 192; // This is NOT 192.168.1.1!

// CORRECT - use inet_addr or manual conversion
uint32_t ip = inet_addr("192.168.1.1");
```

### ❌ Incorrect bit extraction
```c
// WRONG - shifts in wrong direction
int bit = value & (1 << position);  // Gets wrong bit

// CORRECT - shift then mask
int bit = (value >> position) & 1;
```

### ❌ Buffer overflow in string conversion
```c
// WRONG - no size checking
char buffer[8];
sprintf(buffer, "%d", 999999999);  // Overflows!

// CORRECT - use size limit
char buffer[64];
snprintf(buffer, sizeof(buffer), "%d", 999999999);
```

### ❌ Losing data during conversion
```c
// WRONG - 32-bit value becomes 8-bit
char byte = 0x12345678;  // Data loss!

// CORRECT - use proper types
uint32_t value = 0x12345678;
unsigned char low_byte = value & 0xFF;
```

## What to Expect to Learn

After completing this project, you'll understand:

- ✓ How different number bases work and how to convert between them
- ✓ How to manipulate individual bits and fields in C
- ✓ How endianness affects data interpretation
- ✓ How to safely convert between data formats
- ✓ How to handle network data in portable code
- ✓ Real skills used in:
  - Network packet sniffers and analyzers
  - Protocol implementations
  - Firmware and embedded systems
  - Data serialization and compression
  - Cross-platform networking code
  - Security tools (cryptography, checksums)

## Need Help?

1. **Understanding concepts?** → Read LESSON_PLAN.md module again
2. **Stuck on implementation?** → Check PROJECT_GUIDE.md step-by-step guide
3. **Need detailed explanation?** → Read 03_c_solution_explained.md
4. **Debugging binary data?** → See hexdump tips above
5. **What about endianness?** → LESSON_PLAN.md Module 3

## Time Estimate

- Reading lessons: 2-3 hours
- Doing exercises: 2-3 hours
- Writing solution: 2-3 hours
- Testing and debugging: 1-2 hours
- **Total: 7-11 hours**

## What's Next?

After mastering this:
- **Phase 2:** Advanced memory management and data structures
- **Real applications:** Build network tools using these conversion utilities!
- **Deep dive:** Cryptography, checksums, compression algorithms

---

**Start with LESSON_PLAN.md now! 🚀**

Questions? Read the relevant section above or check the documentation files.

Good luck!
