# Packet Header Parser Solution - Detailed Explanation

This document explains every part of the `02_c_solution.c` file line by line.

## Overview

The solution parses binary IP, TCP, and UDP packet headers using:
1. Struct definitions with `#pragma pack` to match binary layout
2. Bitwise operations to extract individual fields
3. Network byte order conversion functions
4. File I/O to read binary packet data

---

## Section 1: Headers and Includes

```c
#include <stdio.h>      // File I/O functions (fopen, fread, printf)
#include <stdlib.h>     // Standard library (could be used for malloc)
#include <string.h>     // String functions (strlen, strncpy)
#include <arpa/inet.h>  // Network functions (htons, ntohs, htonl, ntohl)
```

**Why each header?**
- `stdio.h`: We need `fopen()`, `fread()`, `printf()` for file I/O and output
- `string.h`: We use `strlen()` to check port name strings
- `arpa/inet.h`: Essential for byte order conversion functions
  - `ntohs()` - convert 16-bit network order to host order
  - `ntohl()` - convert 32-bit network order to host order

---

## Section 2: Struct Packing

```c
#pragma pack(push, 1)
```

**What this does:**
- `#pragma pack(1)` tells the compiler: "Do NOT add padding between struct members"
- `push` saves the current packing setting so we can restore it later

**Why it matters:**
- Binary packet data has NO gaps between fields
- By default, C aligns struct members (wastes space, adds padding)
- With `pack(1)`, a 20-byte IP header takes exactly 20 bytes in memory

**Example - struct padding:**
```c
// WITHOUT pragma pack:
struct {
    char a;      // 1 byte (followed by 3 bytes padding)
    int b;       // 4 bytes
}
// Total: 8 bytes (wasteful!)

// WITH pragma pack(1):
struct {
    char a;      // 1 byte (no padding)
    int b;       // 4 bytes
}
// Total: 5 bytes (perfect for binary data!)
```

---

## Section 3: IPv4 Header Structure

```c
typedef struct {
    unsigned char version_ihl;      // Version (4 bits) + IHL (4 bits)
    unsigned char dscp_ecn;         // DSCP (6 bits) + ECN (2 bits)
    unsigned short total_length;    // 16 bits
    unsigned short identification;  // 16 bits
    unsigned short flags_offset;    // Flags (3 bits) + Fragment Offset (13 bits)
    unsigned char ttl;              // 8 bits
    unsigned char protocol;         // 8 bits
    unsigned short header_checksum; // 16 bits
    unsigned int source_ip;         // 32 bits
    unsigned int dest_ip;           // 32 bits
} IPv4Header;
```

**Size: 20 bytes** (exactly as specified in RFC 791)

**Why `unsigned` types?**
- These are network fields (no negative numbers)
- `unsigned char` = 1 byte (8 bits)
- `unsigned short` = 2 bytes (16 bits)
- `unsigned int` = 4 bytes (32 bits)

**Why combined fields?**
- Some IP header fields don't align to byte boundaries
- Example: version is 4 bits, IHL is 4 bits → both fit in one byte
- We extract them using bitwise operations

---

## Section 4: Bit Extraction Functions

### Example 1: Extract Version

```c
unsigned char get_ip_version(unsigned char version_ihl) {
    return (version_ihl >> 4) & 0xF;
}
```

**Breakdown:**
1. `version_ihl` contains: `[VERSION(4 bits)][IHL(4 bits)]`
   - Binary example: `01000101` (version=4, IHL=5)

2. `>> 4` shifts right 4 positions
   - `01000101 >> 4` = `00000100`
   - The version bits are now in the low position

3. `& 0xF` masks to keep only 4 bits
   - `00000100 & 00001111` = `00000100` = 4
   - Any bits above position 3 are cleared

**Result:** Version = 4 ✓

---

### Example 2: Extract IHL

