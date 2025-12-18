# Project 2: Packet Header Parser
## Binary Network Packet Analysis in C

**Objective:** Parse binary network packet headers (IP, TCP, UDP) to understand how network protocols work at the bit level, learn binary data handling, and practice low-level data manipulation in C.

**What You'll Learn:**
- Binary data representation and byte order
- Bitwise operations and bit extraction
- Struct design for binary data
- Network protocols (IP, TCP, UDP)
- Endianness and network byte order
- Safe pointer casting and memory alignment
- Reading and interpreting binary files

---

## Project Structure

```
02_Packet_Header_Parser/
├── PROJECT_GUIDE.md              (This file)
├── LESSON_PLAN.md                (Complete learning path with exercises)
├── 02_starter.c                  (Your starting template)
├── 02_c_solution.c               (Complete reference solution)
├── 02_c_solution_explained.md     (Detailed explanation)
├── sample_packet.bin              (Binary packet data for testing)
├── generate_sample_packet.c       (Create test data)
├── Makefile                       (Build automation)
└── README.md                      (Quick reference)
```

---

## The Challenge

### Problem Statement

Write a program that:

1. **Reads binary packet data** from a file
2. **Parses IP headers** and extracts:
   - IP version and header length (IHL)
   - DSCP/ECN values
   - Total packet length
   - TTL (Time to Live)
   - Protocol type
   - Source and destination IP addresses
   - Header checksum

3. **Detects and parses TCP headers** if protocol is 6:
   - Source and destination ports
   - Sequence and acknowledgment numbers
   - Data offset
   - TCP flags (SYN, ACK, FIN, RST, PSH, URG)
   - Window size
   - Checksum

4. **Detects and parses UDP headers** if protocol is 17:
   - Source and destination ports
   - Length
   - Checksum

5. **Displays formatted output** showing all parsed values
6. **Handles errors gracefully** (invalid files, truncated packets, etc.)

### Example Input/Output

**Input File:** `sample_packet.bin` (binary packet data)

**Expected Output:**
```
=== Packet Header Parser ===
File: sample_packet.bin
File size: 60 bytes

--- IP Header (IPv4) ---
Version: 4
Header Length (IHL): 5 words (20 bytes)
DSCP: 0
ECN: 0
Total Packet Length: 60 bytes
Identification: 0x1234
More Fragments: No
Don't Fragment: Yes
Fragment Offset: 0
TTL: 64
Protocol: 6 (TCP)
Header Checksum: 0x1a2b
Source IP: 192.168.1.100
Destination IP: 10.0.0.50

--- TCP Header ---
Source Port: 54321
Destination Port: 80 (HTTP)
Sequence Number: 0x12345678
Acknowledgment Number: 0x87654321
Data Offset: 5 words (20 bytes)
Reserved: 0
Flags: SYN ACK
  - FIN: No
  - SYN: Yes
  - RST: No
  - PSH: No
  - ACK: Yes
  - URG: No
Window Size: 32768
Checksum: 0x0000 (in header - would need validation)
Urgent Pointer: 0

--- Payload ---
Remaining bytes: 0
```

---

## Your Task

### Step 1: Complete the Lesson Plan

Before writing code, work through **LESSON_PLAN.md** and complete all exercises:

- [ ] Module 1: Binary Data Fundamentals (Exercises 1.1, 1.2)
- [ ] Module 2: Bitwise Operations (Exercises 2.1, 2.2, 2.3)
- [ ] Module 3: Structs and Memory Layout (Exercises 3.1, 3.2)
- [ ] Module 4: Network Byte Order (Exercises 4.1, 4.2)
- [ ] Module 5: Parsing Binary Data (Exercises 5.1, 5.2)

**Why?** Each lesson builds essential skills you'll need. Don't skip!

### Step 2: Review Network Protocol Specifications

Read the simplified headers below to understand the packet structure:

#### IPv4 Header (minimum 20 bytes)
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |DSCP |ECN|            Total Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|      Fragment Offset     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |         Header Checksum       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

#### TCP Header (minimum 20 bytes)
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Data  |       |C|E|U|A|P|R|S|F|                               |
|Offset | Rsrvd|W|C|R|C|S|S|Y|I|            Window Size          |
|       |      |R|E|G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum           |       Urgent Pointer           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

#### UDP Header (8 bytes)
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |           Checksum            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

### Step 3: Design Your Structs

