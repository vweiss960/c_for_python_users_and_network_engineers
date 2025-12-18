# Packet Header Parser - Quick Start Guide

## What This Project Is About

Parse binary network packet headers (IPv4, TCP, UDP) in C. You'll learn binary data manipulation, bitwise operations, and how network protocols work at the bit level.

## Getting Started

### Step 1: Read the Lesson Plan First ⭐ IMPORTANT

**Never skip this!** Start with:
```bash
cat LESSON_PLAN.md
```

This teaches you:
- Binary representation and byte order
- Bitwise operations (AND, OR, XOR, shifting)
- Struct memory layout and packing
- Network byte order (big-endian)
- How to extract and manipulate bits

Complete all 5 modules and practice exercises before touching the code.

### Step 2: Read the Project Guide

```bash
cat PROJECT_GUIDE.md
```

This explains:
- The actual problem you're solving
- Network packet structure (IP, TCP, UDP)
- Step-by-step implementation guide
- Testing and validation strategies

### Step 3: Look at the Starter Template

```bash
cat 02_starter.c
```

This provides:
- Struct definitions (partially filled)
- Helper function stubs
- Main function skeleton
- Hints and TODO comments

### Step 4: Write Your Solution

Using the starter template, implement:
1. Complete the struct definitions
2. Implement bit extraction functions
3. Add IPv4 header parsing
4. Add TCP/UDP header detection
5. Display formatted output

### Step 5: Test and Compare

```bash
gcc -Wall -g 02_starter.c -o parser
./parser sample_packet.bin

# Compare with reference solution
./02_c_solution sample_packet.bin
```

## File Layout

| File | Purpose |
|------|---------|
| `LESSON_PLAN.md` | **Start here!** 5 modules with exercises |
| `PROJECT_GUIDE.md` | Implementation guide and protocol specs |
| `02_starter.c` | Your template (fill this in) |
| `02_c_solution.c` | Reference solution (check your work) |
| `02_c_solution_explained.md` | Line-by-line explanation |
| `generate_sample_packet.c` | Create test packets |
| `sample_packet.bin` | Binary test data (IPv4 + TCP) |
| `Makefile` | Build automation |
| `README.md` | This file |

## Quick Example

Your parser should work like this:

```bash
$ ./parser sample_packet.bin

=== Packet Header Parser ===
File: sample_packet.bin
File size: 60 bytes

--- IP Header (IPv4) ---
Version: 4
Header Length (IHL): 5 words (20 bytes)
DSCP: 0
ECN: 0
Total Packet Length: 60 bytes
TTL: 64
Protocol: 6 (TCP)
Source IP: 192.168.1.100
Destination IP: 10.0.0.50

--- TCP Header ---
Source Port: 54321
Destination Port: 80 (HTTP)
Sequence Number: 0x12345678
Acknowledgment Number: 0x87654321
Data Offset: 5 words (20 bytes)
Flags: SYN ACK
Window Size: 32768
```

## Key Concepts You Need

### Binary & Bitwise
```c
// Extract 4-bit version field (top bits)
int version = (byte >> 4) & 0xF;

// Extract TCP SYN flag (bit 1)
int syn = (flags >> 1) & 1;
```

### Byte Order
```c
// Network data uses big-endian
// Always convert!
unsigned short port = ntohs(header.port);
unsigned int ip = ntohl(header.ip);
```

### Struct Packing
```c
// Binary data requires NO padding
#pragma pack(push, 1)
typedef struct { ... } PacketHeader;
#pragma pack(pop)
```

## Build & Run

### Simple Build
```bash
gcc -Wall -g 02_starter.c -o parser
./parser sample_packet.bin
```

### Using Makefile
```bash
make           # Build
make run       # Run test
make clean     # Clean up
```

## Debugging Tips

### Print Binary
```c
void print_binary(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}
```

### View Binary File
```bash
hexdump -C sample_packet.bin
```

### Check Struct Sizes
```c
printf("IPv4Header size: %zu\n", sizeof(IPv4Header));  // Should be 20
printf("TCPHeader size: %zu\n", sizeof(TCPHeader));    // Should be 20
```

## Learning Order

1. ✅ **Complete LESSON_PLAN.md** - All 5 modules
2. ✅ **Read PROJECT_GUIDE.md** - Understand the problem
3. ✅ **Study 02_c_solution_explained.md** - Before you code
4. ✅ **Write 02_starter.c** - Implement your version
5. ✅ **Test with sample_packet.bin**
6. ✅ **Compare with 02_c_solution**
7. ✅ **Try the extensions**

## Common Mistakes

### ❌ Not using pragma pack
```c
// WRONG - struct will have padding
typedef struct { ... } Header;

// CORRECT - no padding
#pragma pack(push, 1)
typedef struct { ... } Header;
#pragma pack(pop)
```

### ❌ Forgetting byte order
```c
// WRONG - uses wrong endianness
unsigned short port = header.port;

// CORRECT - converts to host byte order
unsigned short port = ntohs(header.port);
```

### ❌ Wrong bit extraction
```c
// WRONG - gets low 4 bits instead of version
int version = byte & 0xF;

// CORRECT - gets high 4 bits
int version = (byte >> 4) & 0xF;
```

## What to Expect to Learn

After completing this project, you'll understand:

- ✓ How network packets are structured in binary
- ✓ How to work with bits and bytes safely
- ✓ How to convert between byte orders
- ✓ How IP, TCP, and UDP headers work
- ✓ How to parse binary files in C
- ✓ Real skills used in:
  - Network security tools
  - Packet sniffers
  - Firewalls
  - Intrusion detection systems
  - Malware analysis

## Need Help?

1. **Understanding concepts?** → Read LESSON_PLAN.md module again
2. **Stuck on implementation?** → Check PROJECT_GUIDE.md step-by-step guide
3. **Need detailed explanation?** → Read 02_c_solution_explained.md
4. **Debugging binary data?** → See hexdump tips above
5. **Protocol questions?** → Check RFC 791 (IP), RFC 793 (TCP)

## Time Estimate

- Reading lessons: 2-3 hours
- Doing exercises: 2-3 hours
- Writing solution: 2-3 hours
- Testing and debugging: 1-2 hours
- **Total: 7-11 hours**

## What's Next?

After mastering this:
- **Project 3:** Binary Data Converter
- **Phase 2:** Advanced memory management and data structures
- **Real applications:** Build actual network tools!

---

**Start with LESSON_PLAN.md now! 🚀**
