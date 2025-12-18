# Binary Data Converter - Complete Lesson Plan

## How to Use This Guide

This lesson plan has 6 progressive modules that build on each other. Each module includes:
- **Concepts:** What you need to understand
- **Code Examples:** How to implement it in C
- **Exercises:** Hands-on practice with complete solutions
- **Key Takeaways:** What to remember

**Time estimate:** 8-12 hours total (can be done in segments)

---

## Module 1: Number System Foundations (1-2 hours)

### 1.1 Why We Need Different Number Systems

Humans naturally think in **decimal (base 10)** because we have 10 fingers.
Computers natively work in **binary (base 2)** because transistors are on/off.

Different bases are useful for:
- **Binary:** Exact representation of computer memory
- **Hexadecimal:** Shorthand for binary (4 bits = 1 hex digit)
- **Decimal:** Human-friendly communication
- **Octal:** Legacy systems (file permissions)

### 1.2 Understanding Number Bases

Each digit's position represents a power of the base:

**Decimal (base 10):**
```
255 = 2×10² + 5×10¹ + 5×10⁰
    = 2×100 + 5×10 + 5×1
    = 200 + 50 + 5
```

**Binary (base 2):**
```
11111111 = 1×2⁷ + 1×2⁶ + 1×2⁵ + 1×2⁴ + 1×2³ + 1×2² + 1×2¹ + 1×2⁰
         = 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1
         = 255
```

**Hexadecimal (base 16):**
```
0xFF = 15×16¹ + 15×16⁰
     = 240 + 15
     = 255

In hex, digits are: 0,1,2,3,4,5,6,7,8,9,A(10),B(11),C(12),D(13),E(14),F(15)
```

**Octal (base 8):**
```
0377 = 3×8² + 7×8¹ + 7×8⁰
     = 3×64 + 7×8 + 7×1
     = 192 + 56 + 7
     = 255
```

### 1.3 Key Relationships

| Decimal | Binary | Hex | Octal |
|---------|--------|-----|-------|
| 0 | 00000000 | 0x00 | 0 |
| 1 | 00000001 | 0x01 | 1 |
| 15 | 00001111 | 0x0F | 17 |
| 16 | 00010000 | 0x10 | 20 |
| 127 | 01111111 | 0x7F | 177 |
| 128 | 10000000 | 0x80 | 200 |
| 255 | 11111111 | 0xFF | 377 |

**Pattern:** One hex digit = 4 binary digits
- 0xF = 1111 (binary)
- 0xA = 1010 (binary)
- 0xFF = 11111111 (binary)

### 1.4 Powers of 2 (Essential Memory Aid)

```
2⁰ = 1
2¹ = 2
2² = 4
2³ = 8
2⁴ = 16
2⁵ = 32
2⁶ = 64
2⁷ = 128
2⁸ = 256
2¹⁰ = 1024 (1 KB)
2¹⁶ = 65536
2³² = 4,294,967,296 (max uint32)
```

### Exercise 1.1: Manual Conversions

**Convert decimal 42 to binary, hex, octal (without calculator)**

Solution approach:
1. **Decimal to Binary:** Find which powers of 2 sum to 42
   - 42 = 32 + 8 + 2 = 2⁵ + 2³ + 2¹
   - Binary: 00101010

2. **Decimal to Hex:** Group binary into 4-bit chunks
   - 00101010 = 0010 | 1010 = 0x2A

3. **Decimal to Octal:** Group binary into 3-bit chunks
   - 00101010 = 000 | 101 | 010 = 052

**Answer:** 42₁₀ = 101010₂ = 0x2A₁₆ = 052₈

### Exercise 1.2: Understanding Bit Positions

```c
// Understanding which bits are set
unsigned char value = 42;  // Binary: 00101010

// Which bits are set (equal 1)?
// Bit 7: 0 (off)
// Bit 6: 0 (off)
// Bit 5: 1 (ON) ← contributes 32
// Bit 4: 0 (off)
// Bit 3: 1 (ON) ← contributes 8
// Bit 2: 0 (off)
// Bit 1: 1 (ON) ← contributes 2
// Bit 0: 0 (off)

// Total: 32 + 8 + 2 = 42

// In C, test individual bits with bitwise AND
int is_bit_set = (value >> 5) & 1;  // Returns 1 if bit 5 is set
```