Using `#pragma pack`, create C structs that match the binary packet layout:

```c
#pragma pack(push, 1)

// IPv4 Header (20 bytes minimum)
typedef struct {
    // YOUR CODE HERE
} IPv4Header;

// TCP Header (20 bytes minimum)
typedef struct {
    // YOUR CODE HERE
} TCPHeader;

// UDP Header (8 bytes)
typedef struct {
    // YOUR CODE HERE
} UDPHeader;

#pragma pack(pop)
```

**Hints:**
- Use `unsigned char` for single bytes
- Use `unsigned short` for 2-byte fields (remember byte order!)
- Use `unsigned int` for 4-byte fields (remember byte order!)
- For bit fields, combine multiple bits into single bytes
- Remember: IP addresses are 4 bytes, ports are 2 bytes

### Step 4: Implement Bit Extraction Functions

Create helper functions to extract bits from bytes:

```c
// Extract version (top 4 bits of first byte)
unsigned char get_ip_version(unsigned char version_ihl) {
    return (version_ihl >> 4) & 0xF;
}

// Extract IHL (bottom 4 bits of first byte)
unsigned char get_ihl(unsigned char version_ihl) {
    return version_ihl & 0xF;
}

// Extract DSCP (top 6 bits of second byte)
unsigned char get_dscp(unsigned char dscp_ecn) {
    return (dscp_ecn >> 2) & 0x3F;
}

// Extract TCP flags from flags byte
void extract_tcp_flags(unsigned char flags,
                       int *fin, int *syn, int *rst,
                       int *psh, int *ack, int *urg) {
    *fin = (flags >> 0) & 1;
    *syn = (flags >> 1) & 1;
    *rst = (flags >> 2) & 1;
    *psh = (flags >> 3) & 1;
    *ack = (flags >> 4) & 1;
    *urg = (flags >> 5) & 1;
}
```

### Step 5: Implement IP Address Conversion

Convert 4-byte IP address to dotted decimal:

```c
void print_ip_address(unsigned int ip_bytes) {
    unsigned char *bytes = (unsigned char *)&ip_bytes;
    printf("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}
```

**Note:** Handle byte order correctly!

### Step 6: Parse the Packet

```c
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <packet.bin>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    // Step 1: Read IPv4 header
    IPv4Header ip_header;
    if (fread(&ip_header, sizeof(IPv4Header), 1, file) != 1) {
        fprintf(stderr, "Error: Could not read IP header\n");
        fclose(file);
        return 1;
    }

    // Step 2: Validate and display IPv4 header
    unsigned char version = get_ip_version(ip_header.version_ihl);
    if (version != 4) {
        fprintf(stderr, "Error: Only IPv4 supported, got version %d\n", version);
        fclose(file);
        return 1;
    }

    // Display IP header information
    printf("=== Packet Header Parser ===\n");
    printf("--- IP Header (IPv4) ---\n");
    printf("Version: %d\n", version);
    // ... more fields ...

    // Step 3: Check protocol and read next header
    unsigned char protocol = ip_header.protocol;

    if (protocol == 6) {  // TCP
        TCPHeader tcp_header;
        if (fread(&tcp_header, sizeof(TCPHeader), 1, file) != 1) {
            fprintf(stderr, "Error: Could not read TCP header\n");
            fclose(file);
            return 1;
        }
        // Display TCP header
        // ...
    } else if (protocol == 17) {  // UDP
        UDPHeader udp_header;
        if (fread(&udp_header, sizeof(UDPHeader), 1, file) != 1) {
            fprintf(stderr, "Error: Could not read UDP header\n");
            fclose(file);
            return 1;
        }
        // Display UDP header
        // ...
    }

    fclose(file);
    return 0;
}
```

### Step 7: Test and Validate

```bash
# Compile
gcc -Wall -g 02_starter.c -o packet_parser

# Run with sample packet
./packet_parser sample_packet.bin

# Compare with solution
./02_c_solution sample_packet.bin
```

---

## Compilation and Running

### Using gcc directly

```bash
# Compile with all warnings
gcc -Wall -g 02_your_solution.c -o packet_parser

# Run with sample packet
./packet_parser sample_packet.bin

# Create your own test packet
gcc -Wall -g generate_sample_packet.c -o gen_packet
./gen_packet packet.bin
./packet_parser packet.bin
```

### Using the Makefile

