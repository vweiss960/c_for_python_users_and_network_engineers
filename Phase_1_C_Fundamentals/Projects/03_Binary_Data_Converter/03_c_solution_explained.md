# Binary Data Converter - Solution Explained

This document explains the reference solution line-by-line, concept-by-concept. Read this **after** attempting your own solution to understand the correct approach.

---

## Part 1: Base Conversion Functions

### format_binary() - Convert to Binary String

**What it does:** Converts a 32-bit unsigned integer to a 32-character binary string.

```c
ConversionResult format_binary(uint32_t value, char *buffer, size_t buffer_size) {
    if (buffer_size < BINARY_STR_MAX) {
        return RESULT_FORMAT_ERROR;
    }
```

**Line 1-3:** Validate that the output buffer is large enough. We need at least 33 bytes (32 for bits + 1 for null terminator).

```c
    for (int i = 31; i >= 0; i--) {
```

**Key insight:** Start from bit 31 (the most significant bit, MSB) and work down to bit 0 (LSB). This produces a string that reads left-to-right from MSB to LSB, which is the normal way humans write binary numbers.

```c
        int bit = (value >> i) & 1;
```

**Bit extraction:**
- `(value >> i)` shifts the desired bit to position 0
- `& 1` masks all other bits away, leaving just the bit value (0 or 1)

For example, extracting bit 5 from 0b11010110:
```
Original:         11010110
Shift right 5:    00000110
AND with 0x01:    00000010 = 1 (bit is set)
```

```c
        buffer[31 - i] = bit ? '1' : '0';
```

**String indexing:**
- When `i = 31`, we store at `buffer[0]` (leftmost character, MSB)
- When `i = 0`, we store at `buffer[31]` (rightmost character, LSB)

So for value 42 (0b00101010):
```
i=31: buffer[0] = '0'  (bit 31 is 0)
i=30: buffer[1] = '0'  (bit 30 is 0)
...
i=5:  buffer[26] = '1' (bit 5 is 1)
...
i=0:  buffer[31] = '0' (bit 0 is 0)

Result: "00000000000000000000000000101010"
```

```c
    buffer[32] = '\0';  // Null terminate
```

**Critical:** Always null-terminate C strings. The buffer needs 33 bytes total: 32 for bits + 1 for null terminator.

---

### format_hex() - Convert to Hexadecimal String

**What it does:** Converts a 32-bit value to an 8-character hex string.

```c
    const char *hex_digits = "0123456789abcdef";
```

**Lookup table:** Instead of using `if` statements to map 0-15 to 0-9,a-f, we use an array index. This is much cleaner:
- `hex_digits[0]` = '0'
- `hex_digits[9]` = '9'
- `hex_digits[10]` = 'a'
- `hex_digits[15]` = 'f'

```c
    int index = 0;

    for (int i = 28; i >= 0; i -= 4) {
```

**Processing 4 bits at a time:** Each hex digit represents 4 binary bits (one nibble).
- Position 28 captures bits [31:28]
- Position 24 captures bits [27:24]
- ...
- Position 0 captures bits [3:0]

For value 255 (0x000000FF):
```
Binary: 00000000 00000000 00000000 11111111

Extract 4-bit chunks from left to right:
i=28: (0x000000FF >> 28) & 0xF = 0x0 → '0'
i=24: (0x000000FF >> 24) & 0xF = 0x0 → '0'
i=20: (0x000000FF >> 20) & 0xF = 0x0 → '0'
i=16: (0x000000FF >> 16) & 0xF = 0x0 → '0'
i=12: (0x000000FF >> 12) & 0xF = 0x0 → '0'
i=8:  (0x000000FF >> 8) & 0xF = 0x0 → '0'
i=4:  (0x000000FF >> 4) & 0xF = 0xF → 'f'
i=0:  (0x000000FF >> 0) & 0xF = 0xF → 'f'

Result: "000000ff"
```

```c
        int digit = (value >> i) & 0xF;
```

**Mask explanation:** `0xF` is 0b1111 in binary, which is four 1's. This selects exactly 4 bits after the shift.

```c
        buffer[index++] = hex_digits[digit];
```

