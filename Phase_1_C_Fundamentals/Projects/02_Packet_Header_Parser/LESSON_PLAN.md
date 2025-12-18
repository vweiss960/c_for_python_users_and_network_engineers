# Packet Header Parser - Lesson Plan

## Overview

This lesson prepares you to parse binary network packet headers (like IP, TCP, UDP headers). You'll learn how to work with binary data, bitwise operations, and structured data in C—essential skills for network programming, security research, and low-level systems work.

**Difficulty:** Intermediate (requires understanding of Project 1)
**Time Estimate:** 8-12 hours of learning + practice
**Prerequisites:** Complete Project 1 (Python Script Rewrite)

---

## What You'll Learn

### Core Concepts

1. **Binary Data and Byte Order**
   - Understanding bits, bytes, and binary representation
   - Big-endian vs. little-endian byte order
   - Network byte order (big-endian) vs. host byte order
   - How to convert between byte orders

2. **Bitwise Operations in C**
   - Bitwise AND (`&`), OR (`|`), XOR (`^`), NOT (`~`)
   - Bit shifting (`<<`, `>>`)
   - Extracting individual bits from bytes
   - Setting and clearing bits
   - Flag fields (common in packet headers)

3. **Structs and Memory Layout**
   - How structs map to memory
   - Struct padding and alignment
   - Using `#pragma pack` to control alignment
   - Casting data to structs safely
   - Calculating struct sizes

4. **Parsing Binary Packet Headers**
   - Understanding IP header format
   - Understanding TCP header format
   - Understanding UDP header format
   - Reading raw bytes from files/buffers
   - Interpreting header fields

5. **Network Programming Concepts**
   - Network protocols (IP, TCP, UDP)
   - Packet structure and field meanings
   - Version numbers, flags, and checksums
   - Port numbers and IP addresses

---

## Lesson Modules

### Module 1: Binary Data Fundamentals (1-2 hours)

**Topics:**
- Binary number system review
- Bytes and bits
- Hexadecimal notation
- Binary representation of integers
- Signed vs. unsigned integers

**Exercises:**

**Exercise 1.1: Binary Conversion**
```c
#include <stdio.h>

int main() {
    // Convert decimal to binary and print
    unsigned int num = 255;

    // Print in different bases
    printf("Decimal: %u\n", num);           // 255
    printf("Hex: 0x%x\n", num);             // 0xff
    printf("Octal: 0%o\n", num);            // 0377

    // Manual binary representation
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");

    return 0;
}
```

**Expected Output:**
```
Decimal: 255
Hex: 0xff
Octal: 0377
11111111
```

**Exercise 1.2: Byte Array Examination**
```c
#include <stdio.h>

int main() {
    // Examine how integers are stored in memory
    unsigned int value = 0x12345678;
    unsigned char *bytes = (unsigned char *)&value;

    printf("Value: 0x%x\n", value);
    printf("Bytes in memory:\n");
    for (int i = 0; i < 4; i++) {
        printf("  Byte %d: 0x%02x (%d)\n", i, bytes[i], bytes[i]);
    }

    return 0;
}
```

**Note:** Output varies by system endianness!

---

### Module 2: Bitwise Operations (2-3 hours)

**Topics:**
- AND, OR, XOR, NOT operations
- Bit shifting (left and right)
- Extracting bits from bytes
- Creating bitmasks
- Flags and bit fields

**Exercises:**

**Exercise 2.1: Bitwise Operations Basics**
```c
#include <stdio.h>

int main() {
    unsigned char byte1 = 0b10101010;  // 170 in decimal
    unsigned char byte2 = 0b11001100;  // 204 in decimal

    printf("byte1: 0x%02x (binary: ", byte1);
    for (int i = 7; i >= 0; i--) printf("%d", (byte1 >> i) & 1);
    printf(")\n");

    printf("byte2: 0x%02x (binary: ", byte2);
    for (int i = 7; i >= 0; i--) printf("%d", (byte2 >> i) & 1);
    printf(")\n");

    printf("\nAND:  0x%02x\n", byte1 & byte2);
    printf("OR:   0x%02x\n", byte1 | byte2);
    printf("XOR:  0x%02x\n", byte1 ^ byte2);
    printf("NOT:  0x%02x\n", ~byte1);

    return 0;
}
```

**Exercise 2.2: Extracting Individual Bits**
```c
#include <stdio.h>

int main() {
    unsigned char byte = 0b11010110;

    // Extract bit 0 (rightmost)
    int bit0 = (byte >> 0) & 1;
    printf("Bit 0: %d\n", bit0);  // 0

    // Extract bit 2
    int bit2 = (byte >> 2) & 1;
    printf("Bit 2: %d\n", bit2);  // 1

    // Extract bit 7 (leftmost)
    int bit7 = (byte >> 7) & 1;
    printf("Bit 7: %d\n", bit7);  // 1

    return 0;
}
```