```bash
# Build
make

# Run with sample
make run

# Clean up
make clean

# Build and run tests
make test
```

---

## Files Provided

### Learning Materials
- **`LESSON_PLAN.md`** - Complete lesson with 5 modules and exercises
- **`PROJECT_GUIDE.md`** - This file

### Code Files
- **`02_starter.c`** - Template with structure and hints (start here)
- **`02_c_solution.c`** - Complete reference solution
- **`02_c_solution_explained.md`** - Line-by-line explanation
- **`generate_sample_packet.c`** - Utility to create test packets

### Test Data
- **`sample_packet.bin`** - Pre-made binary packet (IPv4 + TCP)

### Build
- **`Makefile`** - Build automation

---

## Learning Path

### Prerequisites
- Complete Project 1 (Python Script Rewrite)
- Comfortable with functions, loops, structs in C
- Basic understanding of files and binary data

### During This Project
- Learn binary representation and byte order
- Master bitwise operations
- Understand network protocols at bit level
- Practice safe pointer casting
- Work with binary files

### After This Project
- Ready for Project 3 (Binary Data Converter)
- Ready for network programming in Phase 2
- Can parse any binary file format
- Can write network tools and analyzers

---

## Common Challenges & Solutions

### Challenge 1: Byte Order Confusion

**Problem:** Port numbers and IPs appear as large garbage values

**Solution:**
```c
// Remember: Network data is big-endian!
unsigned short port = ntohs(tcp_header.dest_port);

// For 4-byte values
unsigned int ip = ntohl(ip_header.source_ip);
```

### Challenge 2: Struct Padding

**Problem:** Reading struct from file works sometimes, then crashes

**Solution:**
```c
// ALWAYS use pragma pack for binary data
#pragma pack(push, 1)
typedef struct { ... } IPv4Header;
#pragma pack(pop)

// Verify size matches expected
if (sizeof(IPv4Header) != 20) {
    printf("ERROR: IPv4Header size is %zu, expected 20!\n", sizeof(IPv4Header));
}
```

### Challenge 3: Bit Extraction

**Problem:** Flags and version fields show wrong values

**Solution:**
```c
// Be careful about bit order!
int version = (byte >> 4) & 0xF;  // Top 4 bits
int ihl = byte & 0xF;              // Bottom 4 bits

// For multi-byte fields, remember byte order
unsigned short port = ntohs(*(unsigned short*)data);
```

### Challenge 4: File Handling

**Problem:** File operations sometimes fail silently

**Solution:**
```c
// Always check return values
FILE *file = fopen(argv[1], "rb");
if (file == NULL) {
    perror("fopen failed");  // Shows actual error
    return 1;
}

// Check read operations
if (fread(&header, sizeof(header), 1, file) != 1) {
    perror("fread failed");
    fclose(file);
    return 1;
}
```

---

## Testing Strategy

### Test 1: Parse Sample Packet
```bash
./packet_parser sample_packet.bin
```
Should display IPv4 and TCP headers correctly.

### Test 2: Compare with Solution
```bash
./02_c_solution sample_packet.bin > expected.txt
./packet_parser sample_packet.bin > actual.txt
diff expected.txt actual.txt
```

### Test 3: Test Invalid Files
```bash
# Empty file
touch empty.bin
./packet_parser empty.bin
# Should handle gracefully

# Truncated header
echo "ABC" > truncated.bin
./packet_parser truncated.bin
# Should handle gracefully
```

### Test 4: Test with Real Packets (Advanced)
```bash
# Capture actual packets
sudo tcpdump -i eth0 -c 1 -w real_packet.pcap

# Extract first packet (pcap header is 24 bytes)
dd if=real_packet.pcap of=raw_packet.bin skip=24 bs=1

./packet_parser raw_packet.bin
```

---

## Hints & Tips

### Debugging Binary Data

```c
// Print hex dump of bytes
void print_hex(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

// Check struct sizes before parsing
printf("IPv4Header size: %zu (expected 20)\n", sizeof(IPv4Header));
printf("TCPHeader size: %zu (expected 20)\n", sizeof(TCPHeader));
```

### Understanding IP Addresses

```c
// IP address is stored as 4 bytes
// 192.168.1.1 in network byte order looks like:
// C0 A8 01 01  (hex)

// To print correctly:
unsigned int ip = ip_header.destination_ip;
unsigned char *bytes = (unsigned char *)&ip;
printf("%d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
```

