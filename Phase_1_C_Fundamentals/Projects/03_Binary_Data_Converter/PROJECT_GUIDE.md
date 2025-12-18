# Project 3: Binary Data Converter
## Complete Problem Statement and Implementation Guide

**Objective:** Build a converter program that transforms data between different number bases (decimal, binary, hexadecimal, octal) and handles network data format conversions. Learn to work with binary data at the bit level, understand byte ordering, and practice portable network programming.

**What You'll Learn:**
- Convert between number bases programmatically
- Handle endianness in network data
- Parse and format network addresses (IP, ports, MAC)
- Extract and pack multi-bit fields
- Validate conversions for correctness
- Write portable, network-aware C code

---

## The Challenge

### Problem Statement

Write a converter program that accepts values in multiple formats and produces output in any requested format.

**Input:** A value and desired output format(s)
```bash
./converter 255 all
./converter 192.168.1.1 ip-binary
./converter 0xDEADBEEF endian
./converter detect-endian
```

**Output:** Converted data in human-readable format

```
=== Binary Data Converter ===
Input: 255 (decimal)

Conversions:
  Decimal:      255
  Binary:       11111111
  Hexadecimal:  0xFF
  Octal:        0377
```

### Conversion Categories

**1. Base Conversions (Category A)**

Convert unsigned integers between four bases:
- Decimal (base 10): natural human representation
- Binary (base 2): computer representation
- Hexadecimal (base 16): compact binary notation
- Octal (base 8): legacy system representation

**Input:** Any base, decimal preferred
**Output:** All four bases

```bash
$ ./converter 42 all
Input: 42 (decimal)

Decimal:  42
Binary:   00101010
Hex:      0x2A
Octal:    052

# Each base has different use cases:
# - Decimal: human communication
# - Binary: understanding bit manipulation
# - Hex: network protocols (more compact than binary)
# - Octal: legacy Unix permissions (though less common now)
```

**2. Endianness Operations (Category B)**

Show how values differ across byte orders.

```bash
$ ./converter 0x12345678 swap
Original value:    0x12345678
Big-endian bytes:  12 34 56 78
Little-endian:     78 56 34 12
System uses:       little-endian

$ ./converter detect-endian
System endianness: little-endian
Test pattern: 0x12345678
Bytes in memory: 78 56 34 12
```

**3. Network Data Transformations (Category C)**

Convert between text and binary representations of network data.

**IP Addresses:**
```bash
$ ./converter 192.168.1.1 ip-binary
Address:       192.168.1.1
Binary (32b):  11000000 10101000 00000001 00000001
Hexadecimal:   0xC0A80101
Network order: big-endian (same on wire)

Parse components:
  192 = 0xC0 = 11000000
  168 = 0xA8 = 10101000
    1 = 0x01 = 00000001
    1 = 0x01 = 00000001
```

**Ports:**
```bash
$ ./converter 80 port
Decimal:      80
Hexadecimal:  0x50
Port 80 is HTTP (well-known port)
Network byte order: 0x5000 (on little-endian system)
```

**4. System Information (Category D)**

Display information about the system.

```bash
$ ./converter detect-endian
System Information:
  Endianness: little-endian (Intel/ARM architecture)

Test explanation:
  The test value 0x12345678 is stored as bytes: 78 56 34 12
  This indicates little-endian (least significant byte first)

  On big-endian system, bytes would be: 12 34 56 78
```

---

## File Structure

```
03_Binary_Data_Converter/
├── README.md                       (Quick start)
├── LESSON_PLAN.md                 (6 modules with exercises)
├── GETTING_STARTED.md             (Learning paths)
├── PROJECT_GUIDE.md               (This file)
│
├── 03_starter.c                   (Your template - edit this!)
├── 03_c_solution.c                (Reference solution)
├── 03_c_solution_explained.md      (Detailed explanation)
│
├── generate_test_data.c           (Test data generator)
├── Makefile                       (Build automation)
└── sample_conversions.txt         (Expected outputs reference)
```

---

## Step-by-Step Implementation Guide

### Phase 1: Foundation Functions (Solo Bit Work)

**Goal:** Implement basic number base conversions.

#### Step 1.1: Decimal to Binary String

Convert a decimal value to a binary string representation.

**Function signature:**
```c
void format_binary(uint32_t value, char *buffer, size_t buffer_size) {
    // TODO: Convert value to binary string in buffer
    // Example: format_binary(42, buffer, 33) → "00101010"
    // Hint: Use bit extraction in a loop from MSB to LSB
}
```