**String building:** As we extract each 4-bit group, we immediately append the corresponding hex digit to the output buffer.

---

### format_octal() - Convert to Octal String

**What it does:** Converts a 32-bit value to an octal string (base 8).

```c
    for (int i = 30; i >= 0; i -= 3) {
        int digit = (value >> i) & 0x7;
        buffer[index++] = '0' + digit;
    }
```

**Key differences from hex:**
- Process **3 bits** at a time (not 4), since octal is base 8 (2³ = 8)
- Extract with `& 0x7` (which is 0b111, three 1's)
- Convert digit directly with `'0' + digit` (since octal digits are always 0-7)

For value 255:
```
Binary: 00000000 00000000 00000000 11111111 (32 bits)

Extract 3-bit chunks from position 30 down to 0:
i=30: (255 >> 30) & 0x7 = 0 → '0'
i=27: (255 >> 27) & 0x7 = 0 → '0'
...continuing...
i=3:  (255 >> 3) & 0x7 = 7 → '7'
i=0:  (255 >> 0) & 0x7 = 7 → '7'

Result: "000000377"
```

---

### parse_binary() - Parse Binary String to Integer

**What it does:** Convert "00101010" back to the integer 42.

```c
    if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        str += 2;
    }
```

**Skip prefix:** Allow optional "0b" or "0B" prefix and skip it.

```c
    uint32_t result = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return 0;  // Invalid binary digit
        }

        result = (result << 1) | (str[i] - '0');
    }
```

**Accumulator pattern:**
- Start with `result = 0`
- For each character: `result = (result << 1) | bit_value`
- This shift-and-add builds up the number bit by bit

**Detailed example for "00101010" (should give 42):**
```
Start: result = 0 (binary: 0)

Process '0': result = (0 << 1) | 0 = 0 (binary: 0)
Process '0': result = (0 << 1) | 0 = 0 (binary: 0)
Process '1': result = (0 << 1) | 1 = 1 (binary: 1)
Process '0': result = (1 << 1) | 0 = 2 (binary: 10)
Process '1': result = (2 << 1) | 1 = 5 (binary: 101)
Process '0': result = (5 << 1) | 0 = 10 (binary: 1010)
Process '1': result = (10 << 1) | 1 = 21 (binary: 10101)
Process '0': result = (21 << 1) | 0 = 42 (binary: 101010)

Final: 42
```

**Why this works:** Each shift-and-add doubles the previous value and adds the new bit. This is exactly how positional number systems work:
- Each position represents a power of the base
- Adding bits left to right with shifts achieves this

---

### parse_hex() and parse_octal() - Simple Parsing

```c
uint32_t parse_hex(const char *str) {
    char *endptr;
    uint32_t value = (uint32_t)strtoul(str, &endptr, 16);

    if (*endptr != '\0') {
        return 0;  // Invalid characters
    }

    return value;
}
```

**Using strtoul():**
- `strtoul(str, &endptr, 16)` parses a string as an unsigned long in base 16
- `&endptr` receives a pointer to the first non-digit character
- If `*endptr != '\0'`, the string wasn't fully consumed (invalid input)

**Why strtoul is safe:** It handles edge cases like integer overflow and provides error detection.

---

### parse_auto() - Detect Format and Parse

```c
uint32_t parse_auto(const char *str) {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        return parse_hex(str);  // "0x..." → hex
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
        return parse_binary(str + 2);  // "0b..." → binary
    } else if (str[0] == '0' && isdigit(str[1]) && str[1] >= '0' && str[1] <= '7') {
        return parse_octal(str);  // "0..." → octal
    } else {
        return strtoul(str, NULL, 10);  // Otherwise → decimal
    }
}
```

**Decision tree:**
1. If starts with "0x" or "0X" → hexadecimal
2. If starts with "0b" or "0B" → binary
3. If starts with "0" and next digit is octal (0-7) → octal
4. Otherwise → decimal

**Edge case handling:** The octal check is careful:
- `isdigit(str[1])` ensures there is a second character that is a digit
- `str[1] >= '0' && str[1] <= '7'` ensures it's a valid octal digit (not 8 or 9)

---

## Part 2: Endianness Functions

### detect_endianness() - System Byte Order

```c
int detect_endianness(void) {
    uint32_t test = 0x12345678;
    unsigned char *bytes = (unsigned char *)&test;

    return bytes[0] == 0x12;
}
```

**Clever trick:**
- Store the value 0x12345678 in a 32-bit variable
- Cast the address to unsigned char pointer to access individual bytes
- Check the **first** byte in memory

**Why this works:**
- **Big-endian:** Most significant byte (0x12) stored first
  - Memory: [12][34][56][78] ← returns 1 (true)
- **Little-endian:** Least significant byte (0x78) stored first
  - Memory: [78][56][34][12] ← returns 0 (false)

This is a portable way to detect endianness at runtime.

---

### swap_bytes_32() - Reverse 32-bit Value

```c
uint32_t swap_bytes_32(uint32_t value) {
    uint32_t result = 0;
    result |= (value & 0x000000FF) << 24;  // Byte 0 → Byte 3
    result |= (value & 0x0000FF00) << 8;   // Byte 1 → Byte 2
    result |= (value & 0x00FF0000) >> 8;   // Byte 2 → Byte 1
    result |= (value & 0xFF000000) >> 24;  // Byte 3 → Byte 0
    return result;
}
```

**Manual byte reversal:**

```
Original:    12 34 56 78 (big-endian representation)
Positions:   3  2  1  0  (byte positions)

To reverse:
- Extract byte 0 (0x78):  value & 0x000000FF = 0x78
  Shift to position 3:    0x78 << 24 = 0x78000000

- Extract byte 1 (0x56):  value & 0x0000FF00 = 0x5600
  Shift to position 2:    0x5600 << 8 = 0x00560000

- Extract byte 2 (0x34):  value & 0x00FF0000 = 0x340000
  Shift to position 1:    0x340000 >> 8 = 0x00003400

- Extract byte 3 (0x12):  value & 0xFF000000 = 0x12000000
  Shift to position 0:    0x12000000 >> 24 = 0x00000012

Combine with OR:
  0x78000000 | 0x00560000 | 0x00003400 | 0x00000012 = 0x78563412
```

**Byte mask patterns:**
- `0x000000FF` = last byte (rightmost)
- `0x0000FF00` = second-to-last byte
- `0x00FF0000` = third byte
- `0xFF000000` = first byte (leftmost)

---

## Part 3: Network Data Functions

### parse_ip_string() - "192.168.1.1" → 0xC0A80101

```c
uint32_t parse_ip_string(const char *ip_str) {
    unsigned char a, b, c, d;

    if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d) != 4) {
        return 0;  // Invalid format
    }
```

**Parse IP octets:**
- `%hhu` = unsigned char (range 0-255)
- `sscanf` returns number of successfully parsed values
- Check for exactly 4 values parsed

```c
    uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;
    return htonl(ip);
```

**Combine octets:**
```
a=192:  192 << 24 = 0xC0000000
b=168:  168 << 16 = 0x00A80000
c=1:    1 << 8    = 0x00000100
d=1:    1         = 0x00000001

OR together:       0xC0A80101
```

**Convert to network byte order:** `htonl()` converts from host byte order to network byte order (big-endian). On big-endian systems, this is a no-op. On little-endian systems, this swaps bytes.

---

### format_ip_address() - 0xC0A80101 → "192.168.1.1"

```c
void format_ip_address(uint32_t ip_binary, char *buffer, size_t buffer_size) {
    uint32_t host_ip = ntohl(ip_binary);
```

**Convert from network byte order:** `ntohl()` reverses `htonl()`, converting network byte order back to host byte order.

```c
    unsigned char a = (host_ip >> 24) & 0xFF;
    unsigned char b = (host_ip >> 16) & 0xFF;
    unsigned char c = (host_ip >> 8) & 0xFF;
    unsigned char d = host_ip & 0xFF;
```

**Extract octets:**
- Shift right by 24, 16, 8, 0 positions respectively
- Mask with `0xFF` to get just the 8-bit octet

For 0xC0A80101:
```
Shift right 24: 0xC0A80101 >> 24 = 0x000000C0 = 192 decimal
Shift right 16: 0xC0A80101 >> 16 = 0x0000C0A8, & 0xFF = 0xA8 = 168
Shift right 8:  0xC0A80101 >> 8  = 0x00C0A801, & 0xFF = 0x01 = 1
Shift right 0:  0xC0A80101 >> 0  = 0xC0A80101, & 0xFF = 0x01 = 1
```

```c
    snprintf(buffer, buffer_size, "%u.%u.%u.%u", a, b, c, d);
```

**Format as string:** Use `snprintf()` (not `sprintf()`) to prevent buffer overflow.

---

### get_port_name() - Port Number Lookup

```c
const char* get_port_name(uint16_t port) {
    switch (port) {
        case 21:    return "FTP";
        case 22:    return "SSH";
        case 80:    return "HTTP";
        case 443:   return "HTTPS";
        default:    return "Unknown";
    }
}
```

**Simple lookup:** Switch statement with well-known port numbers. This could also be implemented with an array of structs for larger port tables.

---

## Part 4: Bit-Level Operations

### extract_field() - Get N Bits at Position

```c
uint32_t extract_field(uint32_t value, int start_bit, int num_bits) {
    uint32_t mask = (1U << num_bits) - 1;
    return (value >> start_bit) & mask;
}
```

**Mask creation:**
```
For num_bits=3:
  (1 << 3) = 0b1000 = 8
  8 - 1    = 0b0111 = 7 = 0x7 (three 1 bits)

For num_bits=4:
  (1 << 4) = 0b10000 = 16
  16 - 1   = 0b01111 = 15 = 0xF (four 1 bits)
```

**Extraction:**
```
Example: Extract 3 bits starting at position 3 from 0b11010110
  Mask:        0b00000111 (three 1's)
  Shift right: 0b11010110 >> 3 = 0b00010110
  AND with mask: 0b00010110 & 0b00000111 = 0b00000110 = 6
```

---

### set_field() - Set N Bits at Position

```c
void set_field(uint32_t *value, int start_bit, int num_bits,
               uint32_t field_value) {
    uint32_t mask = (1U << num_bits) - 1;

    *value &= ~(mask << start_bit);

    *value |= (field_value & mask) << start_bit;
}
```

**Three steps:**

1. **Create mask:** Same as `extract_field()`

2. **Clear target bits:**
   ```
   mask = 0b111 (3 bits)
   mask << 3 = 0b111000 (shift to target position)
   ~(mask << 3) = 0b...11000111 (inverted: 0's where we want to clear)
   *value &= 0b...11000111 (AND clears bits [5:3])
   ```

3. **Set new bits:**
   ```
   field_value & mask = mask the input to ensure it fits
   (...) << start_bit = shift to target position
   *value |= ... (OR sets the new bits)
   ```

**Complete example:**
```
Original: 0b11010110
Set bits [5:3] to 2 (0b010):

Step 1: Create mask
  mask = (1 << 3) - 1 = 0b111

Step 2: Clear
  mask << 3 = 0b111000
  ~0b111000 = 0b11000111 (in 8-bit view)
  0b11010110 & 0b11000111 = 0b11000110

Step 3: Set
  2 & 0b111 = 0b010
  0b010 << 3 = 0b010000
  0b11000110 | 0b010000 = 0b11010110

Result: 0b11010110 (correct!)
```

---

## Part 5: Output Functions

### display_all_conversions() - Show Results

This function ties together the conversion functions and displays results:

```c
void display_all_conversions(uint32_t value) {
    char binary[BINARY_STR_MAX];
    char hex[HEX_STR_MAX];
    char octal[OCTAL_STR_MAX];

    format_binary(value, binary, sizeof(binary));
    format_hex(value, hex, sizeof(hex));
    format_octal(value, octal, sizeof(octal));
```

**Call all format functions** and store results in buffers.

```c
    printf("=== Binary Data Converter ===\n");
    printf("Input value: %u (decimal)\n\n", value);
    printf("Conversions:\n");
    printf("  Decimal:      %u\n", value);
    printf("  Binary:       %s\n", binary);
    printf("  Hexadecimal:  0x%s\n", hex);
    printf("  Octal:        0%s\n", octal);
}
```

**Format output** in a readable way with labels and proper prefixes.

---

### display_ip_conversions() - Show IP in Multiple Formats

```c
void display_ip_conversions(const char *ip_str) {
    uint32_t ip = parse_ip_string(ip_str);
    if (ip == 0 && strcmp(ip_str, "0.0.0.0") != 0) {
        printf("Invalid IP address: %s\n", ip_str);
        return;
    }
```

**Parse and validate:** The special case for "0.0.0.0" is needed because parsing it returns 0, which would normally indicate an error.

```c
    char ip_back[IP_STR_MAX];
    format_ip_address(ip, ip_back, sizeof(ip_back));

    char binary[BINARY_STR_MAX];
    format_binary(ntohl(ip), binary, sizeof(binary));
```

**Convert back and forth:** Demonstrates round-trip conversion.

**Note:** We use `ntohl(ip)` to convert from network byte order to host order before passing to `format_binary()`, since `format_binary()` expects a host-order value.

---

## Part 6: Main Program

### Argument Handling

```c
int main(int argc, char *argv[]) {
    const char *input_str = argv[1];
    const char *format_str = (argc > 2) ? argv[2] : "all";
```

**Default format:** If no format specified, default to "all".

### Special Cases

```c
    if (strcmp(format_str, "detect-endian") == 0 ||
        strcmp(input_str, "detect-endian") == 0) {
        display_endianness_info();
        return 0;
    }
```

**Handle "detect-endian"** regardless of position in arguments.

```c
    if (strchr(input_str, '.') != NULL) {
        // IP address detected
        display_ip_conversions(input_str);
        return 0;
    }
```

**Detect IP addresses:** If input contains a dot, treat as IP address, not decimal number.

### Parse and Dispatch

```c
    uint32_t value = parse_auto(input_str);

    if (strcmp(format_str, "all") == 0) {
        display_all_conversions(value);
    } else if (strcmp(format_str, "binary") == 0) {
        char buffer[BINARY_STR_MAX];
        format_binary(value, buffer, sizeof(buffer));
        printf("Binary: %s\n", buffer);
    }
    // ... more format options ...
```

**Dispatch to appropriate function** based on requested format.

---

## Key Patterns You Should Remember

### 1. Shift + Mask = Extract
```c
int bit = (value >> position) & 1;           // Extract single bit
int field = (value >> start) & ((1 << width) - 1);  // Extract field
```

### 2. Accumulator Pattern = Parsing
```c
result = 0;
for (each_digit) {
    result = (result << base) | digit_value;
}
```

### 3. Clear + Set = Modify
```c
*value &= ~(mask << position);
*value |= (new_bits << position);
```

### 4. Byte Order Conversion
```c
network_value = htonl(host_value);   // Send over network
host_value = ntohl(network_value);   // Receive from network
```

### 5. Buffer Validation
```c
if (buffer_size < REQUIRED_SIZE) return ERROR;
// ... use buffer ...
buffer[index] = '\0';  // Always null-terminate strings
```

---

## Common Mistakes to Avoid

### ❌ Wrong shift direction
```c
// WRONG
int bit = (value & (1 << position));  // Gives 0 or 1<<position

// CORRECT
int bit = (value >> position) & 1;    // Gives 0 or 1
```

### ❌ Forgetting null terminator
```c
// WRONG
buffer[num_digits] = 'X';

// CORRECT
buffer[num_digits] = '\0';
```

### ❌ Not checking buffer size
```c
// WRONG
void format(uint32_t v, char *buf) {
    // No size check - could overflow!
    sprintf(buf, "%d", v);
}

// CORRECT
void format(uint32_t v, char *buf, size_t size) {
    if (size < MIN_SIZE) return ERROR;
    // Use snprintf with size limit
}
```

### ❌ Forgetting endianness
```c
// WRONG - IP address parsed wrong
uint32_t ip = (a << 24) | (b << 16) | (c << 8) | d;

// CORRECT - convert to network byte order
uint32_t ip = htonl((a << 24) | (b << 16) | (c << 8) | d);
```

---

This solution demonstrates professional C programming practices:
- Input validation
- Buffer overflow prevention
- Clear separation of concerns
- Portable code (works on any endianness)
- Comprehensive error handling

Study this solution and apply these patterns to your own code!