```c
unsigned char get_ihl(unsigned char version_ihl) {
    return version_ihl & 0xF;
}
```

**Breakdown:**
1. `version_ihl` = `01000101` (version=4, IHL=5)
2. `& 0xF` keeps only low 4 bits
3. `01000101 & 00001111` = `00000101` = 5
4. The IHL bits are already in the right position!

**Result:** IHL = 5 ✓

---

### Example 3: Extract DSCP (6 bits)

```c
unsigned char get_dscp(unsigned char dscp_ecn) {
    return (dscp_ecn >> 2) & 0x3F;
}
```

**Breakdown:**
1. `dscp_ecn` = `DSCP(6 bits)` + `ECN(2 bits)`
2. `>> 2` shifts right 2 positions (moving DSCP to low position)
3. `& 0x3F` masks to 6 bits (binary: 111111)

---

### Example 4: TCP Flags Extraction

```c
void get_tcp_flags(unsigned char flags,
                   int *fin, int *syn, int *rst,
                   int *psh, int *ack, int *urg) {
    *fin = (flags >> 0) & 1;   // Bit 0
    *syn = (flags >> 1) & 1;   // Bit 1
    *rst = (flags >> 2) & 1;   // Bit 2
    *psh = (flags >> 3) & 1;   // Bit 3
    *ack = (flags >> 4) & 1;   // Bit 4
    *urg = (flags >> 5) & 1;   // Bit 5
}
```

**Example:**
- `flags` = `00010010` (SYN and ACK flags set)
- `(flags >> 1) & 1` = `(00001001) & 1` = `1` → SYN is set ✓
- `(flags >> 4) & 1` = `(00000001) & 1` = `1` → ACK is set ✓

**Note:** We use pointers (`*fin`, `*syn`, etc.) because C functions can't return multiple values directly.

---

## Section 5: IP Address Formatting

```c
void format_ip_address(unsigned int ip_bytes, char *buffer, size_t buffer_size) {
    unsigned int host_ip = ntohl(ip_bytes);
    unsigned char *bytes = (unsigned char *)&host_ip;
    snprintf(buffer, buffer_size, "%d.%d.%d.%d",
             bytes[0], bytes[1], bytes[2], bytes[3]);
}
```

**Breakdown:**

1. **`ntohl(ip_bytes)`** - Convert from network byte order (big-endian) to host byte order
   - Network: `C0 A8 01 01` (big-endian)
   - Host: `01 01 A8 C0` (little-endian, on x86)

2. **Cast to `unsigned char *`** - Get pointer to individual bytes
   - This lets us access each byte separately

3. **Extract bytes:**
   - `bytes[0]` = 192
   - `bytes[1]` = 168
   - `bytes[2]` = 1
   - `bytes[3]` = 1

4. **Format with `snprintf()`** - Create string "192.168.1.1"

**Key insight:** By converting to host byte order first, we ensure the bytes are in the right order for our architecture.

---

## Section 6: Port and Protocol Lookup

### Port Names

```c
const char* get_port_name(unsigned short port) {
    switch (port) {
        case 80:   return "HTTP";
        case 443:  return "HTTPS";
        case 22:   return "SSH";
        case 21:   return "FTP";
        case 53:   return "DNS";
        // ... more ports ...
        default:   return "";
    }
}
```

**Why?** Makes output more readable. Instead of "Destination Port: 80", we can print "Destination Port: 80 (HTTP)"

---

## Section 7: Main Parsing Function

```c
void parse_ipv4_header(FILE *file, const char *filename) {
    IPv4Header ip_header;
```

**Step 1: Read the IPv4 header from file**

```c
    if (fread(&ip_header, sizeof(IPv4Header), 1, file) != 1) {
        fprintf(stderr, "Error: Could not read IPv4 header\n");
        return;
    }
```