**Algorithm:**
1. Iterate through bit positions 7 to 0 (for 8-bit)
2. Extract each bit: `(value >> i) & 1`
3. Convert bit to character: `bit ? '1' : '0'`
4. Store in buffer
5. Add null terminator

**Validation:**
- Input: 0 → Output: "00000000"
- Input: 255 → Output: "11111111"
- Input: 42 → Output: "00101010"

---

#### Step 1.2: Decimal to Hexadecimal String

Convert a decimal value to a hexadecimal string representation.

**Function signature:**
```c
void format_hex(uint32_t value, char *buffer, size_t buffer_size) {
    // TODO: Convert value to hex string in buffer
    // Example: format_hex(255, buffer, 9) → "000000ff"
    // Hint: Extract 4 bits at a time, map to 0-9,a-f
}
```

**Algorithm:**
1. Create lookup table: "0123456789abcdef"
2. Iterate through 4-bit chunks (positions 28, 24, 20, ..., 0)
3. Extract 4 bits: `(value >> position) & 0xF`
4. Map to hex digit using lookup table
5. Store in buffer

**Validation:**
- Input: 0 → Output: "00000000"
- Input: 255 → Output: "000000ff"
- Input: 0xDEADBEEF → Output: "deadbeef"

---

#### Step 1.3: Decimal to Octal String

Convert a decimal value to an octal string representation.

**Function signature:**
```c
void format_octal(uint32_t value, char *buffer, size_t buffer_size) {
    // TODO: Convert value to octal string in buffer
    // Example: format_octal(255, buffer, 12) → "000000377"
    // Hint: Extract 3 bits at a time, map to 0-7
}
```

**Validation:**
- Input: 0 → Output: "000000000"
- Input: 255 → Output: "000000377"
- Input: 64 → Output: "000000100"

---

#### Step 1.4: Parse Binary String

Convert a binary string "11111111" back to decimal value.

**Function signature:**
```c
uint32_t parse_binary(const char *str) {
    // TODO: Parse binary string to uint32_t
    // Example: parse_binary("00101010") → 42
    // Hint: For each character, shift left and add bit
    // Error: Return 0 on invalid input
}
```

**Algorithm:**
1. Validate input (only '0' and '1')
2. For each character:
   - Shift result left: `result << 1`
   - Add new bit: `| (char - '0')`
3. Return result

**Validation:**
- Input: "00000000" → Output: 0
- Input: "11111111" → Output: 255
- Input: "00101010" → Output: 42
- Input: "invalid" → Output: 0 (error)

---

#### Step 1.5: Parse Hexadecimal String

Convert a hex string "FF" back to decimal value.

**Function signature:**
```c
uint32_t parse_hex(const char *str) {
    // TODO: Parse hex string to uint32_t
    // Example: parse_hex("0xFF") → 255
    // Example: parse_hex("FF") → 255
    // Hint: Use strtoul with base 16, or manual digit parsing
}
```

**Considerations:**
- Handle both "FF" and "0xFF" formats
- Case insensitive: "ff" and "FF" both valid
- Error checking for invalid characters
- Can use strtoul(str, NULL, 16) for simplicity

**Validation:**
- Input: "0xFF" → Output: 255
- Input: "FF" → Output: 255
- Input: "ff" → Output: 255
- Input: "0x00" → Output: 0
- Input: "deadbeef" → Output: 0xDEADBEEF

---

#### Step 1.6: Parse Octal String

Convert an octal string "377" back to decimal value.

**Function signature:**
```c
uint32_t parse_octal(const char *str) {
    // TODO: Parse octal string to uint32_t
    // Example: parse_octal("0377") → 255
    // Example: parse_octal("377") → 255
    // Hint: Use strtoul with base 8, or manual digit parsing
}
```

**Validation:**
- Input: "0377" → Output: 255
- Input: "377" → Output: 255
- Input: "0" → Output: 0
- Input: "777" → Output: 511

---

### Phase 2: Endianness Utilities (Byte-Order Work)

**Goal:** Implement byte-order detection and conversion.

#### Step 2.1: Detect System Endianness

Determine if the system is big-endian or little-endian.

**Function signature:**
```c
int detect_endianness(void) {
    // TODO: Return 1 if big-endian, 0 if little-endian
    // Hint: Create test value 0x12345678, check first byte
}
```

