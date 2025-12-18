# Packet Header Parser - Getting Started Guide

Welcome! This is a complete project for learning binary data manipulation and network protocols in C. Here's exactly where to start.

## 📋 Step 1: Choose Your Learning Style

### Option A: "I want to learn everything first"
1. Open **README.md** - Overview (5 min)
2. Open **LESSON_PLAN.md** - Complete learning with exercises (2-3 hours)
3. Complete all 5 modules and practice all exercises
4. Then: Open **PROJECT_GUIDE.md**

### Option B: "I want to dive in quickly"
1. Open **PROJECT_GUIDE.md** - Read problem statement (15 min)
2. Skim **LESSON_PLAN.md** - Focus on needed modules only
3. Then: Start editing **02_starter.c**

### Option C: "I want to see the solution first"
1. Open **02_c_solution.c** - Read the code
2. Open **02_c_solution_explained.md** - Detailed explanation
3. Then: Try to write your own version from scratch

**Recommendation:** Option A if you have time, Option C if you're experienced.

---

## 🎯 Step 2: Quick Overview (5 minutes)

**What you're building:** A program that parses binary network packet headers

**Input:** Binary file containing IPv4/TCP/UDP packet data
```bash
./packet_parser sample_packet.bin
```

**Output:** Human-readable packet information
```
=== Packet Header Parser ===
File: sample_packet.bin
File size: 60 bytes

--- IP Header (IPv4) ---
Version: 4
Header Length (IHL): 5 words (20 bytes)
...
```

**Skills you'll gain:**
- Binary data manipulation
- Bitwise operations (AND, OR, XOR, shifting)
- Struct packing and memory layout
- Network byte order (endianness)
- File I/O with binary data
- Network protocol understanding (IP, TCP, UDP)

---

## 🚀 Step 3: Build and Test (2 minutes)

### First Time Setup

```bash
# 1. Generate sample packets
make

# 2. Compile reference solution
make parser_solution

# 3. Run the solution
./parser_solution sample_packet.bin

# 4. See the expected output - this is your target!
```

### Your Workflow

```bash
# Edit your solution
nano 02_starter.c   # or use your editor

# Build your version
make packet_parser

# Run it
./packet_parser sample_packet.bin

# Compare with reference
make compare
```

---

## 📚 Step 4: Learn the Concepts

You **MUST** understand these before writing code:

### 1. Binary Numbers
- How to read hex: 0xFF = 255
- How to read binary: 0b11111111 = 255
- Why network uses big-endian (network byte order)

**Quick exercise:** What is 0x1A in decimal? (Answer: 26)

### 2. Bitwise Operations
```c
// Extract top 4 bits of a byte
unsigned char byte = 0b11010110;
unsigned char top4 = (byte >> 4) & 0xF;  // Result: 0b1101 = 13

// Extract bottom 2 bits
unsigned char bottom2 = byte & 0x3;  // Result: 0b10 = 2
```

**Read:** LESSON_PLAN.md Module 2 (Bitwise Operations)

### 3. Struct Packing
```c
// Without packing - wasted space
typedef struct {
    char a;     // 1 byte + 3 bytes padding
    int b;      // 4 bytes
}
// Total: 8 bytes

// With packing - tight layout
#pragma pack(push, 1)
typedef struct {
    char a;     // 1 byte (no padding)
    int b;      // 4 bytes
}
#pragma pack(pop)
// Total: 5 bytes
```

**Read:** LESSON_PLAN.md Module 3 (Structs and Memory Layout)

### 4. Network Byte Order
```c
// Network sends big-endian (most significant byte first)
// You must convert to host byte order

unsigned short port = 0x0050;  // Raw from network
port = ntohs(port);            // Convert to host order
// Now port can be used correctly
```

**Read:** LESSON_PLAN.md Module 4 (Network Byte Order)

---

## 💻 Step 5: Implement Your Solution

### File Structure
- **02_starter.c** - Start editing this file
- **02_c_solution.c** - Reference solution (check if stuck)
- **02_c_solution_explained.md** - Detailed explanation of each part

### Implementation Checklist

- [ ] Complete LESSON_PLAN.md (all 5 modules)
- [ ] Understand IPv4 header structure
- [ ] Understand TCP header structure
- [ ] Implement bit extraction functions
- [ ] Implement IP address formatting
- [ ] Read IPv4 header from file
- [ ] Parse and display all IPv4 fields
- [ ] Detect protocol type
- [ ] Parse TCP header if protocol == 6
- [ ] Parse UDP header if protocol == 17
- [ ] Extract TCP flags correctly
- [ ] Handle errors gracefully
- [ ] Output matches expected format

### Implement in This Order

1. **Bit extraction functions** (easiest)
   ```c
   unsigned char get_ip_version(unsigned char version_ihl) {
       return (version_ihl >> 4) & 0xF;
   }
   ```

2. **IP address formatting** (uses byte order conversion)
   ```c
   void format_ip_address(unsigned int ip, char *buf, size_t len) {
       unsigned int host_ip = ntohl(ip);
       unsigned char *bytes = (unsigned char *)&host_ip;
       snprintf(buf, len, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
   }
   ```

3. **File I/O and validation** (handle errors)
   ```c
   FILE *file = fopen(filename, "rb");
   if (file == NULL) {
       perror("fopen failed");
       return 1;
   }
   ```

4. **Read and parse IPv4 header** (use your functions)
   ```c
   IPv4Header header;
   fread(&header, sizeof(header), 1, file);
   unsigned char version = get_ip_version(header.version_ihl);
   ```

5. **Display output** (print all fields)
   ```c
   printf("Version: %d\n", version);
   printf("Source IP: %s\n", source_ip_str);
   ```