**Breakdown:**
- `fread(&ip_header, sizeof(IPv4Header), 1, file)`
  - Read 1 struct of size 20 bytes from file
  - Store in memory at address `&ip_header`
  - This reads exactly 20 bytes and fills our struct perfectly!

- `!= 1` checks if we actually read 1 item
  - If file is too short, `fread()` returns 0 and we error out

---

**Step 2: Extract all fields**

```c
    unsigned char version = get_ip_version(ip_header.version_ihl);
    unsigned char ihl = get_ihl(ip_header.version_ihl);
    unsigned char dscp = get_dscp(ip_header.dscp_ecn);
    // ... etc ...
    unsigned short total_len = ntohs(ip_header.total_length);
    unsigned short checksum = ntohs(ip_header.header_checksum);
```

**Key points:**
- Bit-packed fields use helper functions: `get_version()`, `get_ihl()`, etc.
- Multi-byte fields use `ntohs()` or `ntohl()` for byte order conversion
- We store everything in separate variables for clarity

---

**Step 3: Convert IP addresses**

```c
    char src_ip_str[16], dst_ip_str[16];
    format_ip_address(ip_header.source_ip, src_ip_str, sizeof(src_ip_str));
    format_ip_address(ip_header.dest_ip, dst_ip_str, sizeof(dst_ip_str));
```

**Why separate buffer?**
- IP address string needs 15 characters max ("255.255.255.255" + null terminator)
- Easier to use separate variables than `printf()` directly from struct

---

**Step 4: Display IPv4 header**

```c
    printf("--- IP Header (IPv%d) ---\n", version);
    printf("Version: %d\n", version);
    printf("Header Length (IHL): %d words (%d bytes)\n", ihl, ihl * 4);
    // ... more fields ...
```

**Note:** IHL is in "words" (4-byte units), so we multiply by 4 to get bytes.

---

**Step 5: Parse TCP/UDP based on protocol**

```c
    if (ip_header.protocol == 6) {  // TCP
        TCPHeader tcp_header;
        if (fread(&tcp_header, sizeof(TCPHeader), 1, file) != 1) {
            fprintf(stderr, "Error: Could not read TCP header\n");
            return;
        }
```

**Same pattern:**
1. Check protocol type
2. Read the appropriate header struct
3. Extract fields using bit operations and byte order conversion
4. Display formatted output

---

## Section 8: Key Concepts Summary

### Byte Order Conversion

```
Network byte order (big-endian):    Host byte order (little-endian on x86):
Most significant byte first          Least significant byte first

Port 80:                             Port 80:
0x00 0x50                            0x50 0x00
```

**Functions:**
- `ntohs()` - "Network TO Host Short" (16-bit)
- `ntohl()` - "Network TO Host Long" (32-bit)

---

### Struct vs Binary Data

```c
Binary packet:  [V|IHL][DSCP|ECN][LENGTH      ][ID        ][FLAGS|OFFSET  ]
                [TTL  ][PROTOCOL ][CHECKSUM   ][SOURCE IP         ][DEST IP       ]

C Struct:
    version_ihl    (1 byte)
    dscp_ecn       (1 byte)
    total_length   (2 bytes)
    identification (2 bytes)
    flags_offset   (2 bytes)
    ttl            (1 byte)
    protocol       (1 byte)
    header_checksum(2 bytes)
    source_ip      (4 bytes)
    dest_ip        (4 bytes)
    ─────────────
    Total: 20 bytes ✓
```

---

### Bitwise Operations Summary

| Operation | Symbol | Example | Result |
|-----------|--------|---------|--------|
| AND | `&` | `0110 & 0011` | `0010` |
| OR | `\|` | `0110 \| 0011` | `0111` |
| XOR | `^` | `0110 ^ 0011` | `0101` |
| NOT | `~` | `~0110` | `1001` |
| Left shift | `<<` | `0011 << 2` | `1100` |
| Right shift | `>>` | `1100 >> 2` | `0011` |