**Algorithm:**
1. Create test value: `uint32_t test = 0x12345678`
2. Cast to unsigned char pointer: `(unsigned char *)&test`
3. Check first byte:
   - If `bytes[0] == 0x12`: big-endian, return 1
   - If `bytes[0] == 0x78`: little-endian, return 0

**Validation:**
- On Intel/ARM: Returns 0 (little-endian)
- On PowerPC/old Motorola: Returns 1 (big-endian)

---

#### Step 2.2: Swap Bytes (32-bit)

Manually reverse the byte order of a 32-bit value.

**Function signature:**
```c
uint32_t swap_bytes_32(uint32_t value) {
    // TODO: Reverse byte order manually
    // Example: 0x12345678 → 0x78563412
    // Hint: Extract each byte, shift to new position
}
```

**Algorithm:**
```c
uint32_t result = 0;
result |= (value & 0x000000FF) << 24;  // Byte 0 → Byte 3
result |= (value & 0x0000FF00) << 8;   // Byte 1 → Byte 2
result |= (value & 0x00FF0000) >> 8;   // Byte 2 → Byte 1
result |= (value & 0xFF000000) >> 24;  // Byte 3 → Byte 0
return result;
```

**Validation:**
- Input: 0x00000000 → Output: 0x00000000
- Input: 0x12345678 → Output: 0x78563412
- Input: 0xDEADBEEF → Output: 0xEFBEADDE
- Input: 0xFFFFFFFF → Output: 0xFFFFFFFF

---

#### Step 2.3: Swap Bytes (16-bit)

Manually reverse the byte order of a 16-bit value.

**Function signature:**
```c
uint16_t swap_bytes_16(uint16_t value) {
    // TODO: Reverse byte order for 16-bit value
    // Example: 0x1234 → 0x3412
}
```

**Algorithm:**
```c
return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
```

**Validation:**
- Input: 0x0000 → Output: 0x0000
- Input: 0x1234 → Output: 0x3412
- Input: 0xFFFF → Output: 0xFFFF

---

### Phase 3: Network Data Transformations (Protocol Work)

**Goal:** Implement IP address and port conversions.

#### Step 3.1: Parse IP Address String

Convert "192.168.1.1" to 32-bit binary value (network byte order).

**Function signature:**
```c
uint32_t parse_ip_string(const char *ip_str) {
    // TODO: Parse IP address string to uint32_t
    // Example: "192.168.1.1" → 0xC0A80101
    // Hint: Use sscanf to parse four octets
    // Return 0 on error (INADDR_NONE)
}
```

**Algorithm:**
```c
unsigned char a, b, c, d;
if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d) != 4) {
    return 0;  // Invalid format
}

// Combine into 32-bit value in host order
uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;

// Convert to network byte order
return htonl(ip);
```

**Validation:**
- Input: "127.0.0.1" → Output: 0x7F000001 (loopback)
- Input: "192.168.1.1" → Output: 0xC0A80101
- Input: "10.0.0.1" → Output: 0x0A000001
- Input: "invalid" → Output: 0
- Input: "256.1.1.1" → Output: 0 (out of range)

---

#### Step 3.2: Format IP Address String

Convert 32-bit binary value back to "192.168.1.1" format.

**Function signature:**
```c
void format_ip_address(uint32_t ip_binary, char *buffer, size_t buffer_size) {
    // TODO: Format IP address from binary
    // Example: 0xC0A80101 (network) → "192.168.1.1"
    // Hint: Convert from network byte order, extract octets
}
```

**Algorithm:**
```c
// Convert from network byte order
uint32_t host_ip = ntohl(ip_binary);

// Extract octets
unsigned char a = (host_ip >> 24) & 0xFF;
unsigned char b = (host_ip >> 16) & 0xFF;
unsigned char c = (host_ip >> 8) & 0xFF;
unsigned char d = host_ip & 0xFF;

// Format as string
snprintf(buffer, buffer_size, "%u.%u.%u.%u", a, b, c, d);
```

**Validation:**
- Input: 0x7F000001 → Output: "127.0.0.1"
- Input: 0xC0A80101 → Output: "192.168.1.1"
- Input: 0x0A000001 → Output: "10.0.0.1"

---

#### Step 3.3: Port Number Information

Provide information about well-known ports.

**Function signature:**
```c
const char* get_port_name(uint16_t port) {
    // TODO: Return service name for common ports
    // Examples:
    // 80 → "HTTP"
    // 443 → "HTTPS"
    // 22 → "SSH"
    // 21 → "FTP"
    // Hint: Use switch statement or lookup table
}
```