6. **Parse TCP/UDP** (detect protocol, read appropriate header)
   ```c
   if (header.protocol == 6) {  // TCP
       TCPHeader tcp;
       fread(&tcp, sizeof(tcp), 1, file);
       // Display TCP fields
   }
   ```

---

## 🔧 Tools You'll Use

### Build
```bash
make              # Build everything
make run          # Build and run
make clean        # Remove binaries
```

### Debug
```bash
# View binary file
hexdump -C sample_packet.bin

# Run under debugger
gdb ./packet_parser

# Compare outputs
make compare
```

### Test Data
```bash
# Three pre-generated sample packets:
./packet_parser sample_packet.bin       # IPv4 + TCP (60 bytes)
./packet_parser sample_udp_packet.bin   # IPv4 + UDP (40 bytes)
./packet_parser minimal_packet.bin      # IPv4 only (20 bytes)
```

---

## 📖 Documentation Files

| File | Purpose | Read Time |
|------|---------|-----------|
| **README.md** | Quick overview | 10 min |
| **LESSON_PLAN.md** | Complete learning guide | 2-3 hours |
| **PROJECT_GUIDE.md** | Problem statement & hints | 30 min |
| **02_c_solution.c** | Reference implementation | 30 min |
| **02_c_solution_explained.md** | Line-by-line explanation | 1 hour |
| **02_starter.c** | Your template (edit this!) | - |
| **generate_sample_packet.c** | Creates test data | 15 min |
| **Makefile** | Build automation | - |

---

## ✅ Success Criteria

Your solution is correct when:

- ✓ Compiles without warnings (`make packet_parser`)
- ✓ Runs without crashing (`./packet_parser sample_packet.bin`)
- ✓ Output matches reference solution (`make compare`)
- ✓ Handles errors gracefully (invalid files, truncated packets)
- ✓ Parses IPv4 header correctly
- ✓ Parses TCP/UDP based on protocol field
- ✓ Converts byte order properly
- ✓ Extracts bitwise fields correctly

---

## 🎓 Learning Outcomes

After completing this project, you'll understand:

- **Binary representation:** How data is stored at the bit level
- **Bitwise operations:** AND, OR, XOR, shifting for bit manipulation
- **Memory layout:** How structs map to binary data
- **Byte order:** Why network uses big-endian and how to convert
- **Network protocols:** How IP, TCP, UDP headers work
- **File I/O:** Reading binary files safely
- **Low-level programming:** Working close to the metal in C

These are essential skills for:
- Network security (packet sniffing, analysis)
- Systems programming (kernel modules, drivers)
- Embedded systems (hardware interaction)
- Security research (malware analysis, reverse engineering)
- Performance optimization (understanding CPU architecture)

---

## 🆘 Getting Help

### If you're stuck on concepts
- **Binary/hex numbers:** See LESSON_PLAN.md Module 1
- **Bitwise operations:** See LESSON_PLAN.md Module 2
- **Struct packing:** See LESSON_PLAN.md Module 3
- **Byte order:** See LESSON_PLAN.md Module 4

### If you're stuck on implementation
- **How to do X:** Check PROJECT_GUIDE.md Step-by-Step section
- **What's wrong with my code:** Compare with 02_c_solution.c
- **Why did it fail:** Read 02_c_solution_explained.md
- **Common mistakes:** See PROJECT_GUIDE.md "Common Challenges"

### If you're stuck on specific functions
- Read the exact function in 02_c_solution.c
- Read the same function explained in 02_c_solution_explained.md
- Use debugger: `gdb ./parser` then `break function_name`

---

## 📋 Recommended Reading Order

### First Time Through (Recommended)
1. **README.md** (this gives you the big picture)
2. **LESSON_PLAN.md** (learn the fundamentals)
3. **PROJECT_GUIDE.md** (understand the problem)
4. **02_starter.c** (look at structure)
5. Edit **02_starter.c** (implement your solution)
6. **02_c_solution_explained.md** (check your work)
7. **02_c_solution.c** (reference if very stuck)

### Experienced Developers
1. **PROJECT_GUIDE.md** (understand the problem)
2. **02_starter.c** (see hints)
3. Edit and implement
4. Check **02_c_solution.c** if needed

---

## 🚀 Next Steps

### After You Complete This Project

**Immediate next steps:**
1. Try the extensions in PROJECT_GUIDE.md
2. Parse real packets: `sudo tcpdump -i eth0 -c 1 -w real.pcap`
3. Add ICMP support (protocol 1)
4. Add IPv6 support (more complex!)

**Then move to:**
- **Project 3:** Binary Data Converter
- **Phase 2:** Advanced memory management and data structures
- **Build real tools:** Packet sniffer, network analyzer, firewall rules

---

## 💡 Pro Tips

1. **Compile frequently** - Check for errors early
2. **Test with all three packets** - Each tests different protocols
3. **Use printf for debugging** - Print raw bytes in hex
4. **Read RFCs** - RFC 791 (IP), RFC 793 (TCP), RFC 768 (UDP)
5. **Use hexdump** - Visual inspection of binary data
6. **Check struct sizes** - `sizeof(IPv4Header)` should be 20

---

## 🎯 Quick Start (TL;DR)

```bash
# 1. Build everything
make

# 2. See what the output should look like
./parser_solution sample_packet.bin

# 3. Edit your solution
nano 02_starter.c

# 4. Build your version
make packet_parser

# 5. Test it
./packet_parser sample_packet.bin

# 6. Compare output
make compare

# 7. Read explanation if stuck
less 02_c_solution_explained.md
```

---

**You're ready! Start with README.md or jump straight to LESSON_PLAN.md if you know what you're doing.** 🎉

Questions? Read the relevant section above or check the documentation files.

Good luck! 🚀