**Exercise 2.3: Creating Bitmasks**
```c
#include <stdio.h>

int main() {
    unsigned char byte = 0b11010110;

    // Extract bits 2-5 (4 bits)
    unsigned char mask = 0b00111100;  // Mask for bits 2-5
    unsigned char bits_2_5 = (byte & mask) >> 2;
    printf("Bits 2-5: 0x%x\n", bits_2_5);  // 0x5 (binary: 0101)

    return 0;
}
```

---

### Module 3: Structs and Memory Layout (1-2 hours)

**Topics:**
- How structs align in memory
- Struct padding
- Calculating struct sizes
- Controlling alignment with `#pragma pack`
- Casting to structs

**Exercises:**

**Exercise 3.1: Struct Memory Layout**
```c
#include <stdio.h>

typedef struct {
    unsigned char byte1;    // 1 byte
    unsigned short short1;  // 2 bytes
    unsigned int int1;      // 4 bytes
} MyStruct;

int main() {
    printf("Size of MyStruct: %zu bytes\n", sizeof(MyStruct));
    printf("Size of byte1: %zu bytes\n", sizeof(((MyStruct*)0)->byte1));
    printf("Size of short1: %zu bytes\n", sizeof(((MyStruct*)0)->short1));
    printf("Size of int1: %zu bytes\n", sizeof(((MyStruct*)0)->int1));

    // Addresses (offsets)
    MyStruct s;
    printf("\nOffsets:\n");
    printf("byte1 offset: %zu\n", (unsigned char*)&s.byte1 - (unsigned char*)&s);
    printf("short1 offset: %zu\n", (unsigned char*)&s.short1 - (unsigned char*)&s);
    printf("int1 offset: %zu\n", (unsigned char*)&s.int1 - (unsigned char*)&s);

    return 0;
}
```

**Expected Output (on most systems):**
```
Size of MyStruct: 8 bytes
Size of byte1: 1 bytes
Size of short1: 2 bytes
Size of int1: 4 bytes

Offsets:
byte1 offset: 0
short1 offset: 2
int1 offset: 4
```

**Exercise 3.2: Controlling Struct Packing**
```c
#include <stdio.h>

// Normal struct (with padding)
typedef struct {
    unsigned char a;
    unsigned short b;
    unsigned char c;
} NormalStruct;

// Packed struct (no padding)
#pragma pack(push, 1)
typedef struct {
    unsigned char a;
    unsigned short b;
    unsigned char c;
} PackedStruct;
#pragma pack(pop)

int main() {
    printf("NormalStruct size: %zu bytes\n", sizeof(NormalStruct));
    printf("PackedStruct size: %zu bytes\n", sizeof(PackedStruct));

    return 0;
}
```

**Expected Output:**
```
NormalStruct size: 6 bytes (with padding)
PackedStruct size: 4 bytes (no padding)
```

---

### Module 4: Network Byte Order (1 hour)

**Topics:**
- Endianness (big-endian, little-endian)
- Network byte order (always big-endian)
- Converting with `htons()`, `ntohs()`, `htonl()`, `ntohl()`
- Writing portable code

**Exercises:**

**Exercise 4.1: Understanding Byte Order**
```c
#include <stdio.h>
#include <arpa/inet.h>  // For htons, ntohs

int main() {
    unsigned short host_value = 0x1234;  // Host byte order
    unsigned short network_value = htons(host_value);  // To network order

    printf("Host order:    0x%04x\n", host_value);
    printf("Network order: 0x%04x\n", network_value);

    // Converting back
    unsigned short back = ntohs(network_value);
    printf("Back to host:  0x%04x\n", back);

    return 0;
}
```

**Exercise 4.2: 32-bit Values**
```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
    // IP address in host byte order
    unsigned int ip = 0xC0A80001;  // 192.168.0.1
    unsigned int network_ip = htonl(ip);

    printf("Host IP:    0x%08x\n", ip);
    printf("Network IP: 0x%08x\n", network_ip);

    return 0;
}
```

---

### Module 5: Parsing Binary Data (2-3 hours)

**Topics:**
- Reading raw bytes from files
- Interpreting bytes as structures
- Handling variable-length data
- Checksums and validation
- Error handling

**Exercises:**

**Exercise 5.1: Reading Binary Data**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple IPv4 header (simplified, 20 bytes minimum)
#pragma pack(push, 1)
typedef struct {
    unsigned char version_ihl;      // Version (4 bits) + IHL (4 bits)
    unsigned char dscp_ecn;         // DSCP (6 bits) + ECN (2 bits)
    unsigned short total_length;    // Total length
    unsigned short identification;  // Identification
    unsigned short flags_offset;    // Flags (3 bits) + Fragment offset (13 bits)
    unsigned char ttl;              // Time to live
    unsigned char protocol;         // Protocol
    unsigned short header_checksum; // Header checksum
    unsigned int source_ip;         // Source IP
    unsigned int dest_ip;           // Destination IP
} IPv4Header;
#pragma pack(pop)