### 1.5 Key Takeaways

- Each base is just a different way to represent the same value
- Position = power of the base
- Binary is how computers store data
- Hex is shorthand for binary
- Understanding powers of 2 is essential

---

## Module 2: Bitwise Operations for Conversions (1.5-2 hours)

### 2.1 The Six Bitwise Operators

**AND (`&`):** Both bits must be 1
```c
0b1100 & 0b1010 = 0b1000  (only bit 3 is 1 in both)
```

**OR (`|`):** At least one bit must be 1
```c
0b1100 | 0b1010 = 0b1110  (bits 1,2,3 are 1 in at least one)
```

**XOR (`^`):** Bits must be different
```c
0b1100 ^ 0b1010 = 0b0110  (bits 1,2 are different)
```

**NOT (`~`):** Flip all bits
```c
~0b1100 = 0b0011 (assuming 4-bit, all bits flip)
```

**Left Shift (`<<`):** Move bits left, fill with 0
```c
0b0011 << 2 = 0b1100  (shifted left 2 positions)
// Equivalent to: multiply by 2²
```

**Right Shift (`>>`):** Move bits right, fill with 0 (unsigned) or sign bit (signed)
```c
0b1100 >> 2 = 0b0011  (shifted right 2 positions)
// Equivalent to: divide by 2²
```

### 2.2 Using These Operators for Conversion

**Pattern: Shift → AND → Extract**

The most important pattern for conversion is **shift + mask**:

```c
// Extract bits at positions [start, start+width)
uint32_t value = 0b11010110;

// Extract bits 5-3 (3 bits starting at position 3)
// Shift right 3 positions, then mask with 0b111 (3 ones)
uint32_t field = (value >> 3) & 0b111;  // Result: 0b101 = 5

// Extract single bit at position 2
int bit = (value >> 2) & 1;  // Result: 1
```

**Why this works:**
```
Original:     1 1 0 1 0 1 1 0
Positions:    7 6 5 4 3 2 1 0

To extract bits [5:3]:
Step 1 - Shift right 3:   0 0 0 1 1 0 1 0  (value >> 3)
Step 2 - Mask with 0b111: 0 0 0 0 1 0 1 = 0b101
```

### 2.3 Converting Decimal to Binary String

```c
// Convert decimal 42 to binary string "00101010"
void decimal_to_binary(uint32_t value, char *buffer) {
    // Iterate through 8 bits, from MSB (bit 7) to LSB (bit 0)
    for (int i = 7; i >= 0; i--) {
        // Extract bit at position i
        int bit = (value >> i) & 1;
        // Convert to character ('0' or '1')
        buffer[7 - i] = bit ? '1' : '0';
    }
    buffer[8] = '\0';  // Null terminate
}
```

**Step-by-step for value=42 (0b00101010):**
```
i=7: bit = (42 >> 7) & 1 = 0b00000000 & 1 = 0 → '0'
i=6: bit = (42 >> 6) & 1 = 0b00000000 & 1 = 0 → '0'
i=5: bit = (42 >> 5) & 1 = 0b00000001 & 1 = 1 → '1'
i=4: bit = (42 >> 4) & 1 = 0b00000010 & 1 = 0 → '0'
i=3: bit = (42 >> 3) & 1 = 0b00000101 & 1 = 1 → '1'
i=2: bit = (42 >> 2) & 1 = 0b00001010 & 1 = 0 → '0'
i=1: bit = (42 >> 1) & 1 = 0b00010101 & 1 = 1 → '1'
i=0: bit = (42 >> 0) & 1 = 0b00101010 & 1 = 0 → '0'

Result: "00101010"
```

### 2.4 Converting Binary String to Decimal

```c
// Convert binary string "00101010" to decimal 42
uint32_t binary_to_decimal(const char *binary_str) {
    uint32_t result = 0;

    // Process each character
    for (int i = 0; binary_str[i] != '\0'; i++) {
        if (binary_str[i] != '0' && binary_str[i] != '1') {
            return 0;  // Invalid character
        }

        // Shift previous bits left and add new bit
        result = (result << 1) | (binary_str[i] - '0');
    }

    return result;
}
```