### Common Port Numbers
- 80: HTTP
- 443: HTTPS
- 22: SSH
- 21: FTP
- 25: SMTP
- 53: DNS
- 3306: MySQL

---

## Extensions (After Completing Basic Project)

1. **Checksum Validation**
   - Implement IP header checksum verification
   - Implement TCP/UDP pseudo-header checksum

2. **Multiple Packets**
   - Parse file containing multiple packets
   - Display packet statistics (count, types, bytes)

3. **ICMP Support**
   - Add ICMP header parsing
   - Detect ping requests/replies

4. **IPv6 Support**
   - Parse IPv6 headers (more complex!)
   - Handle different address format

5. **Packet Flow Analysis**
   - Track connection states
   - Build flow statistics
   - Detect common attack patterns

6. **PCAP File Support**
   - Read standard pcap format
   - Parse multiple packets from tcpdump file

7. **Pretty Printing**
   - Add color output
   - Create ASCII art packet diagrams
   - Export to JSON or CSV

---

## Learning Milestones

Track your progress:

- [ ] Complete Lesson Plan (all 5 modules)
- [ ] Understand IPv4 header structure
- [ ] Understand TCP header structure
- [ ] Understand UDP header structure
- [ ] Design C structs for headers
- [ ] Implement bit extraction functions
- [ ] Read binary file correctly
- [ ] Parse IPv4 header from file
- [ ] Display parsed IPv4 fields
- [ ] Detect protocol type
- [ ] Parse TCP header when protocol == 6
- [ ] Parse UDP header when protocol == 17
- [ ] Extract and display TCP flags
- [ ] Handle errors gracefully
- [ ] Match reference solution output exactly
- [ ] Test with multiple packets
- [ ] Complete at least one extension

---

## Performance Notes

This is a simple parser, so performance isn't critical. However:

- `fopen()` + `fread()` is fast enough for single packets
- For processing many packets, consider memory-mapped files
- Struct access is O(1) - very fast
- Bitwise operations are single CPU cycles

---

## Safety Considerations

When working with binary data:

1. **Always validate** file size before reading
2. **Check return values** from fread/fopen
3. **Use bounds checking** when calculating offsets
4. **Verify struct sizes** with sizeof()
5. **Handle truncated packets** gracefully
6. **Never assume** file format without validation

```c
// Example safe reading
if (fstat(fileno(file), &stat_info) == -1) {
    perror("fstat");
    return 1;
}

if (stat_info.st_size < sizeof(IPv4Header)) {
    fprintf(stderr, "Error: File too small for IPv4 header\n");
    return 1;
}
```

---

## Resources

### RFCs (Protocol Specifications)
- [RFC 791 - IP Protocol](https://tools.ietf.org/html/rfc791)
- [RFC 793 - TCP Protocol](https://tools.ietf.org/html/rfc793)
- [RFC 768 - UDP Protocol](https://tools.ietf.org/html/rfc768)
- [RFC 4291 - IPv6 Protocol](https://tools.ietf.org/html/rfc4291)

### Books
- "The TCP/IP Illustrated" series (W. Richard Stevens)
- "Network Warrior" (Gary A. Donahue)
- "Wireshark Network Analysis" (Laura Chappell)

### Tools
- **wireshark** - Visual packet analyzer
- **tcpdump** - Command-line packet capture
- **hexdump** - View binary files
- **scapy** - Python packet builder (for test data)

---

## Next Steps

1. **Before starting:** Work through LESSON_PLAN.md completely
2. **While coding:** Refer to protocol specifications
3. **When stuck:** Check debugging tips section
4. **After completion:** Try one of the extensions
5. **Ready for Project 3:** Binary Data Converter (more binary manipulation)

---

## Success Criteria

Your solution should:

- ✅ Parse IPv4 headers correctly
- ✅ Extract all header fields accurately
- ✅ Handle byte order properly
- ✅ Parse TCP headers (protocol == 6)
- ✅ Parse UDP headers (protocol == 17)
- ✅ Extract TCP flags correctly
- ✅ Handle errors gracefully
- ✅ Display output matching expected format
- ✅ Work with sample_packet.bin
- ✅ Match reference solution output

---

Good luck! You're doing real network programming now! 🚀

Questions? Review LESSON_PLAN.md or check the reference solution in 02_c_solution_explained.md