int main() {
    FILE *file = fopen("packet.bin", "rb");
    if (file == NULL) {
        perror("fopen failed");
        return 1;
    }

    IPv4Header header;

    // Read the header
    if (fread(&header, sizeof(IPv4Header), 1, file) != 1) {
        perror("fread failed");
        fclose(file);
        return 1;
    }

    // Parse version and IHL
    unsigned char version = (header.version_ihl >> 4) & 0xF;
    unsigned char ihl = header.version_ihl & 0xF;

    printf("IP Version: %d\n", version);
    printf("IHL (header length in words): %d\n", ihl);
    printf("Header size: %d bytes\n", ihl * 4);
    printf("Total packet length: %d\n", ntohs(header.total_length));
    printf("TTL: %d\n", header.ttl);
    printf("Protocol: %d\n", header.protocol);

    fclose(file);
    return 0;
}
```

**Exercise 5.2: Extracting TCP Flags**
```c
#include <stdio.h>

// TCP header flags
#pragma pack(push, 1)
typedef struct {
    unsigned short source_port;
    unsigned short dest_port;
    unsigned int sequence_num;
    unsigned int ack_num;
    unsigned char data_offset;     // 4 bits data offset + 4 bits reserved
    unsigned char flags;            // Flags (URG, ACK, PSH, RST, SYN, FIN)
    unsigned short window_size;
    unsigned short checksum;
    unsigned short urgent_pointer;
} TCPHeader;
#pragma pack(pop)

int main() {
    TCPHeader header = {
        .source_port = 0x1234,
        .dest_port = 0x0050,        // Port 80
        .sequence_num = 0x12345678,
        .ack_num = 0x87654321,
        .data_offset = 0x50,        // 5 words (20 bytes)
        .flags = 0x02,              // SYN flag
        .window_size = 0x4000,
        .checksum = 0x0000,
        .urgent_pointer = 0x0000
    };

    // Extract flags
    int FIN = (header.flags >> 0) & 1;
    int SYN = (header.flags >> 1) & 1;
    int RST = (header.flags >> 2) & 1;
    int PSH = (header.flags >> 3) & 1;
    int ACK = (header.flags >> 4) & 1;
    int URG = (header.flags >> 5) & 1;

    printf("TCP Flags: ");
    if (URG) printf("URG ");
    if (ACK) printf("ACK ");
    if (PSH) printf("PSH ");
    if (RST) printf("RST ");
    if (SYN) printf("SYN ");
    if (FIN) printf("FIN");
    printf("\n");

    return 0;
}
```

---

### Module 6: Packet Header Parsing Project Overview (1 hour)

**What the Project Will Involve:**

You'll create a program that:
1. Reads binary packet data from a file
2. Parses IP headers (extract version, protocol, IHL, TTL, source/dest IP)
3. Parses TCP/UDP headers (extract ports, flags, sequence numbers)
4. Displays parsed information in a human-readable format
5. Handles errors gracefully
6. Validates checksums (optional challenge)

**Example Output:**

```
=== Packet Header Parser ===
File: sample_packet.bin

--- IP Header ---
Version: 4
IHL: 5 (20 bytes)
Total Length: 60 bytes
TTL: 64
Protocol: 6 (TCP)
Source IP: 192.168.1.100
Dest IP: 10.0.0.50

--- TCP Header ---
Source Port: 54321
Dest Port: 80 (HTTP)
Sequence: 0x12345678
Acknowledgment: 0x87654321
Flags: SYN ACK
Window Size: 32768
```

---

## Progression of Skills

### After This Lesson, You'll Be Able To:

1. ✓ Understand binary data representation
2. ✓ Perform bitwise operations
3. ✓ Design structs that map to binary data
4. ✓ Control memory layout with packing
5. ✓ Convert between host and network byte order
6. ✓ Parse real network packet headers
7. ✓ Handle endianness correctly
8. ✓ Work with flag fields and bit fields
9. ✓ Read and interpret binary files
10. ✓ Validate packet data

---

## Practice Exercises (Do These First)

Complete these exercises in order before starting the project:

### P1: Binary Representation
- [ ] Exercise 1.1: Binary Conversion
- [ ] Exercise 1.2: Byte Array Examination

### P2: Bitwise Operations
- [ ] Exercise 2.1: Bitwise Operations Basics
- [ ] Exercise 2.2: Extracting Individual Bits
- [ ] Exercise 2.3: Creating Bitmasks

### P3: Struct Memory
- [ ] Exercise 3.1: Struct Memory Layout
- [ ] Exercise 3.2: Controlling Struct Packing

### P4: Network Byte Order
- [ ] Exercise 4.1: Understanding Byte Order
- [ ] Exercise 4.2: 32-bit Values

### P5: Binary Data Parsing
- [ ] Exercise 5.1: Reading Binary Data
- [ ] Exercise 5.2: Extracting TCP Flags

---

## Common Pitfalls to Avoid

### Pitfall 1: Byte Order Confusion
**Problem:** Not converting between host and network byte order
```c
// WRONG: Reading port directly from network data
unsigned short port = *(unsigned short*)data;