**Step-by-step for "00101010":**
```
Start: result = 0

'0': result = (0 << 1) | 0 = 0
'0': result = (0 << 1) | 0 = 0
'1': result = (0 << 1) | 1 = 1
'0': result = (1 << 1) | 0 = 2
'1': result = (2 << 1) | 1 = 5
'0': result = (5 << 1) | 0 = 10
'1': result = (10 << 1) | 1 = 21
'0': result = (21 << 1) | 0 = 42

Result: 42
```

### 2.5 Converting Decimal to Hexadecimal String

```c
// Convert decimal 255 to hex string "ff"
void decimal_to_hex(uint32_t value, char *buffer) {
    // Hex digits: 0-9, a-f
    const char *hex_digits = "0123456789abcdef";

    // For 32-bit value, we need up to 8 hex digits
    int index = 0;

    // Extract 4 bits at a time, starting from top
    for (int i = 28; i >= 0; i -= 4) {
        // Extract 4 bits (one hex digit)
        int digit = (value >> i) & 0xF;
        buffer[index++] = hex_digits[digit];
    }

    buffer[index] = '\0';
}
```

**Step-by-step for value=255 (0x000000FF):**
```
Binary: 00000000 00000000 00000000 11111111

Extract 4 bits at a time:
i=28: (255 >> 28) & 0xF = 0x0 → '0'
i=24: (255 >> 24) & 0xF = 0x0 → '0'
i=20: (255 >> 20) & 0xF = 0x0 → '0'
i=16: (255 >> 16) & 0xF = 0x0 → '0'
i=12: (255 >> 12) & 0xF = 0x0 → '0'
i=8:  (255 >> 8) & 0xF = 0x0 → '0'
i=4:  (255 >> 4) & 0xF = 0xF → 'f'
i=0:  (255 >> 0) & 0xF = 0xF → 'f'

Result: "000000ff"
```

### Exercise 2.1: Extract Bits

```c
// Given: unsigned char value = 0b11010110 (214)
// Extract the 3-bit field at positions [5:3]

// Solution:
int field = (value >> 3) & 0b111;  // Result: 0b101 = 5
```

### Exercise 2.2: Convert to Binary String

```c
// Convert decimal 73 to binary string
// 73 = 64 + 8 + 1 = 2^6 + 2^3 + 2^0
// Binary: 01001001

char binary[9];
uint32_t value = 73;
for (int i = 7; i >= 0; i--) {
    int bit = (value >> i) & 1;
    binary[7 - i] = bit ? '1' : '0';
}
binary[8] = '\0';
// Result: "01001001"
```

### 2.6 Key Takeaways

- Shift + AND = extract bits
- Loop + shift + mask = convert to/from binary
- Each hex digit = 4 binary digits
- Bitwise operations are the foundation for all conversions

---

## Module 3: Endianness and Byte Order (1.5-2 hours)

### 3.1 What is Endianness?

**The Problem:** When we store a multi-byte value (like 0x12345678), how should we arrange the bytes?

**Answer:** Two different ways!