**For bit extraction:**
```
Shift right to move target bit to position 0
AND with mask to keep only that bit (or those bits)

Example: Extract bits 2-5 from byte 11001010
(11001010 >> 2) & 00001111 = 00110010 & 00001111 = 00000010 = 2 ✓
```

---

## Section 9: Error Handling

The solution checks for:

1. **Command-line arguments**
   ```c
   if (argc != 2) {
       fprintf(stderr, "Usage: %s <packet_file.bin>\n", argv[0]);
       return 1;
   }
   ```

2. **File opening**
   ```c
   if (file == NULL) {
       perror("Error opening file");
       return 1;
   }
   ```

3. **File size**
   ```c
   if (file_size < (long)sizeof(IPv4Header)) {
       fprintf(stderr, "Error: File too small...\n");
       return 1;
   }
   ```

4. **Header reading**
   ```c
   if (fread(&ip_header, sizeof(IPv4Header), 1, file) != 1) {
       fprintf(stderr, "Error: Could not read IPv4 header\n");
       return 1;
   }
   ```

**Why so much error checking?**
- Binary parsing is fragile - wrong assumptions crash the program
- With proper checking, we give useful error messages
- This is how production code handles untrusted input

---

## Section 10: Compilation and Testing

### Compile
```bash
gcc -Wall -g 02_c_solution.c -o parser
```

**Flags:**
- `-Wall` - Enable all warnings
- `-g` - Include debug symbols (for debugger)
- `-o parser` - Output filename

### Run
```bash
./parser sample_packet.bin
```

### Debug
```bash
# View binary file
hexdump -C sample_packet.bin

# Run with debugger
gdb ./parser
(gdb) run sample_packet.bin
```

---

## Section 11: Common Mistakes and Fixes

### Mistake 1: Forgetting pragma pack

```c
// WRONG - struct has padding
typedef struct { ... } IPv4Header;
// Size: 24 bytes instead of 20!

// CORRECT
#pragma pack(push, 1)
typedef struct { ... } IPv4Header;
#pragma pack(pop)
// Size: exactly 20 bytes ✓
```

### Mistake 2: Not converting byte order

```c
// WRONG - shows garbage
unsigned short port = header.source_port;  // 0x1234 instead of 0x3412

// CORRECT
unsigned short port = ntohs(header.source_port);  // Proper conversion
```

### Mistake 3: Wrong bit positions

```c
// WRONG - gets low 4 bits instead of high 4 bits
int version = version_ihl & 0xF;  // Gets IHL, not version!

// CORRECT
int version = (version_ihl >> 4) & 0xF;  // Shifts first, then masks
```

### Mistake 4: Not checking fread return value

```c
// WRONG - crashes if file is too short
fread(&header, sizeof(header), 1, file);
unsigned char version = get_ip_version(header.version_ihl);  // Uninitialized!

// CORRECT
if (fread(&header, sizeof(header), 1, file) != 1) {
    fprintf(stderr, "Error reading header\n");
    return 1;
}
```

---

## Learning Resources

### To understand IPv4 better:
- RFC 791: https://tools.ietf.org/html/rfc791
- Beej's Guide: https://beej.us/guide/bgnet/

### To understand TCP better:
- RFC 793: https://tools.ietf.org/html/rfc793

### Bitwise operations:
- Khan Academy: "Bits and Bytes"
- GeeksforGeeks: "Bitwise Operators in C"

---

## Testing Checklist

- [ ] Understand what each bit extraction function does
- [ ] Understand byte order conversion functions
- [ ] Understand IPv4 header structure
- [ ] Understand TCP header structure
- [ ] Can explain what `#pragma pack` does
- [ ] Can compile without warnings
- [ ] Can run on sample_packet.bin
- [ ] Output matches expected format
- [ ] Can handle invalid files gracefully
- [ ] Can trace through with debugger

Once you check all boxes, you're ready to write your own version!