**Common Ports:**
```
21   → FTP
22   → SSH
25   → SMTP
53   → DNS
80   → HTTP
110  → POP3
143  → IMAP
443  → HTTPS
3306 → MySQL
5432 → PostgreSQL
8080 → HTTP-alt
```

---

### Phase 4: Bit-Level Utilities (Advanced Work)

**Goal:** Implement field extraction and manipulation.

#### Step 4.1: Extract Bit Field

Extract N bits starting at position S.

**Function signature:**
```c
uint32_t extract_field(uint32_t value, int start_bit, int num_bits) {
    // TODO: Extract N bits starting at position start_bit
    // Example: extract_field(0b11010110, 3, 3) → 0b101
}
```

**Algorithm:**
```c
uint32_t mask = (1U << num_bits) - 1;
return (value >> start_bit) & mask;
```

**Validation:**
- extract_field(0b11010110, 0, 1) → 0
- extract_field(0b11010110, 1, 1) → 1
- extract_field(0b11010110, 3, 3) → 5 (0b101)
- extract_field(0xFF, 0, 4) → 15

---

#### Step 4.2: Set Bit Field

Set N bits starting at position S to a new value.

**Function signature:**
```c
void set_field(uint32_t *value, int start_bit, int num_bits,
               uint32_t field_value) {
    // TODO: Set N bits starting at start_bit to field_value
    // Example: set_field(&data, 3, 3, 5) sets bits [5:3] to 0b101
}
```

**Algorithm:**
```c
uint32_t mask = (1U << num_bits) - 1;
*value = (*value & ~(mask << start_bit)) |
         ((field_value & mask) << start_bit);
```

**Validation:**
- Start with 0b11010110
- set_field(&value, 3, 3, 2) → 0b11010010

---

### Phase 5: Main Program and User Interface

**Goal:** Tie everything together into a working converter program.

#### Step 5.1: Argument Parsing

Parse command-line arguments to determine conversion type.

```c
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    // argv[1] = input value
    // argv[2] = conversion type (or NULL for default)
    // ...

    // TODO: Dispatch to appropriate conversion function
}
```

---

#### Step 5.2: Input Parsing

Automatically detect input format (decimal, binary, hex, octal).

```c
uint32_t parse_auto(const char *str) {
    // Detect format:
    // "0x..." → hexadecimal
    // "0b..." or "0B..." → binary
    // "0..." → octal
    // Otherwise → decimal

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        return parse_hex(str);
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        return parse_binary(str + 2);
    } else if (str[0] == '0' && str[1] >= '0' && str[1] <= '7') {
        return parse_octal(str);
    } else {
        return strtoul(str, NULL, 10);
    }
}
```

---

#### Step 5.3: Output Formatting

Display results based on requested format.

```bash
# Show all conversions
./converter 255 all
Decimal:      255
Binary:       11111111
Hexadecimal:  0xFF
Octal:        0377

# Show specific formats
./converter 255 binary hex
Binary: 11111111
Hex: 0xFF

# Show IP conversion
./converter 192.168.1.1 ip
Address:    192.168.1.1
Binary:     11000000101010000000000100000001
Hex:        0xC0A80101
```

---

## Testing Strategy

### Test Categories

**1. Base Conversions**
```bash
# Zero and max values
./converter 0 all
./converter 255 all
./converter 4294967295 all

# Powers of 2
./converter 1 all
./converter 2 all
./converter 256 all

# Common values
./converter 42 all
./converter 73 all
./converter 127 all
```

**2. Endianness**
```bash
./converter detect-endian
./converter 0x12345678 swap
./converter 0xDEADBEEF swap
```

**3. Network Data**
```bash
./converter 127.0.0.1 ip
./converter 192.168.1.1 ip
./converter 10.0.0.1 ip
./converter 80 port
./converter 443 port
```

**4. Edge Cases**
```bash
./converter 0 all
./converter 1 all
./converter -1 all  # If signed support added
./converter invalid-input all
```

---

## Common Challenges and Solutions

### Challenge 1: Off-by-One in Bit Extraction

**Problem:** Extracting bits gives wrong result
```c
// WRONG - gets wrong bit
int bit = (value & (1 << position));  // Returns 0 or non-zero

// CORRECT - shifts then masks
int bit = (value >> position) & 1;    // Returns 0 or 1
```