**Big-Endian (Network Byte Order):**
- Most significant byte first
- Used by network protocols (why it's called "network byte order")
- Used by Motorola processors, PowerPC

```
Value: 0x12345678
Bytes in memory: 12 34 56 78
Address:        0  1  2  3
```

**Little-Endian:**
- Least significant byte first
- Used by Intel x86, ARM, most modern systems

```
Value: 0x12345678
Bytes in memory: 78 56 34 12
Address:        0  1  2  3
```

**The Confusing Part:** The value is the same (0x12345678), but the bytes in memory are different!

### 3.2 Detecting Your System's Endianness

```c
#include <stdio.h>
#include <stdint.h>

int is_big_endian(void) {
    uint32_t test = 0x12345678;
    unsigned char *bytes = (unsigned char *)&test;

    // If big-endian, first byte is 0x12
    // If little-endian, first byte is 0x78
    return bytes[0] == 0x12;
}

// Usage:
int main(void) {
    if (is_big_endian()) {
        printf("This system is big-endian\n");
    } else {
        printf("This system is little-endian\n");
    }
    return 0;
}
```

**On most systems (Intel/ARM), this prints: "This system is little-endian"**

### 3.3 Why Endianness Matters

Network protocols always use big-endian (network byte order). When you receive data over the network:

1. Data arrives in big-endian format
2. Your CPU is likely little-endian
3. You must convert!

Example: Receiving port 80 (0x0050) from network

```
Network sends (big-endian):  0x00 0x50
Your little-endian system reads: port = 0x5000  ← WRONG!

Must convert with ntohs():   port = ntohs(0x5000) = 0x0050 ← CORRECT
```

### 3.4 Byte Order Conversion Functions

**Built-in functions (from `<arpa/inet.h>`):**

```c
#include <arpa/inet.h>
#include <stdint.h>

// 16-bit values (2 bytes) - for ports
uint16_t htons(uint16_t hostshort);    // Host to Network Short
uint16_t ntohs(uint16_t netshort);     // Network to Host Short

// 32-bit values (4 bytes) - for IP addresses
uint32_t htonl(uint32_t hostlong);     // Host to Network Long
uint32_t ntohl(uint32_t netlong);      // Network to Host Long
```

**How they work:**

```c
// On little-endian system:
uint16_t host_port = 0x0050;  // 80 decimal
uint16_t net_port = htons(host_port);  // Result: 0x5000

// To reverse:
uint16_t back = ntohs(net_port);  // Result: 0x0050
```

### 3.5 Manual Byte Swapping

Understanding the internals:

```c
// Manual byte swap for 32-bit value
uint32_t swap_bytes_32(uint32_t value) {
    return ((value & 0xFF000000) >> 24) |  // Byte 3 → Byte 0
           ((value & 0x00FF0000) >> 8)  |  // Byte 2 → Byte 1
           ((value & 0x0000FF00) << 8)  |  // Byte 1 → Byte 2
           ((value & 0x000000FF) << 24);   // Byte 0 → Byte 3
}

// Example: 0x12345678 → 0x78563412
uint32_t value = 0x12345678;
uint32_t swapped = swap_bytes_32(value);
// swapped = 0x78563412
```

**Step-by-step for 0x12345678:**
```
Original: 0x12345678 = 0001 0010 0011 0100 0101 0110 0111 1000
          Byte:       0    1    2    3

Extract byte 3 (0x78) and shift to position 0:
  (0x12345678 & 0xFF000000) >> 24 = 0x12000000 >> 24 = 0x00000012
  Wait, that's wrong. Let me recalculate:

  (0x12345678 & 0xFF000000) = 0x12000000
  0x12000000 >> 24 = 0x00000012

Hmm, let me think about this differently. The mask selects the byte:
  (value & 0x000000FF) = 0x78 (low byte)
  Shift left 24: 0x78000000 (now in high position)

So the correct formula is:
  ((value & 0xFF000000) >> 24) → gets byte 3, puts in byte 0
  ((value & 0x00FF0000) >> 8)  → gets byte 2, puts in byte 1
  ((value & 0x0000FF00) << 8)  → gets byte 1, puts in byte 2
  ((value & 0x000000FF) << 24) → gets byte 0, puts in byte 3

Example with 0x12345678:
  0x78 (was byte 0) → byte 3: 0x78 << 24 = 0x78000000
  0x56 (was byte 1) → byte 2: 0x56 << 8 = 0x00005600
  0x34 (was byte 2) → byte 1: 0x34 >> 8 = 0x00000034
  0x12 (was byte 3) → byte 0: 0x12 >> 24 = 0x00000000

Wait, that's still not right. Let me be more careful:

  0x12345678: Bytes are [12, 34, 56, 78]

  Select byte 0 (0x78): value & 0x000000FF = 0x78
  Shift to position 3: 0x78 << 24 = 0x78000000

  Select byte 1 (0x56): value & 0x0000FF00 = 0x5600
  Shift to position 2: 0x5600 << 8 = 0x560000

  Select byte 2 (0x34): value & 0x00FF0000 = 0x340000
  Shift to position 1: 0x340000 >> 8 = 0x3400

  Select byte 3 (0x12): value & 0xFF000000 = 0x12000000
  Shift to position 0: 0x12000000 >> 24 = 0x12

  Result: 0x78000000 | 0x560000 | 0x3400 | 0x12 = 0x78563412
```

### Exercise 3.1: Manual Byte Swap

```c
// Swap bytes in 0xABCDEF01
// Answer: 0x01EFCDAB

uint32_t value = 0xABCDEF01;
uint32_t swapped = ((value & 0xFF000000) >> 24) |
                   ((value & 0x00FF0000) >> 8)  |
                   ((value & 0x0000FF00) << 8)  |
                   ((value & 0x000000FF) << 24);
// swapped = 0x01EFCDAB
```

### 3.6 Key Takeaways

- Big-endian: most significant byte first (network order)
- Little-endian: least significant byte first (modern systems)
- Network protocols always use big-endian
- Use htonl/ntohl for 32-bit values
- Use htons/ntohs for 16-bit values
- Always convert when crossing system boundaries

---

## Module 4: Network Data Transformations (1.5-2 hours)

### 4.1 IP Address Conversions

IP addresses have two representations:

**Text Form:** "192.168.1.1"
**Binary Form:** 0xC0A80101 (32-bit integer, network byte order)

**Quick reference:**
```
192 = 0xC0
168 = 0xA8
1   = 0x01
1   = 0x01
Result: 0xC0A80101
```

### 4.2 Parsing IP Addresses from Strings

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t parse_ip_string(const char *ip_str) {
    unsigned char a, b, c, d;

    // Parse four octets
    if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d) != 4) {
        return 0;  // Invalid format
    }

    // Combine into 32-bit value and convert to network byte order
    uint32_t ip = htonl((a << 24) | (b << 16) | (c << 8) | d);

    return ip;
}
```

**Step-by-step for "192.168.1.1":**
```
Parse: a=192, b=168, c=1, d=1