// CORRECT: Convert from network byte order
unsigned short port = ntohs(*(unsigned short*)data);
```

### Pitfall 2: Struct Padding
**Problem:** Not accounting for struct padding when reading binary data
```c
// WRONG: sizeof() includes padding
sizeof(MyStruct)  // Might be 8 bytes instead of expected 5

// CORRECT: Use #pragma pack
#pragma pack(push, 1)
typedef struct { ... } MyStruct;
#pragma pack(pop)
```

### Pitfall 3: Bit Extraction
**Problem:** Extracting bits in wrong order or with wrong mask
```c
// WRONG: Assumes bits are in different positions
int version = byte & 0xF;  // Gets lower 4 bits, not upper

// CORRECT: Shift first, then mask
int version = (byte >> 4) & 0xF;
```

### Pitfall 4: Type Casting
**Problem:** Casting pointers unsafely or misaligning data
```c
// WRONG: Could crash if pointer is misaligned
IPv4Header *header = (IPv4Header*)random_pointer;

// CORRECT: Ensure proper alignment and bounds checking
if (buffer_size >= sizeof(IPv4Header)) {
    IPv4Header *header = (IPv4Header*)buffer;
}
```

---

## Debugging Tips

### Print Hex Values
```c
// Print individual bytes
unsigned char byte = 0xAB;
printf("Byte: 0x%02x\n", byte);

// Print multi-byte values
unsigned int value = 0x12345678;
printf("Value: 0x%08x\n", value);
```

### Print Binary Representation
```c
void print_binary(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}
```

### Use Hexdump for Binary Files
```bash
# View binary file in hex
hexdump -C packet.bin

# View first 64 bytes
hexdump -C -n 64 packet.bin
```

### Check Struct Sizes
```c
printf("IPv4Header size: %zu\n", sizeof(IPv4Header));
printf("TCPHeader size: %zu\n", sizeof(TCPHeader));
```

---

## Resources for Learning

### Online References
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [IP Header Format Specification (RFC 791)](https://tools.ietf.org/html/rfc791)
- [TCP Header Format Specification (RFC 793)](https://tools.ietf.org/html/rfc793)
- [UDP Header Format Specification (RFC 768)](https://tools.ietf.org/html/rfc768)

### Tools
- `hexdump` - View binary files
- `od` - Another binary viewer
- `wireshark` - Packet capture and analysis (visual reference)
- `tcpdump` - Command-line packet capture

---

## Learning Checklist

Before starting the project, confirm you can:

### Binary Basics
- [ ] Convert between decimal, binary, and hex
- [ ] Understand byte and bit ordering
- [ ] Explain what endianness means

### Bitwise Operations
- [ ] Perform AND, OR, XOR, NOT operations
- [ ] Extract specific bits from a byte
- [ ] Create and use bitmasks
- [ ] Shift bits left and right

### Structs
- [ ] Design a struct that maps to binary data
- [ ] Calculate struct size with and without padding
- [ ] Use `#pragma pack` correctly
- [ ] Safely cast pointers to structs

### Network Concepts
- [ ] Explain network byte order
- [ ] Use htons(), ntohs(), htonl(), ntohl()
- [ ] Understand IP header fields
- [ ] Understand TCP/UDP header fields
- [ ] Explain TCP flags (SYN, ACK, FIN, RST, etc.)

---

## Next Steps

1. **Work through all practice exercises** above in order
2. **Create simple test programs** for each concept
3. **Read and understand** real packet specifications (RFC 791, RFC 793)
4. **Build incrementally** - start with IP header parsing, then TCP, then UDP
5. **Test with real packets** - use tcpdump to capture actual network traffic

---

## Project Teaser

Once you complete this lesson, you'll be ready for **Project 2: Packet Header Parser**

You'll write a program that can:
- Read binary packet dumps
- Parse and display IP headers
- Extract TCP/UDP information
- Analyze packet flows
- Validate checksums
- Generate formatted output

This is a real skill used in:
- Network security tools
- Packet sniffers
- Firewalls
- VPN software
- Intrusion detection systems
- Malware analysis

Good luck! 🚀