**Solution:** Always shift first, then mask with 1 to get clean 0/1.

---

### Challenge 2: Binary String Conversion

**Problem:** String has wrong bits or wrong order
```c
// WRONG - processes LSB first
for (int i = 0; i < 8; i++) {
    buffer[i] = ((value >> i) & 1) ? '1' : '0';
}

// CORRECT - processes MSB first
for (int i = 7; i >= 0; i--) {
    buffer[7 - i] = ((value >> i) & 1) ? '1' : '0';
}
```

**Solution:** Always start from MSB (bit 7 for 8-bit value).

---

### Challenge 3: Endianness Confusion

**Problem:** Swapped bytes in wrong direction
```c
// WRONG - reverses incorrectly
result = (value << 24) | (value >> 24);

// CORRECT - carefully extract and place each byte
result = ((value & 0x000000FF) << 24) |
         ((value & 0x0000FF00) << 8)  |
         ((value & 0x00FF0000) >> 8)  |
         ((value & 0xFF000000) >> 24);
```

**Solution:** Extract each byte individually, then shift to correct position.

---

### Challenge 4: IP Address Parsing

**Problem:** Parsed IP address is wrong value
```c
// Confusion about byte order - use htonl!
uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;  // Host byte order
uint32_t network_ip = htonl(ip);  // Convert to network byte order
```

**Solution:** Always convert with htonl/ntohl for network data.

---

## Extensions (After Basic Implementation)

### Extension 1: Support Signed Integers

Add support for signed decimal values.

```bash
./converter -42 all
./converter -1 all
```

Considerations:
- Negative values in binary (two's complement)
- Careful handling of sign bit

---

### Extension 2: Bitfield Display

Show individual bit flags.

```bash
./converter 0x83 bitfield
Binary: 10000011
Bits:   7  6  5  4  3  2  1  0
        1  0  0  0  0  0  1  1
```

---

### Extension 3: MAC Address Support

Parse and format MAC addresses.

```bash
./converter 00:1A:2B:3C:4D:5E mac
Formatted: 00:1A:2B:3C:4D:5E
Binary: ...
```

---

### Extension 4: Custom Bit Field Extraction

Allow users to extract specific bit ranges.

```bash
./converter 0xFF extract-bits 2 4
Extract 4 bits starting at bit 2 from 0xFF
Result: 0xF (binary: 1111)
```

---

### Extension 5: Endianness Conversion Utilities

Create functions to convert struct data.

```c
struct NetworkData {
    uint16_t port;
    uint32_t ip;
    uint16_t flags;
};

void convert_struct_to_network(struct NetworkData *data) {
    data->port = htons(data->port);
    data->ip = htonl(data->ip);
    data->flags = htons(data->flags);
}
```

---

## Success Criteria

Your solution is correct when:

- ✓ All base conversions (decimal ↔ binary, hex, octal) work correctly
- ✓ Round-trip conversions return original value (A→B→A = A)
- ✓ Endianness detection works on your system
- ✓ IP address parsing and formatting work correctly
- ✓ Port number handling works
- ✓ Edge cases handled (0, max values, invalid input)
- ✓ Output format matches specification exactly
- ✓ Program compiles without warnings
- ✓ Program runs without crashing on bad input
- ✓ All conversions are validated for correctness

---

## Learning Outcomes

After completing this project, you'll be able to:

- **Explain** why different number bases exist and when to use them
- **Convert** between number bases manually and programmatically
- **Understand** endianness and how it affects network programming
- **Detect** system endianness and convert byte orders
- **Parse** network addresses (IP, ports, MAC) safely
- **Extract** and set individual bits and bit fields
- **Write** portable code that works across different systems
- **Validate** conversions and detect errors
- **Test** data transformations comprehensively

---

## Time Estimate

- Phase 1 (Foundation): 2-3 hours
- Phase 2 (Endianness): 1-2 hours
- Phase 3 (Network Data): 1-2 hours
- Phase 4 (Bit Work): 1-2 hours
- Phase 5 (Main Program): 1-2 hours
- Testing & Debugging: 1-2 hours
- **Total: 7-13 hours**

---

## Reference Files

- **LESSON_PLAN.md:** Detailed explanations of all concepts
- **03_c_solution.c:** Complete reference implementation
- **03_c_solution_explained.md:** Line-by-line walkthrough
- **generate_test_data.c:** Generates test cases

Start with Phase 1, build up progressively, test thoroughly!

Good luck! 🚀