Combine:
  (192 << 24) = 0xC0000000
  (168 << 16) = 0x00A80000
  (1 << 8)    = 0x00000100
  (1)         = 0x00000001
  OR together: 0xC0A80101

Convert to network byte order:
  htonl(0xC0A80101) = 0xC0A80101 (already big-endian on network, but value is correct)
```

### 4.3 Formatting IP Addresses for Display

```c
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

void format_ip_address(uint32_t ip_binary, char *buffer, size_t buffer_size) {
    // Convert from network byte order to host byte order
    uint32_t host_ip = ntohl(ip_binary);

    // Extract individual octets
    unsigned char a = (host_ip >> 24) & 0xFF;
    unsigned char b = (host_ip >> 16) & 0xFF;
    unsigned char c = (host_ip >> 8) & 0xFF;
    unsigned char d = (host_ip) & 0xFF;

    // Format as string
    snprintf(buffer, buffer_size, "%u.%u.%u.%u", a, b, c, d);
}
```

**Step-by-step for 0xC0A80101:**
```
Convert from network: ntohl(0xC0A80101) = 0xC0A80101 (value)

Extract bytes:
  a = (0xC0A80101 >> 24) & 0xFF = 0xC0 = 192
  b = (0xC0A80101 >> 16) & 0xFF = 0xA8 = 168
  c = (0xC0A80101 >> 8) & 0xFF  = 0x01 = 1
  d = (0xC0A80101) & 0xFF       = 0x01 = 1

Format: "192.168.1.1"
```

### 4.4 Port Number Conversions

Ports are 16-bit values (0-65535).

```c
#include <arpa/inet.h>

// Port 80 (HTTP) example:
uint16_t port = 80;

// Convert to network byte order for transmission
uint16_t network_port = htons(port);  // Result: 0x5000 on little-endian

// Receive from network and convert back
uint16_t received = 0x5000;
uint16_t decoded = ntohs(received);   // Result: 80
```

### 4.5 MAC Address Conversions

MAC addresses are 6 bytes, usually written as "00:1A:2B:3C:4D:5E"

```c
void parse_mac_string(const char *mac_str, unsigned char *mac_bytes) {
    // Parse six hex octets separated by colons
    sscanf(mac_str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &mac_bytes[0], &mac_bytes[1], &mac_bytes[2],
           &mac_bytes[3], &mac_bytes[4], &mac_bytes[5]);
}

void format_mac_address(unsigned char *mac_bytes, char *buffer) {
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_bytes[0], mac_bytes[1], mac_bytes[2],
            mac_bytes[3], mac_bytes[4], mac_bytes[5]);
}
```

### Exercise 4.1: Parse and Format IP

```c
#include <stdio.h>

// Parse "10.0.0.50" and format it back

uint32_t ip = parse_ip_string("10.0.0.50");
// ip = htonl((10 << 24) | (0 << 16) | (0 << 8) | 50)
// ip = htonl(0x0A000032)
// ip = 0x0A000032

char buffer[16];
format_ip_address(ip, buffer, sizeof(buffer));
// buffer = "10.0.0.50"
```

### 4.6 Key Takeaways

- IP addresses: 4 decimal octets → 32-bit value
- Ports: decimal → 16-bit value
- MAC addresses: 6 hex octets → 6 bytes
- Always use htonl/ntohl for multi-byte network data
- Always validate parsed values

---

## Module 5: Bit-Level Conversions (2-3 hours)

### 5.1 Extracting Multi-Bit Fields

```c
// Extract a field of N bits starting at position S
uint32_t extract_field(uint32_t value, int start_bit, int num_bits) {
    // Create a mask with N ones: (1 << N) - 1
    uint32_t mask = (1U << num_bits) - 1;

    // Shift right to position, then mask
    return (value >> start_bit) & mask;
}

// Example: Extract 3 bits starting at position 5
uint32_t value = 0b11010110;
uint32_t field = extract_field(value, 5, 3);  // Returns 0b101 = 5
```

**Visual:**
```
Original:     1 1 0 1 0 1 1 0
Positions:    7 6 5 4 3 2 1 0

Extract positions [7:5] (3 bits starting at 5):
Step 1: Shift right 5 positions
        Result: 0 0 0 0 1 1 0 1
Step 2: Mask with 3 ones (0b111)
        Result: 0 0 0 0 0 1 0 1 = 5
```

### 5.2 Setting Multi-Bit Fields

```c
// Set N bits starting at position S to a new value
void set_field(uint32_t *value, int start_bit, int num_bits,
               uint32_t field_value) {
    // Create mask with N ones
    uint32_t mask = (1U << num_bits) - 1;

    // Clear the bits we're about to set
    *value &= ~(mask << start_bit);

    // Set new bits (mask the input in case it's too large)
    *value |= (field_value & mask) << start_bit;
}

// Example: Set bits [5:3] to value 2
uint32_t data = 0b11010110;
set_field(&data, 3, 3, 2);  // Bits [5:3] become 010, rest unchanged
// Result: 0b11010010
```

**Visual:**
```
Original data: 1 1 0 1 0 1 1 0
Positions:     7 6 5 4 3 2 1 0

Set positions [5:3] to 2 (binary 010):
Step 1: Create mask (3 ones): 0 0 0 0 1 1 1 = 0b111
Step 2: Shift mask left 3:     0 0 1 1 1 0 0 = 0b11100
Step 3: Invert mask:           1 1 0 0 0 1 1 = 0b100011
Step 4: AND with data:         1 1 0 0 0 1 1 0 & 0x100011 = 0b11000110
Step 5: Prepare new bits:      2 (0b010) shifted left 3 = 0b010000
Step 6: OR together:           0b11000110 | 0b01000 = 0b11010010
```

### 5.3 TCP Flags Example

TCP flags are individual bits in a single byte:

```
Bit 0: FIN (connection finished)
Bit 1: SYN (synchronize sequence numbers)
Bit 2: RST (reset connection)
Bit 3: PSH (push data immediately)
Bit 4: ACK (acknowledgment field valid)
Bit 5: URG (urgent pointer valid)
Bit 6-7: Reserved
```

```c
struct TCPFlags {
    unsigned int fin: 1;
    unsigned int syn: 1;
    unsigned int rst: 1;
    unsigned int psh: 1;
    unsigned int ack: 1;
    unsigned int urg: 1;
    unsigned int reserved: 2;
};

// Unpack from byte
void unpack_flags(unsigned char flags_byte, struct TCPFlags *flags) {
    flags->fin = (flags_byte >> 0) & 1;
    flags->syn = (flags_byte >> 1) & 1;
    flags->rst = (flags_byte >> 2) & 1;
    flags->psh = (flags_byte >> 3) & 1;
    flags->ack = (flags_byte >> 4) & 1;
    flags->urg = (flags_byte >> 5) & 1;
    flags->reserved = (flags_byte >> 6) & 0x3;
}

// Pack to byte
unsigned char pack_flags(struct TCPFlags *flags) {
    unsigned char byte = 0;
    byte |= (flags->fin & 1) << 0;
    byte |= (flags->syn & 1) << 1;
    byte |= (flags->rst & 1) << 2;
    byte |= (flags->psh & 1) << 3;
    byte |= (flags->ack & 1) << 4;
    byte |= (flags->urg & 1) << 5;
    byte |= (flags->reserved & 0x3) << 6;
    return byte;
}
```

### 5.4 Binary-Coded Decimal (BCD)

BCD encodes each decimal digit as 4 bits:

```
Decimal 42 as BCD:
4 = 0x4 = 0100
2 = 0x2 = 0010
Packed BCD: 0x42
```

```c
unsigned char decimal_to_bcd(unsigned char decimal) {
    // Validate input
    if (decimal > 99) return 0xFF;  // Error

    unsigned char tens = decimal / 10;
    unsigned char ones = decimal % 10;

    // Pack: high nibble = tens, low nibble = ones
    return (tens << 4) | ones;
}

unsigned char bcd_to_decimal(unsigned char bcd) {
    unsigned char tens = (bcd >> 4) & 0xF;
    unsigned char ones = bcd & 0xF;

    // Validate BCD
    if (tens > 9 || ones > 9) return 0xFF;  // Error

    return tens * 10 + ones;
}

// Example:
unsigned char bcd = decimal_to_bcd(42);  // Result: 0x42
unsigned char dec = bcd_to_decimal(bcd); // Result: 42
```

### Exercise 5.1: Extract and Set Fields

```c
// Start with value 0b10110011
// Extract bits [5:2] (4 bits starting at position 2)
// Then set them to 0b1001

uint8_t value = 0b10110011;

// Extract
uint8_t field = (value >> 2) & 0xF;  // Result: 0b1100

// Set
uint8_t new_field = 0b1001;
value &= ~(0xF << 2);           // Clear bits [5:2]
value |= (new_field << 2);       // Set new bits
// Result: 0b10100111
```

### 5.6 Key Takeaways

- Shift + mask = extract
- Clear + set = modify fields
- Packing multiple values into one byte saves space
- Understanding bit positions is crucial
- Validation is essential (BCD only 0-9 per nibble)

---

## Module 6: Integration and Project Overview (1-2 hours)

### 6.1 Bringing It All Together

The Binary Data Converter combines all five previous modules:

1. **Module 1 Knowledge:** Understanding different number bases
2. **Module 2 Skills:** Using bitwise operations for conversions
3. **Module 3 Practice:** Handling endianness correctly
4. **Module 4 Application:** Transforming network data
5. **Module 5 Technique:** Packing and unpacking bits

### 6.2 Program Architecture

```c
// Main conversion functions (all modules combined)

// Base conversions (Module 2)
void format_binary(uint32_t value, char *buffer);
void format_hex(uint32_t value, char *buffer);
void format_octal(uint32_t value, char *buffer);

uint32_t parse_binary(const char *str);
uint32_t parse_hex(const char *str);
uint32_t parse_octal(const char *str);

// Endianness (Module 3)
int detect_endianness(void);
uint32_t swap_bytes_32(uint32_t value);
uint16_t swap_bytes_16(uint16_t value);

// Network data (Module 4)
uint32_t parse_ip_string(const char *ip);
void format_ip_address(uint32_t ip, char *buffer);
uint16_t parse_port_string(const char *port);

// Bit operations (Module 5)
uint32_t extract_field(uint32_t value, int start, int width);
void set_field(uint32_t *value, int start, int width, uint32_t field);
```

### 6.3 Usage Examples

```bash
# Convert decimal 255 to all bases
./converter 255 all
Output:
  Decimal: 255
  Binary: 11111111
  Hex: 0xFF
  Octal: 0377

# Show endianness
./converter detect-endian
Output: System is little-endian

# Convert IP address
./converter 192.168.1.1 ip-binary
Output:
  String: 192.168.1.1
  Binary: 11000000 10101000 00000001 00000001
  Hex: C0A80101

# Convert with byte swapping
./converter 0x12345678 swap
Output:
  Original: 0x12345678
  Swapped: 0x78563412
```

### 6.4 Testing Strategy

**Test each category:**

1. **Basic conversions:**
   - Edge cases: 0, 1, 255, max values
   - Powers of 2: 1, 2, 4, 8, 16, 32, 64, 128, 256...
   - Mixed values: 42, 73, 137...

2. **Endianness:**
   - Patterns: 0x00000000, 0xFFFFFFFF, 0xAAAAAAAA, 0x55555555
   - Real IPs: 127.0.0.1, 192.168.1.1, 10.0.0.1
   - Real ports: 0, 80, 443, 65535

3. **Validation:**
   - Invalid input: "abc" for decimal, "2" for binary, "G" for hex
   - Overflow: values too large for type
   - Truncation: handling values larger than output format

4. **Round-trip:**
   - Convert A → B → A should equal original
   - Test: binary → decimal → binary
   - Test: hex → decimal → hex

### Exercise 6.1: Complete Integration

```c
// Implement a simple version of the main converter

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: converter <value> <output_format>\n");
        printf("Format: binary, hex, octal, all\n");
        return 1;
    }

    const char *input = argv[1];
    const char *format = argv[2];

    // Parse input (assume decimal)
    uint32_t value = strtoul(input, NULL, 10);

    char buffer[33];

    if (strcmp(format, "binary") == 0 || strcmp(format, "all") == 0) {
        format_binary(value, buffer);
        printf("Binary: %s\n", buffer);
    }

    if (strcmp(format, "hex") == 0 || strcmp(format, "all") == 0) {
        format_hex(value, buffer);
        printf("Hex: 0x%s\n", buffer);
    }

    if (strcmp(format, "octal") == 0 || strcmp(format, "all") == 0) {
        format_octal(value, buffer);
        printf("Octal: 0%s\n", buffer);
    }

    return 0;
}
```

### 6.5 Key Takeaways

- All five modules build on each other
- Test extensively, especially edge cases
- Round-trip testing (A→B→A) validates correctness
- Real-world applications use network byte order
- Bit manipulation requires careful thinking and testing

---

## Summary: What You've Learned

### Module 1: Number Foundations
- Different bases represent the same value
- Position = power of base
- Powers of 2 are fundamental

### Module 2: Bitwise Conversions
- Shift + mask = extract
- Loop + shift + mask = convert
- Binary string ↔ decimal ↔ hex/octal

### Module 3: Endianness
- Big-endian = network order (MSB first)
- Little-endian = host order (LSB first)
- htonl/ntohl for conversion

### Module 4: Network Data
- IP: "192.168.1.1" ↔ 0xC0A80101
- Ports: decimal ↔ 16-bit network order
- Validation essential for parsing

### Module 5: Bit-Level Work
- Field extraction and setting
- TCP flags as bit fields
- BCD and other packed formats

### Module 6: Integration
- Combine all techniques
- Test comprehensively
- Real applications use all these skills

---

## Next Steps

After mastering these concepts:

1. **Complete the starter code** using these techniques
2. **Test thoroughly** with edge cases
3. **Compare with reference solution**
4. **Try the extensions** in PROJECT_GUIDE.md
5. **Move to Phase 2** with this foundation

You now understand the fundamental techniques used in:
- Network protocol implementations
- Firmware and embedded systems
- Data serialization and compression
- Cross-platform networking code
- Security tools and cryptography

Good luck! 🚀
